#include "mystic_book_doc_list.h"
#include "mystic_book_file_list_view.h"

struct _MysticBookDocList
{
	GtkWidget parent;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *scrolled_window;
	GtkWidget *file_list;
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
	self->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	self->scrolled_window = gtk_scrolled_window_new();
	self->label = gtk_label_new("Document List");
	self->file_list = mystic_book_file_list_view_new();

	gtk_box_append(GTK_BOX(self->vbox), self->label);
	gtk_box_append(GTK_BOX(self->vbox), self->file_list);
	gtk_widget_set_parent(self->vbox, GTK_WIDGET(self));
}

static void
mystic_book_doc_list_class_init(MysticBookDocListClass *klass)
{
	gtk_widget_class_set_layout_manager_type(
		GTK_WIDGET_CLASS(klass),
		GTK_TYPE_BOX_LAYOUT
	);
}
