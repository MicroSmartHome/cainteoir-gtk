/* Cainteoir Gtk Application.
 *
 * Copyright (C) 2011 Reece H. Dunn
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

#include <config.h>
#include <cainteoir/engines.hpp>
#include <cainteoir/document.hpp>
#include <cainteoir/platform.hpp>
#include <locale.h>

#undef foreach_iter

#include <gtkmm.h>

namespace rdf = cainteoir::rdf;
namespace rql = cainteoir::rdf::query;

void create_recent_filter(Gtk::RecentFilter & filter, const rdf::graph & aMetadata)
{
	rql::results formats = rql::select(
		rql::select(aMetadata, rql::predicate, rdf::rdf("type")),
		rql::object, rdf::tts("DocumentFormat"));

	for(auto format = formats.begin(), last = formats.end(); format != last; ++format)
	{
		const rdf::uri * uri = rql::subject(*format);

		rql::results mimetypes = rql::select(
			rql::select(aMetadata, rql::predicate, rdf::tts("mimetype")),
			rql::subject, *uri);

		for(auto mimetype = mimetypes.begin(), last = mimetypes.end(); mimetype != last; ++mimetype)
			filter.add_mime_type(rql::value(*mimetype));
	}
}

struct document : public cainteoir::document_events
{
	document()
		: tts(m_metadata)
		, m_doc(new cainteoir::document())
	{
	}

	void metadata(const rdf::statement &aStatement)
	{
		m_metadata.push_back(aStatement);
	}

	const rdf::bnode genid()
	{
		return m_metadata.genid();
	}

	void text(std::tr1::shared_ptr<cainteoir::buffer> aText)
	{
		m_doc->add(aText);
	}

	rdf::graph m_metadata;
	std::tr1::shared_ptr<cainteoir::document> m_doc;
	cainteoir::tts::engines tts;
};

class MetadataViewColumns : public Gtk::TreeModelColumnRecord
{
public:
	MetadataViewColumns()
	{
		add(name);
		add(value);
	}

	Gtk::TreeModelColumn<Glib::ustring> name;
	Gtk::TreeModelColumn<Glib::ustring> value;
};

class MetadataView : public Gtk::ScrolledWindow
{
public:
	MetadataView();

	void add_metadata(const Glib::ustring & name, const Glib::ustring & value);

	void clear()
	{
		data->clear();
	}
private:
	MetadataViewColumns columns;
	Glib::RefPtr<Gtk::ListStore> data;
	Gtk::TreeView view;
};

MetadataView::MetadataView()
{
	data = Gtk::ListStore::create(columns);
	view.set_model(data);

	view.append_column(_("Name"), columns.name);
	view.append_column(_("Description"), columns.value);

	set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	add(view);

	add_metadata(_("Title"), "");
	add_metadata(_("Author"), "");
	add_metadata(_("Publisher"), "");
	add_metadata(_("Description"), "");
	add_metadata(_("Language"), "");
}

void MetadataView::add_metadata(const Glib::ustring & name, const Glib::ustring & value)
{
	Gtk::TreeModel::Row row = *data->append();
	row[columns.name]  = name;
	row[columns.value] = value;
}

class Cainteoir : public Gtk::Window
{
public:
	Cainteoir();
protected:
	void on_open_document();
	void on_recent_files_dialog();
	void on_recent_file();

	void load_document(std::string filename);
private:
	Gtk::VBox box;
	MetadataView metadata;

	Glib::RefPtr<Gtk::UIManager> uiManager;
	Glib::RefPtr<Gtk::ActionGroup> actions;

	Gtk::RecentFilter recentFilter;
	Glib::RefPtr<Gtk::RecentManager> recentManager;
	Glib::RefPtr<Gtk::RecentAction> recentAction;

	document doc;
};

Cainteoir::Cainteoir()
{
	set_title(_("Cainteoir Text-to-Speech"));
	set_size_request(600, 400);

	actions = Gtk::ActionGroup::create();
	uiManager = Gtk::UIManager::create();
	recentManager = Gtk::RecentManager::get_default();

	create_recent_filter(recentFilter, doc.m_metadata);

	actions->add(Gtk::Action::create("FileMenu", _("_File")));
	actions->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN), sigc::mem_fun(*this, &Cainteoir::on_open_document));
	actions->add(Gtk::Action::create("FileRecentDialog", _("Recent Files _Dialog")), sigc::mem_fun(*this, &Cainteoir::on_recent_files_dialog));
	actions->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT), sigc::mem_fun(*this, &Cainteoir::hide));

	recentAction = Gtk::RecentAction::create("FileRecentFiles", _("_Recent Files"));
	recentAction->signal_item_activated().connect(sigc::mem_fun(*this, &Cainteoir::on_recent_file));
	actions->add(recentAction);

	recentAction->set_show_numbers(true);
	recentAction->set_sort_type(Gtk::RECENT_SORT_MRU);
	recentAction->set_filter(recentFilter);

	uiManager->insert_action_group(actions);
	add_accel_group(uiManager->get_accel_group());

	uiManager->add_ui_from_string(
		"<ui>"
		"	<menubar name='MenuBar'>"
		"		<menu action='FileMenu'>"
		"			<menuitem action='FileOpen'/>"
		"			<menuitem action='FileRecentFiles'/>"
		"			<menuitem action='FileRecentDialog'/>"
		"			<separator/>"
		"			<menuitem action='FileQuit'/>"
		"		</menu>"
		"	</menubar>"
		"	<toolbar  name='ToolBar'>"
		"		<toolitem action='FileOpen'/>"
		"	</toolbar>"
		"</ui>");

	add(box);
	box.pack_start(*uiManager->get_widget("/MenuBar"), Gtk::PACK_SHRINK);
	box.pack_start(*uiManager->get_widget("/ToolBar"), Gtk::PACK_SHRINK);
	box.pack_start(metadata);

	show_all_children();
}

void Cainteoir::on_open_document()
{
	Gtk::FileChooserDialog dialog(_("Open Document"), Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	rql::results formats = rql::select(
		rql::select(doc.m_metadata, rql::predicate, rdf::rdf("type")),
		rql::object, rdf::tts("DocumentFormat"));

	for(auto format = formats.begin(), last = formats.end(); format != last; ++format)
	{
		const rdf::uri * uri = rql::subject(*format);

		Gtk::FileFilter filter;
		filter.set_name(rql::select_value<std::string>(doc.m_metadata, *uri, rdf::dc("title")));

		rql::results mimetypes = rql::select(
			rql::select(doc.m_metadata, rql::predicate, rdf::tts("mimetype")),
			rql::subject, *uri);

		for(auto mimetype = mimetypes.begin(), last = mimetypes.end(); mimetype != last; ++mimetype)
			filter.add_mime_type(rql::value(*mimetype));

 		dialog.add_filter(filter);
	}

	if (dialog.run() == Gtk::RESPONSE_OK)
		load_document(dialog.get_filename());
}

void Cainteoir::on_recent_files_dialog()
{
	Gtk::RecentChooserDialog dialog(*this, _("Recent Files"), recentManager);
	dialog.add_button(_("Select Document"), Gtk::RESPONSE_OK);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

	dialog.set_filter(recentFilter);
	dialog.set_sort_type(Gtk::RECENT_SORT_MRU);

	const int response = dialog.run();
	dialog.hide();
	if (response == Gtk::RESPONSE_OK)
		load_document(dialog.get_current_uri());
}

void Cainteoir::on_recent_file()
{
	load_document(recentAction->get_current_uri());
}

void Cainteoir::load_document(std::string filename)
{
	doc.m_doc->clear();
	try
	{
		if (filename.find("file://") == 0)
			filename.erase(0, 7);

		if (cainteoir::parseDocument(filename.c_str(), doc))
		{
			recentManager->add_item(filename);

			rdf::uri subject(filename, std::string());

			metadata.clear();
			metadata.add_metadata(_("Title"), rql::select_value<std::string>(doc.m_metadata, subject, rdf::dc("title")));
			metadata.add_metadata(_("Author"), rql::select_value<std::string>(doc.m_metadata, subject, rdf::dc("creator")));
			metadata.add_metadata(_("Publisher"), rql::select_value<std::string>(doc.m_metadata, subject, rdf::dc("publisher")));
			metadata.add_metadata(_("Description"), rql::select_value<std::string>(doc.m_metadata, subject, rdf::dc("description")));
			metadata.add_metadata(_("Language"), rql::select_value<std::string>(doc.m_metadata, subject, rdf::dc("language")));
		}
	}
	catch (const std::exception & e)
	{
		Gtk::MessageDialog dialog(*this, _("Unable to load the document"), false, Gtk::MESSAGE_ERROR);
		dialog.set_title(_("Open Document"));
		dialog.set_secondary_text(e.what());
		dialog.run();
	}
}

int main(int argc, char ** argv)
{
	cainteoir::initialise();

	setlocale(LC_MESSAGES, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	Gtk::Main app(argc, argv);

	Cainteoir window;

	Gtk::Main::run(window);

	cainteoir::cleanup();
	return 0;
}
