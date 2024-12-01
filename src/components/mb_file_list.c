#include "./mb_file_list.h"

GtkWidget* mb_file_list_new(GList *files)
{
	GtkWidget *list_box = gtk_list_box_new();

	gtk_list_box_set_selection_mode(GTK_LIST_BOX(list_box), GTK_SELECTION_SINGLE);
	
	for(GList *current = files; current != NULL; current = current->next)
	{
		GFile *file = (GFile*) current->data;
		gchar *path = g_file_get_path(file);
		GtkWidget *row = gtk_list_box_row_new();
		GtkWidget *label = gtk_label_new(path);
		gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);
		gtk_list_box_append(GTK_LIST_BOX(list_box), row);
	}

	gtk_list_box_unselect_all(GTK_LIST_BOX(list_box));

	return list_box;
}
