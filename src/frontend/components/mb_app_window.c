#include "./mb_app_window.h"
#include "./mb_tree_outline.h"

/* WIDGET DEFINITION */
struct _MbAppWindow
{
  GtkApplicationWindow parent;
  /* WIDGETS */
	GtkWidget *container;
	GtkWidget *tree_outline;
};
G_DEFINE_TYPE(MbAppWindow, mb_app_window, GTK_TYPE_APPLICATION_WINDOW)
/* FORWARD DECLARATION */
/* CALLBACK */
/* PROPERTIES */
/* SIGNALS */
/* WIDGET LIFEYCLE */
static void mb_app_window_init(MbAppWindow *self)
{
	self->tree_outline = mb_tree_outline_new();
	gtk_window_set_child(GTK_WINDOW(self), self->tree_outline);
}
static void mb_app_window_class_init(MbAppWindowClass *klass) {}
/* PUBLIC IMPLEMENTATION */
GtkWidget* mb_app_window_new(GtkApplication *application)
{
	return GTK_WIDGET(g_object_new(MB_TYPE_APP_WINDOW, "application", application, NULL));
}
/* PRIVATE IMPLEMENTATION */
