/* A GTK+ wrapper around the DocumentFormat/AudioFormat metadata.
 *
 * Copyright (C) 2014 Reece H. Dunn
 *
 * This file is part of cainteoir-gtk.
 *
 * cainteoir-gtk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cainteoir-gtk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cainteoir-gtk.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gtk/gtk.h>

#include <cainteoir-gtk/cainteoir_supported_formats.h>
#include <cainteoir/document.hpp>
#include <cainteoir/audio.hpp>

namespace rdf = cainteoir::rdf;
namespace rql = cainteoir::rdf::query;

struct _CainteoirSupportedFormatsPrivate
{
	rdf::graph metadata;
};

G_DEFINE_TYPE_WITH_PRIVATE(CainteoirSupportedFormats, cainteoir_supported_formats, G_TYPE_OBJECT)

static void
cainteoir_supported_formats_finalize(GObject *object)
{
	CainteoirSupportedFormats *doc = CAINTEOIR_SUPPORTED_FORMATS(object);
	doc->priv->~CainteoirSupportedFormatsPrivate();

	G_OBJECT_CLASS(cainteoir_supported_formats_parent_class)->finalize(object);
}

static void
cainteoir_supported_formats_class_init(CainteoirSupportedFormatsClass *klass)
{
	GObjectClass *object = G_OBJECT_CLASS(klass);
	object->finalize = cainteoir_supported_formats_finalize;
}

static void
cainteoir_supported_formats_init(CainteoirSupportedFormats *doc)
{
	void * data = cainteoir_supported_formats_get_instance_private(doc);
	doc->priv = new (data)CainteoirSupportedFormatsPrivate();
}

CainteoirSupportedFormats *
cainteoir_supported_formats_new(CainteoirFormatType type)
{
	CainteoirSupportedFormats *formats = CAINTEOIR_SUPPORTED_FORMATS(g_object_new(CAINTEOIR_TYPE_SUPPORTED_FORMATS, nullptr));
	switch (type)
	{
	case CAINTEOIR_FORMAT_DOCUMENT:
		cainteoir::supportedDocumentFormats(formats->priv->metadata, cainteoir::text_support);
		break;
	case CAINTEOIR_FORMAT_AUDIO:
		cainteoir::supported_audio_formats(formats->priv->metadata);
		break;
	default:
		fprintf(stderr, "error: unknown cainteoir format type\n");
		g_object_unref(formats);
		return nullptr;
	}
	return formats;
}
