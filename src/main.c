// Author: Andrew Shinjo
// Date: 05-29-2024

#include <gtk/gtk.h>
#include "mystic_book_application.h"

#define APPLICATION_ID "org.mysticbook.app"

/*
static void 
activate(
	GtkApplication *app, 
	gpointer user_data
)
{

    GtkSettings *settings = gtk_settings_get_default();
    GtkWidget *scrolled_window;
	GtkWidget *mystic_text_view;
    // GtkWidget *text_block;
    GtkWidget *window;
    
    GtkEventController *keyEventController;

    // Configure initial settings
    {
        g_object_set(
			settings, 
			"gtk-font-name", 
			"Ubuntu 12", 
			NULL
		);
    }

    // Instantiate widgets
    {
        scrolled_window = gtk_scrolled_window_new();
		mystic_text_view = mystic_text_view_new();
        window = gtk_application_window_new(app);
    }

    {
        gtk_application_window_set_show_menubar(
			GTK_APPLICATION_WINDOW(window),
			TRUE
		);

		gtk_scrolled_window_set_child(
			GTK_SCROLLED_WINDOW(scrolled_window),
			mystic_text_view
		);

        gtk_scrolled_window_set_policy(
			GTK_SCROLLED_WINDOW(scrolled_window), 
			GTK_POLICY_NEVER, 
			GTK_POLICY_AUTOMATIC
		);

        gtk_window_set_title(
			GTK_WINDOW(window),
			"Mysticbook"
		);

        gtk_window_set_child(
			GTK_WINDOW(window),
			scrolled_window
		);

        gtk_window_set_default_size(
			GTK_WINDOW(window),
			900, 
			600
		);

        gtk_window_present(GTK_WINDOW(window));
    }
}

static void 
file_open_callback(
	GObject* source_object,
	GAsyncResult* res, 
	gpointer data
) 
{
	GtkApplication *app = GTK_APPLICATION(data);
	GtkFileDialog *file_dialog = GTK_FILE_DIALOG(source_object);
	
	GError *err = NULL;
		
	GFile *file = gtk_file_dialog_open_finish(
		file_dialog, 
		res, 
		&err
	);

	if(file != NULL)
	{

		GtkWindow *window = gtk_application_get_active_window(app);
		GtkWidget *child = gtk_window_get_child(window);
		GtkScrolledWindow* gsw = GTK_SCROLLED_WINDOW(child);
		GtkWidget *mtv = mystic_text_view_new_with_file(file);
		gtk_scrolled_window_set_child(gsw, mtv);
	}
}

static void 
file_open_activated(
	GSimpleAction *action, 
	GVariant *parameter, 
	gpointer app
)
{
    GtkFileDialog *file_dialog = gtk_file_dialog_new();

    gtk_file_dialog_open(
		file_dialog, 
		NULL, 
		NULL, 
		file_open_callback, 
		app
	);
}

// "startup" signal is called only once in the primary app instance
static void on_startup(GtkApplication *app, gpointer user_data) {

    // Initialize menu bar.
    {
        GSimpleAction *file_open_simple_action = g_simple_action_new(
			"file_open", 
			NULL
		);

        g_action_map_add_action(
			G_ACTION_MAP(app), 
			G_ACTION(file_open_simple_action)
		);

        g_signal_connect(
			file_open_simple_action, 
			"activate", 
			G_CALLBACK(file_open_activated), 
			app
		);

        GMenu *file_menu = g_menu_new ();
        GMenu *menubar = g_menu_new ();

        GMenuItem *file_menu_item = g_menu_item_new(
			"File", 
			NULL
		);
        
		GMenuItem *file_open_menu_item = g_menu_item_new(
			"Open", 
			"app.file_open"
		);

        g_menu_item_set_submenu(
			file_menu_item, 
			G_MENU_MODEL (file_menu)
		);

        g_menu_append_item(file_menu, file_open_menu_item);
        g_menu_append_item(menubar, file_menu_item);

        g_object_unref(file_open_menu_item);
        g_object_unref(file_menu);
        g_object_unref(file_menu_item);

        gtk_application_set_menubar(
			GTK_APPLICATION(app), 
			G_MENU_MODEL(menubar)
		);
    }
}
*/


int 
main(int argc, char **argv) 
{
	MysticBookApplication *app;
    int status;

    app = mystic_book_application_new(
		APPLICATION_ID, 
		G_APPLICATION_DEFAULT_FLAGS
	);

	status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}