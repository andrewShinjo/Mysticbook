#include <gtk/gtk.h>
#include <sqlite3.h>
#include "components/mysticbook_application_window.h"
#include "../backend/database.h"

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *mysticbook_application_window = mb_application_window_new (app);
  GtkCssProvider *css_provider = gtk_css_provider_new();
  gtk_css_provider_load_from_file(css_provider, g_file_new_for_path("stylesheet.css"));
  gtk_style_context_add_provider_for_display(
    gdk_display_get_default(), 
    GTK_STYLE_PROVIDER(css_provider), 
    GTK_STYLE_PROVIDER_PRIORITY_USER
  );
  GtkWindow *window = GTK_WINDOW (mysticbook_application_window);
  gtk_window_set_title (window, "Mysticbook");
  gtk_window_maximize (window);
  gtk_window_present (window);
}

int main (int argc, char *argv[])
{

// *********************************************************************
// Open SQLite3 database.
// *********************************************************************

	int return_code =	database_open ("test.db");
	if (return_code != 0)
	{
		fprintf (stderr, "Failed to open database.");
		return return_code;
	}

// *********************************************************************
// Initialize GTK application GUI.
// *********************************************************************

	GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

// *********************************************************************
// Close SQLite3 database.
// *********************************************************************
	
	database_close ();
	return status;
}
