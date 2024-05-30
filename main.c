// Author: Andrew Shinjo
// Date: 05-29-2024

#include <gtk/gtk.h>

gboolean
key_pressed (
  GtkEventControllerKey* self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
)
{
    g_print("key_pressed event\n");
    return GDK_EVENT_STOP;
}

static void
activate(
    GtkApplication *app,
    gpointer user_data
)
{
    GtkTextBuffer *textBuffer;
    GtkWidget *expander;
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *window;
    GtkWidget *textview;

    GtkEventController *keyEventController;

    // Instantiate widgets
    {
        expander = gtk_label_new(" • ");
        hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        textview = gtk_text_view_new();
        window = gtk_application_window_new(app);
        textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

        keyEventController = gtk_event_controller_key_new();
    }

    {

        gtk_widget_add_controller(textview, keyEventController);
        gtk_widget_set_hexpand(textview, true);
        gtk_widget_set_valign(expander, GTK_ALIGN_START);

        gtk_box_append(GTK_BOX(hbox), expander);
        gtk_box_append(GTK_BOX(hbox), textview);
        gtk_box_append(GTK_BOX(vbox), hbox);

        gtk_window_set_title(GTK_WINDOW(window), "Mysticbook");
        gtk_window_set_child(GTK_WINDOW(window), vbox);
        gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);
        gtk_window_present(GTK_WINDOW(window));
    }

    // Signal
    {
        g_signal_connect(
            keyEventController, 
            "key-pressed",
            G_CALLBACK(key_pressed), 
            NULL
        );
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
