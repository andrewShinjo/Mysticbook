#include <gtk/gtk.h>
#include "components/mb_app_window.h"
#include "file_service.h"

#define APPLICATION_ID "org.mysticbook.mysticbook"
#define WINDOW_TITLE "Mysticbook"
#define DEFAULT_STYLESHEET_PATH "./stylesheet.css"

static void activate(GtkApplication *application, gpointer user_data)
{
  GtkWidget *mb_app_window = mb_app_window_new(application);
  GtkWindow *window = GTK_WINDOW(mb_app_window);
	gtk_window_set_default_size(window, 1920 / 2, 1080 / 2);
  gtk_window_set_title(window, WINDOW_TITLE);
  gtk_window_present(window);
}

int main (int argc, char *argv[])
{
	GtkApplication *app = gtk_application_new(APPLICATION_ID, G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}
