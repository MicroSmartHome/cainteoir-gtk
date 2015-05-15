/* Cainteoir Main Window
 *
 * Copyright (C) 2011-2014 Reece H. Dunn
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

#ifndef CAINTEOIRGTK_SRC_CAINTEOIR_HPP
#define CAINTEOIRGTK_SRC_CAINTEOIR_HPP

#include <cainteoir/locale.hpp>
#include <cainteoir-gtk/cainteoir_document.h>
#include <cainteoir-gtk/cainteoir_supported_formats.h>
#include <cainteoir-gtk/cainteoir_settings.h>

#include "settings.hpp"
#include "settings_view.hpp"
#include "voice_selection.hpp"
#include "metadata.hpp"
#include "timebar.hpp"
#include "library.hpp"
#include "navbar.hpp"
#include "toc.hpp"

class Cainteoir
{
public:
	Cainteoir(const char *filename);
	~Cainteoir();

	bool load_document(std::string filename, bool suppress_error_message=false);

	void read();
	void record();
	void stop();

	void save_settings();
	bool on_speaking();

	operator GtkWindow *() const { return GTK_WINDOW(window); }
protected:
	void on_open_document();

	void on_speak(const char * state);

	bool switch_voice(const rdf::uri &voice);

	bool switch_voice_by_language(const std::string &language);
private:
	void updateProgress(double elapsed, double total, double completed);

	CainteoirSupportedFormats *mDocumentFormats;
	CainteoirSupportedFormats *mAudioFormats;

	std::shared_ptr<TimeBar> timebar;
	TocPane toc;
	NavigationBar navbar;

	GtkWidget *window;
	GtkWidget *view;

	MetadataView doc_metadata;
	MetadataView voice_metadata;
	MetadataView engine_metadata;
	GtkWidget *metadata_view;

	GtkWidget *docview;
	GtkTextTagTable *tags;

	std::shared_ptr<VoiceSelectionView> voiceSelection;
	std::shared_ptr<SettingsView> settingsView;
	std::shared_ptr<DocumentLibrary> library;
	GtkWidget *library_button;
	GtkWidget *document_button;
	GtkWidget *info_button;

	GtkRecentManager *recentManager;

	GtkAction *readAction;
	GtkAction *stopAction;
	GtkAction *recordAction;
	GtkAction *openAction;

	rdf::graph tts_metadata;
	cainteoir::tts::engines tts;

	CainteoirDocument *mDocument;
	rdf::uri subject;

	cainteoir::languages languages;
	std::shared_ptr<cainteoir::tts::speech> speech;
	std::shared_ptr<cainteoir::audio> out;
	application_settings settings;

	CainteoirSettings *mSettings;
};

#endif
