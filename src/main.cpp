/* Cainteoir Gtk Application.
 *
 * Copyright (C) 2011-2012 Reece H. Dunn
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
#include <gtkmm.h>
#include <cainteoir/platform.hpp>

#include "cainteoir.hpp"

std::string get_theme_path(const char *theme)
{
	return std::string(DATADIR "/" PACKAGE "/themes/") + theme + std::string("/gtk3.css");
}

int main(int argc, char ** argv)
{
	cainteoir::initialise();

	setlocale(LC_MESSAGES, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	Gtk::Main app(argc, argv);

#if GTK_CHECK_VERSION(3, 0, 0)
	GtkCssProvider *provider = gtk_css_provider_new();
	GdkDisplay *display = gdk_display_get_default();
	GdkScreen *screen = gdk_display_get_default_screen(display);
	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	char *theme_name = NULL;
	g_object_get(gtk_settings_get_default(), "gtk-theme-name", &theme_name, NULL);

	try
	{
		cainteoir::mmap_buffer theme(get_theme_path(theme_name).c_str());
		gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (provider), theme.begin(), theme.size(), NULL);
	}
	catch (const std::exception &e)
	{
	}

	g_free(theme_name);
#endif

	Cainteoir window(argc > 1 ? argv[1] : NULL);
	Gtk::Main::run(window);

	cainteoir::cleanup();
	return 0;
}
