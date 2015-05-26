/* Display the available speech synthesis voices object.
 *
 * Copyright (C) 2015 Reece H. Dunn
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

#ifndef CAINTEOIR_GTK_CAINTEOIR_SPEECH_VOICE_VIEW_H
#define CAINTEOIR_GTK_CAINTEOIR_SPEECH_VOICE_VIEW_H

#include "cainteoir_speech_synthesizers.h"
#include "cainteoir_document.h"

G_BEGIN_DECLS

#define CAINTEOIR_TYPE_SPEECH_VOICE_VIEW \
	(cainteoir_speech_voice_view_get_type())
#define CAINTEOIR_SPEECH_VOICE_VIEW(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), CAINTEOIR_TYPE_SPEECH_VOICE_VIEW, CainteoirSpeechVoiceView))
#define CAINTEOIR_SPEECH_VOICE_VIEW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), CAINTEOIR_TYPE_SPEECH_VOICE_VIEW, CainteoirSpeechVoiceViewClass))
#define CAINTEOIR_IS_SPEECH_VOICE_VIEW(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), CAINTEOIR_TYPE_SPEECH_VOICE_VIEW))
#define CAINTEOIR_IS_SPEECH_VOICE_VIEW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), CAINTEOIR_TYPE_SPEECH_VOICE_VIEW))
#define CAINTEOIR_SPEECH_VOICE_VIEW_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), CAINTEOIR_TYPE_SPEECH_VOICE_VIEW, CainteoirSpeechVoiceViewClass))

typedef struct _CainteoirSpeechVoiceView        CainteoirSpeechVoiceView;
typedef struct _CainteoirSpeechVoiceViewPrivate CainteoirSpeechVoiceViewPrivate;
typedef struct _CainteoirSpeechVoiceViewClass   CainteoirSpeechVoiceViewClass;

struct _CainteoirSpeechVoiceView
{
	GtkTreeView widget;

	/*< private >*/
	CainteoirSpeechVoiceViewPrivate *priv;
};

struct _CainteoirSpeechVoiceViewClass
{
	GtkTreeViewClass parent_class;

	/* Padding for future expansion */
	void (*_reserved1)(void);
	void (*_reserved2)(void);
	void (*_reserved3)(void);
	void (*_reserved4)(void);
};

GType                                  cainteoir_speech_voice_view_get_type(void) G_GNUC_CONST;

GtkWidget *                            cainteoir_speech_voice_view_new(CainteoirSpeechSynthesizers *synthesizers);

void                                   cainteoir_speech_voice_view_filter_by_language(CainteoirSpeechVoiceView *view,
                                                                                      const gchar *language);

void                                   cainteoir_speech_voice_view_filter_by_document_language(CainteoirSpeechVoiceView *view,
                                                                                               CainteoirDocument *document);

G_END_DECLS

#endif
