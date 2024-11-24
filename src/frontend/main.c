#include <gtk/gtk.h>
#include "components/mb_app_window.h"
#include "../backend/database/database.h"

#define APPLICATION_ID "org.mysticbook.mysticbook"
#define WINDOW_TITLE "Mysticbook"
#define DEFAULT_STYLESHEET_PATH "./stylesheet.css"
#define DEVELOPMENT_DB "./development.db"

/* 
 * Activates the GTK application.
 *
 */
static void activate(GtkApplication *application, gpointer user_data)
{
  GtkWidget *mb_app_window = mb_app_window_new(application);

  GtkCssProvider *css_provider = gtk_css_provider_new();
  GFile *css_file = g_file_new_for_path(DEFAULT_STYLESHEET_PATH);
  gtk_css_provider_load_from_file(css_provider, css_file);

  GdkDisplay *default_display = gdk_display_get_default();
  GtkStyleProvider *style_provider = GTK_STYLE_PROVIDER(css_provider);
  gtk_style_context_add_provider_for_display(default_display, style_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

  GtkWindow *window = GTK_WINDOW(mb_app_window);
  gtk_window_set_title(window, WINDOW_TITLE);
  gtk_window_maximize(window);
  gtk_window_present(window);
}

/** Setup, and launch application.
 *
 */
int main (int argc, char *argv[])
{
  int return_code =	open_database(DEVELOPMENT_DB);
	if (return_code != 0)
	{
    g_print("Failed to open database.\n");
		return return_code;
	}
  else
  {
    g_print("Opened database test.db\n");
  }

	GtkApplication *app = gtk_application_new(APPLICATION_ID, G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
  close_database(DEVELOPMENT_DB);
	return status;
}
