#include "mystic_book_editor.h"

// GObject definition
struct _MysticBookEditor
{
	GtkWidget parent;
	GtkWidget *scrolled_window;
	GtkWidget *text_view;
};

GtkWidget * 
mystic_book_editor_new()
{
	return GTK_WIDGET(g_object_new(MYSTIC_BOOK_TYPE_EDITOR, NULL));
}


/* Private */

static void mystic_book_editor_dispose(GObject *object);
static void mystic_book_editor_finalize(GObject *object);

G_DEFINE_TYPE(MysticBookEditor, mystic_book_editor, GTK_TYPE_WIDGET)

static void mystic_book_editor_init(MysticBookEditor *self) 
{
	self->scrolled_window = gtk_scrolled_window_new();
	self->text_view = gtk_text_view_new();

	gtk_widget_set_hexpand(self->scrolled_window, true);
	
	gtk_scrolled_window_set_child(
		GTK_SCROLLED_WINDOW(self->scrolled_window),
		self->text_view
	);

	gtk_widget_set_parent(
		self->scrolled_window,
		GTK_WIDGET(self)
	);

}

static void mystic_book_editor_class_init(MysticBookEditorClass *klass)
{
	gtk_widget_class_set_layout_manager_type(
		GTK_WIDGET_CLASS(klass),
		GTK_TYPE_BOX_LAYOUT
	);
}
