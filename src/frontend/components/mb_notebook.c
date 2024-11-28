#include "./mb_notebook.h"

GtkWidget* mb_notebook_new()
{
	return gtk_notebook_new();
}

int mb_notebook_append_page(GtkNotebook *notebook, GtkWidget *page)
{
	GtkWidget *label = gtk_label_new("Tab");
	return gtk_notebook_append_page(notebook, page, label);
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
