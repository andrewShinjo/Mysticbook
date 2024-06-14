// Author: Andrew Shinjo
// Date: 05-29-2024

#include <gtk/gtk.h>
#include "text_block.h"

static void
activate(
    GtkApplication *app,
    gpointer user_data
)
{

    GtkSettings *settings = gtk_settings_get_default();

    GtkWidget *scrolled_window;
    GtkWidget *text_block;
    GtkWidget *window;
    
    GtkEventController *keyEventController;

    // Configure initial settings
    {
        g_object_set(settings, "gtk-font-name", "Ubuntu 12", NULL);
    }

    // Instantiate widgets
    {
        scrolled_window = gtk_scrolled_window_new();
        text_block = text_block_new();
        window = gtk_application_window_new(app);
    }

    {
        gtk_application_window_set_show_menubar (GTK_APPLICATION_WINDOW (window), TRUE);
        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), text_block);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
        gtk_window_set_title(GTK_WINDOW(window), "Mysticbook");
        gtk_window_set_child(GTK_WINDOW(window), scrolled_window);
        gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);
        gtk_window_present(GTK_WINDOW(window));
    }
}


static void on_startup(GtkApplication *app, gpointer user_data) {
    g_print("on_startup\n");
    GSimpleAction *act_quit = g_simple_action_new ("quit", NULL);
    g_action_map_add_action (G_ACTION_MAP (app), G_ACTION (act_quit));

    GMenu *menubar = g_menu_new ();
    GMenuItem *menu_item_menu = g_menu_item_new ("Menu", NULL);
    GMenu *menu = g_menu_new ();
    GMenuItem *menu_item_quit = g_menu_item_new ("Quit", "app.quit");
    g_menu_append_item (menu, menu_item_quit);
    g_object_unref (menu_item_quit);
    g_menu_item_set_submenu (menu_item_menu, G_MENU_MODEL (menu));
    g_object_unref (menu);
    g_menu_append_item (menubar, menu_item_menu);
    g_object_unref (menu_item_menu);

    gtk_application_set_menubar (GTK_APPLICATION (app), G_MENU_MODEL (menubar));
}

int
main(
    int argc,
    char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.mysticbook.app", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    g_signal_connect(app, "startup", G_CALLBACK(on_startup), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
