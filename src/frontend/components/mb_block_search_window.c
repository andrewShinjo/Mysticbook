#include "../../backend/service/block_service.h"
#include "../../backend/block.h"
#include "./mb_block_search_entry.h"
#include "./mb_block_search_window.h"

/* Widget definition */
struct _MbBlockSearch
{
	GtkWindow parent;
	/* Reference */
	MbAppWindow *app_window;
	/* Widgets */
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *entry;
	GtkWidget *list_box;
	/* Event listeners */
	/* Properties */
};
G_DEFINE_TYPE(MbBlockSearch, mb_block_search, GTK_TYPE_WINDOW)
/* Forward declaration */
static void dispose(GObject *object);
static void finalize(GObject *object);
/* Callback */
static void on_changed(GtkEditable *self, gpointer user_data)
{
	g_print("changed\n");
}
/* Properties */
/* Signals */
/* Widget lifecycle */
static void mb_block_search_init(MbBlockSearch *self)
{
	/* Instantiate widgets */
	self->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
	self->label = gtk_label_new("Search for a block.");
	self->entry = gtk_entry_new();
	self->list_box = gtk_list_box_new();
	/* Configure widgets */
	gtk_widget_set_margin_start(self->vbox, 18);
	gtk_widget_set_margin_end(self->vbox, 18);
	gtk_widget_set_margin_top(self->vbox, 18);
	gtk_widget_set_margin_bottom(self->vbox, 18);
	gtk_window_set_child(GTK_WINDOW(self), self->vbox);
	gtk_box_append(GTK_BOX(self->vbox), self->label);
	gtk_box_append(GTK_BOX(self->vbox), self->entry);
	gtk_box_append(GTK_BOX(self->vbox), self->list_box);
	/* Connect to signals */
	g_signal_connect(self->entry, "changed", G_CALLBACK(on_changed), self);
}
static void mb_block_search_class_init(MbBlockSearchClass *klass)
{

}
static void dispose(GObject *object) {}
static void finalize(GObject *object) {}
/* Public implementation */
GtkWidget* mb_block_search_window_new(MbAppWindow *app_window)
{
	GtkWidget *self = g_object_new(MB_TYPE_BLOCK_SEARCH, NULL);	
	MB_BLOCK_SEARCH(self)->app_window = app_window;
	return self;
}
/* Refactor these. */

/*
static void on_changed(GtkEditable *self, gpointer user_data)
{
	GtkListBox *list_box = GTK_LIST_BOX(user_data);
	gtk_list_box_remove_all(list_box);

	const char *text = gtk_editable_get_text(self);
	GArray *matching_blocks = block_service_get_10_best_matching_blocks(text);

	for(int i=0; i < matching_blocks->len; i++)
	{
		BlockFts5 b = g_array_index(matching_blocks, BlockFts5, i);
		GtkWidget *block_search_entry = mb_block_search_entry_new(b.content, b.id);
		gtk_list_box_append(list_box, block_search_entry);
	}
	g_free(matching_blocks);
}
*/
