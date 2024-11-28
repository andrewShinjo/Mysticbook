#include "./mb_notebook.h"

GtkWidget* mb_notebook_new()
{
	return gtk_notebook_new();
}

int mb_notebook_append_page(GtkNotebook *notebook, GtkWidget *page)
{
	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label = gtk_label_new("Tab");
	GtkWidget *button = gtk_button_new_with_label("X");
	gtk_box_append(GTK_BOX(hbox), label);
	gtk_box_append(GTK_BOX(hbox), button);
	gtk_widget_set_hexpand(label, TRUE);
	return gtk_notebook_append_page(notebook, page, hbox);
}

void mb_notebook_replace_page_content(GtkNotebook *notebook, GtkWidget *page)
{
	int current_page_number = gtk_notebook_get_current_page(notebook);
	gtk_notebook_remove_page(notebook, current_page_number);
	GtkWidget *label = gtk_label_new("Tab");
	gtk_notebook_insert_page(notebook, page, label, current_page_number);
}

void mb_notebook_set_current_page(GtkNotebook *notebook, int page_number)
{
	gtk_notebook_set_current_page(notebook, page_number);
}
