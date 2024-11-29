#include "./mb_app_window.h"
/* WIDGET DEFINITION */
struct _MbAppWindow
{
  GtkApplicationWindow parent;
  /* WIDGETS */
	GtkBox *container;
	GtkBox *left_sidebar;
	GtkPaned *paned;
	GtkFrame *left_frame;
	GtkFrame *right_frame;
};
G_DEFINE_TYPE(MbAppWindow, mb_app_window, GTK_TYPE_APPLICATION_WINDOW)
/* FORWARD DECLARATION */
/* CALLBACK */
/* PROPERTIES */
/* SIGNALS */
/* WIDGET LIFEYCLE */
static void mb_app_window_init(MbAppWindow *self)
{
  /* INSTANTIATE WIDGETS */
	self->container = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
	self->left_sidebar = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
	self->paned = GTK_PANED(gtk_paned_new(GTK_ORIENTATION_HORIZONTAL));
	self->left_frame = GTK_FRAME(gtk_frame_new(NULL));
	self->right_frame = GTK_FRAME(gtk_frame_new(NULL));

	gtk_paned_set_start_child(self->paned, GTK_WIDGET(self->left_frame));
	gtk_paned_set_end_child(self->paned, GTK_WIDGET(self->right_frame));
	gtk_paned_set_position(self->paned, 100);

	gtk_widget_set_hexpand(GTK_WIDGET(self->paned), TRUE);
	gtk_widget_set_vexpand(GTK_WIDGET(self->paned), TRUE);

	gtk_box_append(self->container, GTK_WIDGET(self->left_sidebar));
	gtk_box_append(self->container, GTK_WIDGET(self->paned));

	gtk_widget_set_size_request(GTK_WIDGET(self->left_sidebar), 32, -1);

	gtk_window_set_child(GTK_WINDOW(self), GTK_WIDGET(self->container));
}
static void mb_app_window_class_init(MbAppWindowClass *klass) {}
/* PUBLIC IMPLEMENTATION */
GtkWidget* mb_app_window_new(GtkApplication *application)
{
	return GTK_WIDGET(g_object_new(MB_TYPE_APP_WINDOW, "application", application, NULL));
}
/* PRIVATE IMPLEMENTATION */
