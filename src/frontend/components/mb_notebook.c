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
