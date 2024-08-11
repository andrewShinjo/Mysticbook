#include "mysticbook_application_window.h"

struct _MysticbookApplicationWindow
{
    GtkApplicationWindow parent;
    GtkWidget *scrolled_window;
    GtkWidget *vertical_box;
};

G_DEFINE_TYPE(
    MysticbookApplicationWindow,
    mysticbook_application_window,
    GTK_TYPE_APPLICATION_WINDOW
)

static void
mysticbook_application_window_init(MysticbookApplicationWindow *self)
{
    GtkWindow *window = GTK_WINDOW(self);
    self->scrolled_window = gtk_scrolled_window_new();
    self->vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // GtkWidget *text_view = gtk_text_view_new();
    // GtkWidget *namespace_expander = gtk_expander_new("Namespace");
    // GtkWidget *explicit_relationship_expander = gtk_expander_new("Explicit Relationship(s)");
    // GtkWidget *implicit_relationship_expander = gtk_expander_new("Implicit Relationship(s)");

    // gtk_widget_allocate(text_view, 0, 0, 0, NULL);

    // gtk_box_append(GTK_BOX(self->vertical_box), text_view);
    // gtk_box_append(GTK_BOX(self->vertical_box), namespace_expander);
    // gtk_box_append(GTK_BOX(self->vertical_box), explicit_relationship_expander);
    // gtk_box_append(GTK_BOX(self->vertical_box), implicit_relationship_expander);

    // gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(self->scrolled_window), self->vertical_box);

    gtk_window_set_child(window, self->scrolled_window);
    gtk_window_set_title(window, "Mysticbook");
	gtk_window_maximize(window);
	gtk_window_present(window);
}

static void
mysticbook_application_window_class_init(MysticbookApplicationWindowClass *klass)
{

}

GtkWidget *
mysticbook_application_window_new(GtkApplication *application)
{
    return g_object_new(
        MYSTICBOOK_TYPE_APPLICATION_WINDOW, 
        "application", 
        application, 
        NULL
    );
}