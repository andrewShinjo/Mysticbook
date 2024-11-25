#include "./mb_block_search.h"

GtkWidget* mb_block_search_open()
{
	static GtkWidget *window = NULL;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *entry;

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
		gtk_box_append(GTK_BOX(vbox), label);
		gtk_box_append(GTK_BOX(vbox), entry);
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
