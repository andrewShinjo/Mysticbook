#include "../../backend/service/block_service.h"
#include "../../backend/block.h"
#include "./mb_block_search.h"

static void create_block_search_entry(BlockFts5 block)
{

}

static void on_changed(GtkEditable *self, gpointer user_data)
{
	GtkListBox *list_box = GTK_LIST_BOX(user_data);
	gtk_list_box_remove_all(list_box);

	const char *text = gtk_editable_get_text(self);
	GArray *matching_blocks = block_service_get_10_best_matching_blocks(text);

	for(int i=0; i < matching_blocks->len; i++)
	{
		BlockFts5 b = g_array_index(matching_blocks, BlockFts5, i);
		GtkWidget *label = gtk_label_new(b.content);
		gtk_list_box_append(list_box, label);
	}
}

GtkWidget* mb_block_search_open()
{
	static GtkWidget *window = NULL;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *entry;
	GtkWidget *list_box;

	if(!window)
	{
		window = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(window), "Search Block");
		gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

		vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
		gtk_widget_set_margin_start(vbox, 18);
		gtk_widget_set_margin_end(vbox, 18);
		gtk_widget_set_margin_top(vbox, 18);
		gtk_widget_set_margin_bottom(vbox, 18);
		gtk_window_set_child(GTK_WINDOW(window), vbox);

		label = gtk_label_new("Search for a block.");
		entry = gtk_entry_new();
		list_box = gtk_list_box_new();

		gtk_box_append(GTK_BOX(vbox), label);
		gtk_box_append(GTK_BOX(vbox), entry);
		gtk_box_append(GTK_BOX(vbox), list_box);

		g_signal_connect(entry, "changed", G_CALLBACK(on_changed), list_box);
	}

	if(!gtk_widget_get_visible(window))
	{
		gtk_widget_set_visible(window, TRUE);
	}
	else
	{
		gtk_window_destroy(GTK_WINDOW(window));
	}

	return window;
}
