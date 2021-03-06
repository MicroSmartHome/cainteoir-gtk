/* The Cainteoir Reader document view.
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

#include "config.h"
#include "i18n.h"

#include <gtk/gtk.h>

#include "reader_document_view.h"

#include <cainteoir-gtk/cainteoir_document_view.h>
#include <cainteoir-gtk/cainteoir_document_index.h>
#include <cainteoir-gtk/cainteoir_document.h>
#include <cainteoir-gtk/cainteoir_metadata.h>
#include <cainteoir-gtk/cainteoir_settings.h>

#include "extensions/glib.h"

enum IndexTypeColumns
{
	INDEX_TYPE_LABEL,
	INDEX_TYPE_ID,
	INDEX_TYPE_COUNT,
};

struct ReaderDocumentViewPrivate
{
	GtkWidget *doc_pane;
	GtkWidget *index_type;
	GtkWidget *index_pane_close;
	GtkWidget *index;
	GtkWidget *view;

	CainteoirSettings *settings;

	~ReaderDocumentViewPrivate()
	{
		g_object_unref(G_OBJECT(settings));
	}
};

G_DEFINE_TYPE_WITH_PRIVATE(ReaderDocumentView, reader_document_view, GTK_TYPE_BOX)

#define READER_DOCUMENT_VIEW_PRIVATE(object) \
	((ReaderDocumentViewPrivate *)reader_document_view_get_instance_private(READER_DOCUMENT_VIEW(object)))

GXT_DEFINE_TYPE_CONSTRUCTION(ReaderDocumentView, reader_document_view, READER_DOCUMENT_VIEW)

static void
reader_document_view_class_init(ReaderDocumentViewClass *klass)
{
	GObjectClass *object = G_OBJECT_CLASS(klass);
	object->finalize = reader_document_view_finalize;
}

static void
on_document_view_show(GtkWidget *widget, ReaderDocumentViewPrivate *priv)
{
	gtk_paned_set_position(GTK_PANED(priv->doc_pane),
	                       cainteoir_settings_get_integer(priv->settings, "index", "position", INDEX_PANE_WIDTH));

	if (cainteoir_settings_get_boolean(priv->settings, "index", "visible", TRUE))
		gtk_widget_show(gtk_paned_get_child1(GTK_PANED(priv->doc_pane)));
	else
		gtk_widget_hide(gtk_paned_get_child1(GTK_PANED(priv->doc_pane)));
}

static GtkWidget *
create_index_type_combo(void)
{
	GtkTreeStore *store = gtk_tree_store_new(INDEX_TYPE_COUNT, G_TYPE_STRING, G_TYPE_STRING);
	GtkTreeIter row;

	gtk_tree_store_append(store, &row, nullptr);
	gtk_tree_store_set(store, &row,
	                   INDEX_TYPE_LABEL, i18n("Index"),
	                   INDEX_TYPE_ID,    CAINTEOIR_INDEXTYPE_TOC,
	                   -1);

	gtk_tree_store_append(store, &row, nullptr);
	gtk_tree_store_set(store, &row,
	                   INDEX_TYPE_LABEL, i18n("Index (Abridged)"),
	                   INDEX_TYPE_ID,    CAINTEOIR_INDEXTYPE_TOC_BRIEF,
	                   -1);

	gtk_tree_store_append(store, &row, nullptr);
	gtk_tree_store_set(store, &row,
	                   INDEX_TYPE_LABEL, i18n("Landmarks"),
	                   INDEX_TYPE_ID,    CAINTEOIR_INDEXTYPE_LANDMARKS,
	                   -1);

	gtk_tree_store_append(store, &row, nullptr);
	gtk_tree_store_set(store, &row,
	                   INDEX_TYPE_LABEL, i18n("Pages"),
	                   INDEX_TYPE_ID,    CAINTEOIR_INDEXTYPE_PAGE_LIST,
	                   -1);

	gtk_tree_store_append(store, &row, nullptr);
	gtk_tree_store_set(store, &row,
	                   INDEX_TYPE_LABEL, i18n("Illustrations"),
	                   INDEX_TYPE_ID,    CAINTEOIR_INDEXTYPE_ILLUSTRATIONS,
	                   -1);

	gtk_tree_store_append(store, &row, nullptr);
	gtk_tree_store_set(store, &row,
	                   INDEX_TYPE_LABEL, i18n("Tables"),
	                   INDEX_TYPE_ID,    CAINTEOIR_INDEXTYPE_TABLES,
	                   -1);

	gtk_tree_store_append(store, &row, nullptr);
	gtk_tree_store_set(store, &row,
	                   INDEX_TYPE_LABEL, i18n("Audio"),
	                   INDEX_TYPE_ID,    CAINTEOIR_INDEXTYPE_AUDIO,
	                   -1);

	gtk_tree_store_append(store, &row, nullptr);
	gtk_tree_store_set(store, &row,
	                   INDEX_TYPE_LABEL, i18n("Video"),
	                   INDEX_TYPE_ID,    CAINTEOIR_INDEXTYPE_VIDEO,
	                   -1);

	GtkWidget *combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtk_combo_box_set_id_column(GTK_COMBO_BOX(combo), INDEX_TYPE_ID);

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), renderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), renderer, "text", INDEX_TYPE_LABEL, nullptr);

	g_object_unref(G_OBJECT(store));
	return combo;
}

static void
on_index_type_changed(GtkWidget *widget, ReaderDocumentViewPrivate *priv)
{
	CainteoirDocument *doc = cainteoir_document_view_get_document(CAINTEOIR_DOCUMENT_VIEW(priv->view));
	if (doc)
	{
		cainteoir_document_index_build(CAINTEOIR_DOCUMENT_INDEX(priv->index), doc,
		                               gtk_combo_box_get_active_id(GTK_COMBO_BOX(priv->index_type)));
		g_object_unref(G_OBJECT(doc));
	}
}

GtkWidget *
reader_document_view_new(CainteoirSettings *settings)
{
	ReaderDocumentView *view = READER_DOCUMENT_VIEW(g_object_new(READER_TYPE_DOCUMENT_VIEW, nullptr));
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	priv->settings = CAINTEOIR_SETTINGS(g_object_ref(G_OBJECT(settings)));

	priv->doc_pane = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_set_hexpand(priv->doc_pane, TRUE);
	gtk_container_add(GTK_CONTAINER(view), priv->doc_pane);

	GtkWidget *view_scroll = gtk_scrolled_window_new(nullptr, nullptr);
	gtk_widget_set_size_request(view_scroll, DOCUMENT_PANE_WIDTH, -1);
	gtk_paned_pack2(GTK_PANED(priv->doc_pane), view_scroll, TRUE, FALSE);

	priv->view  = cainteoir_document_view_new();
	gtk_container_add(GTK_CONTAINER(view_scroll), priv->view);

	GtkWidget *index_pane = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_widget_set_size_request(index_pane, INDEX_PANE_WIDTH, -1);
	gtk_paned_pack1(GTK_PANED(priv->doc_pane), index_pane, TRUE, FALSE);

	GtkWidget *index_pane_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(index_pane), index_pane_header, FALSE, FALSE, 0);

	priv->index_type = create_index_type_combo();
	gtk_box_pack_start(GTK_BOX(index_pane_header), priv->index_type, TRUE, TRUE, 0);
	g_signal_connect(priv->index_type, "changed", G_CALLBACK(on_index_type_changed), priv);

	priv->index_pane_close = gtk_button_new_from_icon_name("window-close", GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_style_context_add_class(gtk_widget_get_style_context(priv->index_pane_close), "close-pane");
	gtk_button_set_always_show_image(GTK_BUTTON(priv->index_pane_close), TRUE);
	gtk_button_set_relief(GTK_BUTTON(priv->index_pane_close), GTK_RELIEF_NONE);
	gtk_box_pack_start(GTK_BOX(index_pane_header), priv->index_pane_close, FALSE, FALSE, 0);

	GtkWidget *index_scroll = gtk_scrolled_window_new(nullptr, nullptr);
	gtk_box_pack_start(GTK_BOX(index_pane), index_scroll, TRUE, TRUE, 0);

	priv->index = cainteoir_document_index_new(CAINTEOIR_DOCUMENT_VIEW(priv->view));
	gtk_container_add(GTK_CONTAINER(index_scroll), priv->index);

	g_signal_connect(view, "show", G_CALLBACK(on_document_view_show), priv);

	gchar *active_index = cainteoir_settings_get_string(priv->settings, "index", "type", CAINTEOIR_INDEXTYPE_TOC);
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(priv->index_type), active_index);
	g_free(active_index);

	return GTK_WIDGET(view);
}

void
reader_document_view_set_index_pane_close_action_name(ReaderDocumentView *view,
                                                      const gchar *action_name)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	gtk_actionable_set_action_name(GTK_ACTIONABLE(priv->index_pane_close), action_name);
}

gboolean
reader_document_view_load_document(ReaderDocumentView *view,
                                   const gchar *filename)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	CainteoirDocument *doc = cainteoir_document_new(filename);
	if (doc)
	{
		cainteoir_document_view_set_document(CAINTEOIR_DOCUMENT_VIEW(priv->view), doc);

		CainteoirMetadata *metadata = cainteoir_document_get_metadata(doc);
		gchar *mimetype = cainteoir_metadata_get_string(metadata, CAINTEOIR_METADATA_MIMETYPE);

		cainteoir_settings_set_string(priv->settings, "document", "filename", filename);
		cainteoir_settings_set_string(priv->settings, "document", "mimetype", mimetype);
		cainteoir_settings_save(priv->settings);

		cainteoir_document_index_build(CAINTEOIR_DOCUMENT_INDEX(priv->index), doc,
		                               gtk_combo_box_get_active_id(GTK_COMBO_BOX(priv->index_type)));

		if (mimetype) g_free(mimetype);
		g_object_unref(metadata);
		g_object_unref(doc);
		return TRUE;
	}
	return FALSE;
}

CainteoirDocument *
reader_document_view_get_document(ReaderDocumentView *view)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	return cainteoir_document_view_get_document(CAINTEOIR_DOCUMENT_VIEW(priv->view));
}

CainteoirDocumentIndex *
reader_document_view_get_document_index(ReaderDocumentView *view)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	return CAINTEOIR_DOCUMENT_INDEX(g_object_ref(G_OBJECT(priv->index)));
}

const gchar *
reader_document_view_get_index_type(ReaderDocumentView *view)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	return gtk_combo_box_get_active_id(GTK_COMBO_BOX(priv->index_type));
}

void
reader_document_view_set_index_type(ReaderDocumentView *view,
                                    const gchar *index_type)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(priv->index_type), index_type);
}

gint
reader_document_view_get_index_pane_position(ReaderDocumentView *view)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	return gtk_paned_get_position(GTK_PANED(priv->doc_pane));
}

void
reader_document_view_set_index_pane_position(ReaderDocumentView *view,
                                             gint position)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	gtk_paned_set_position(GTK_PANED(priv->doc_pane), position);
}

gboolean
reader_document_view_get_index_pane_visible(ReaderDocumentView *view)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	return gtk_widget_get_visible(gtk_paned_get_child1(GTK_PANED(priv->doc_pane)));
}

void
reader_document_view_set_index_pane_visible(ReaderDocumentView *view,
                                            gboolean visible)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	if (visible)
		gtk_widget_show(gtk_paned_get_child1(GTK_PANED(priv->doc_pane)));
	else
		gtk_widget_hide(gtk_paned_get_child1(GTK_PANED(priv->doc_pane)));
}

void
reader_document_view_select_text(ReaderDocumentView *view,
                                 gint start_pos,
                                 gint end_pos,
                                 GtkAlign anchor)
{
	ReaderDocumentViewPrivate *priv = READER_DOCUMENT_VIEW_PRIVATE(view);
	cainteoir_document_view_select_text(CAINTEOIR_DOCUMENT_VIEW(priv->view), start_pos, end_pos, anchor);
}
