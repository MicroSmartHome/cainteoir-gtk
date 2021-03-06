/* Audio Waveform Viewer.
 *
 * Copyright (C) 2014-2015 Reece H. Dunn
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

#ifndef CAINTEOIR_GTK_CAINTEOIR_WAVEFORM_VIEW_H
#define CAINTEOIR_GTK_CAINTEOIR_WAVEFORM_VIEW_H

#include <stdint.h>

typedef struct _CainteoirAudioDataS16  CainteoirAudioDataS16;

G_BEGIN_DECLS

#define CAINTEOIR_TYPE_WAVEFORM_VIEW \
	(cainteoir_waveform_view_get_type())
#define CAINTEOIR_WAVEFORM_VIEW(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), CAINTEOIR_TYPE_WAVEFORM_VIEW, CainteoirWaveformView))
#define CAINTEOIR_WAVEFORM_VIEW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), CAINTEOIR_TYPE_WAVEFORM_VIEW, CainteoirWaveformViewClass))
#define CAINTEOIR_IS_WAVEFORM_VIEW(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), CAINTEOIR_TYPE_WAVEFORM_VIEW))
#define CAINTEOIR_IS_WAVEFORM_VIEW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), CAINTEOIR_TYPE_WAVEFORM_VIEW))
#define CAINTEOIR_WAVEFORM_VIEW_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), CAINTEOIR_TYPE_WAVEFORM_VIEW, CainteoirWaveformViewClass))

typedef struct _CainteoirWaveformView        CainteoirWaveformView;
typedef struct _CainteoirWaveformViewClass   CainteoirWaveformViewClass;

struct _CainteoirWaveformView
{
	GtkDrawingArea widget;
};

struct _CainteoirWaveformViewClass
{
	GtkDrawingAreaClass parent_class;

	/* Padding for future expansion */
	void (*_gtk_reserved1)(void);
	void (*_gtk_reserved2)(void);
	void (*_gtk_reserved3)(void);
	void (*_gtk_reserved4)(void);
};

GType                                  cainteoir_waveform_view_get_type(void) G_GNUC_CONST;

GtkWidget *                            cainteoir_waveform_view_new(void);

void                                   cainteoir_waveform_view_set_s16_data(CainteoirWaveformView *view,
                                                                            CainteoirAudioDataS16 *data);

CainteoirAudioDataS16 *                cainteoir_waveform_view_get_s16_data(CainteoirWaveformView *view);

void                                   cainteoir_waveform_view_set_window_size(CainteoirWaveformView *view,
                                                                               uint16_t window_size);

uint16_t                               cainteoir_waveform_view_get_window_size(CainteoirWaveformView *view);

void                                   cainteoir_waveform_view_set_maximum_height(CainteoirWaveformView *view,
                                                                                  uint16_t maximum_height);

uint16_t                               cainteoir_waveform_view_get_maximum_height(CainteoirWaveformView *view);

void                                   cainteoir_waveform_view_set_view_duration(CainteoirWaveformView *view,
                                                                                 float view_duration);

float                                  cainteoir_waveform_view_get_view_duration(CainteoirWaveformView *view);

G_END_DECLS

#endif
