// Author: Andrew Shinjo
// Date: 05-29-2024

#include <gtk/gtk.h>

static void
activate(
    GtkApplication *app,
    gpointer user_data
)
{
    GtkSettings *settings = gtk_settings_get_default();
    gchar *font_family;
    gint font_size;

    GtkTextBuffer *buffer;
    GtkTextTag *tag;
    GtkWidget *window;
    GtkWidget *text_view;

    // Instantiate widgets
    {
        text_view = gtk_text_view_new();
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        window = gtk_application_window_new(app);
        tag = gtk_text_buffer_create_tag(buffer, "bold_tag", "weight", PANGO_WEIGHT_BOLD, NULL);
    }

    {
        gtk_window_set_title(GTK_WINDOW(window), "Mysticbook");
        gtk_window_set_child(GTK_WINDOW(window), text_view);
        gtk_window_set_default_size(GTK_WINDOW(window),900, 600);
        gtk_window_present(GTK_WINDOW(window));
    }

    {
        g_object_set(settings, "gtk-font-name", "Ubuntu 24", NULL);
        g_object_get(settings, "gtk-font-name", &font_family, NULL);
        g_print("Default Font Family: %s\n", font_family);
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
