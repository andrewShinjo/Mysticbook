#include <gtk/gtk.h>
#include "components/mb_app_window.h"
#include "../backend/database/database.h"

static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *mb_app_window = mb_app_window_new(app);
  GtkCssProvider *css_provider = gtk_css_provider_new();
  GtkStyleProvider *style_provider = GTK_STYLE_PROVIDER(css_provider);
  gtk_css_provider_load_from_file(
    css_provider, 
    g_file_new_for_path("stylesheet.css")
  );
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), style_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  GtkWindow *window = GTK_WINDOW(mb_app_window);
  gtk_window_set_title(window, "Mysticbook");
  gtk_window_maximize(window);
  gtk_window_present(window);
}

int main (int argc, char *argv[])
{
	int return_code =	open_database("test.db");
	if (return_code != 0)
	{
    g_print("Failed to open database.\n");
		return return_code;
	}
  else
  {
    g_print("Opened database test.db\n");
  }

	GtkApplication *app = gtk_application_new("org.mysticbook.mysticbook", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
  close_database("test.db");
	return status;
}
