#include "./gfile_list_view.h"
#include "./mb_file_browser.h"

/* Private declaration */

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

	return vbox;
}

/* Private implementation */
