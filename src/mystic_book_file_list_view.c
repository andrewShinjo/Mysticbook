#include "mystic_book_file_list_view.h"

struct _MysticBookFileListView
{
	GtkWidget parent;
	GtkWidget *list_view;
};

GtkWidget *
mystic_book_file_list_view_new()
{
	return GTK_WIDGET(
		g_object_new(MYSTIC_BOOK_TYPE_FILE_LIST_VIEW, NULL)
	);
}

static void mystic_book_file_list_view_dispose(GObject *object);
static void mystic_book_file_list_view_finalize(GObject *object);

G_DEFINE_TYPE(
	MysticBookFileListView, 
	mystic_book_file_list_view, 
	GTK_TYPE_WIDGET
)

// Emitted to setup a GtkListItem object.
static void
setup(
	GtkSignalListItemFactory *self, 
	GtkListItem *list_item, 
	gpointer user_data
)
{
	GtkWidget *label = gtk_label_new("");
	gtk_list_item_set_child(list_item, label);
}

// Emitted to bind an item in the list model to the widget.
static void
bind(
	GtkSignalListItemFactory *self, 
	GtkListItem *list_item, 
	gpointer user_data
)
{
	GtkStringObject *strobj = gtk_list_item_get_item (list_item);
	GtkWidget *child = gtk_list_item_get_child (list_item);

	gtk_label_set_text(
		GTK_LABEL(child), 
		gtk_string_object_get_string (strobj)
	);
}

static void
mystic_book_file_list_view_init(MysticBookFileListView *self)
{
	GListStore *store = g_list_store_new(GTK_TYPE_STRING_OBJECT);
	GtkSelectionModel *selection = GTK_SELECTION_MODEL(
		gtk_single_selection_new(G_LIST_MODEL(store))
	);

	GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(setup), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(bind), NULL);

	self->list_view = gtk_list_view_new(selection, factory);

	g_autoptr(GtkStringObject) strobj1 = gtk_string_object_new ("foo");
	g_autoptr(GtkStringObject) strobj2 = gtk_string_object_new ("bar");

	g_list_store_append (store, strobj1);
	g_list_store_append (store, strobj2);

	gtk_widget_set_hexpand(self->list_view, true);
	gtk_widget_set_parent(self->list_view, GTK_WIDGET(self));

}

static void
mystic_book_file_list_view_class_init(
	MysticBookFileListViewClass *klass
) 
{
	gtk_widget_class_set_layout_manager_type(
		GTK_WIDGET_CLASS(klass),
		GTK_TYPE_BOX_LAYOUT
	);
}
