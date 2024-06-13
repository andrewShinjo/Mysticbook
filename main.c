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

    GtkWidget *window;
    GtkWidget *text_block;
    GtkWidget *scrolled_window;

    GtkEventController *keyEventController;

    // Configure initial settings
    {
        g_object_set(settings, "gtk-font-name", "Ubuntu 12", NULL);
    }

    // Instantiate widgets
    {
        text_block = text_block_new();
        scrolled_window = gtk_scrolled_window_new();
        window = gtk_application_window_new(app);
    }

    {
        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), text_block);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

        gtk_window_set_title(GTK_WINDOW(window), "Mysticbook");
        gtk_window_set_child(GTK_WINDOW(window), scrolled_window);
        gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);
        gtk_window_present(GTK_WINDOW(window));
    }
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
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
