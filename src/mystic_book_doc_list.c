#include "mystic_book_doc_list.h"

struct _MysticBookDocList
{
	GtkWidget parent;
	GtkWidget *scrolled_window;
	GtkWidget *list_box;
};

GtkWidget *
mystic_book_doc_list_new()
{
	return GTK_WIDGET(g_object_new(MYSTIC_BOOK_TYPE_DOC_LIST, NULL));
}

static void mystic_book_doc_list_dispose(GObject *object);
static void mystic_book_doc_list_finalize(GObject *object);

G_DEFINE_TYPE(MysticBookDocList, mystic_book_doc_list, GTK_TYPE_WIDGET)

static void
mystic_book_doc_list_init(MysticBookDocList *self)
{
	self->scrolled_window = gtk_scrolled_window_new();
	self->list_box = gtk_list_box_new();

	gtk_widget_set_hexpand(self->scrolled_window, true);

	gtk_scrolled_window_set_child(
		GTK_SCROLLED_WINDOW(self->scrolled_window),
		self->list_box
	);
}

static void
mystic_book_doc_list_class_init(MysticBookDocListClass *klass)
{
	gtk_widget_class_set_layout_manager_type(
		GTK_WIDGET_CLASS(klass),
		GTK_TYPE_BOX_LAYOUT
	);
}
