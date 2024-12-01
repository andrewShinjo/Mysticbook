#include "./gfile_list_view.h"
#include "./mb_file_browser.h"
#include "../file_service.h"

/* Private declaration */

static void clicked(GtkButton *button, gpointer user_data);

/* Public implementation */

GtkWidget* mb_file_browser_new(MbAppWindow *app_window)
{
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *button_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *new_file_button = gtk_button_new_with_label("New File");
	GtkWidget *file_list_view = gfile_list_view_new(app_window);

	gtk_box_append(GTK_BOX(button_panel), new_file_button);

	gtk_box_append(GTK_BOX(vbox), button_panel);
	gtk_box_append(GTK_BOX(vbox), file_list_view);

	gtk_widget_set_vexpand(file_list_view, TRUE);

	g_signal_connect(new_file_button, "clicked", G_CALLBACK(clicked), file_list_view);

	return vbox;
}

/* Private implementation */

static void clicked(GtkButton *button, gpointer user_data)
{
	GtkListView *file_list_view = GTK_LIST_VIEW(user_data);
	gchar* filename = file_service_create_unique_filename();
	GFile *file = file_service_create_file(filename);
	gfile_list_view_insert_gfile(file_list_view, file);
	g_free(filename);
}
