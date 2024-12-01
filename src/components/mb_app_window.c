#include "./gfile_list_view.h"
#include "./mb_app_window.h"
#include "./mb_file_browser.h"
#include "./mb_text_view.h"

/* Private definition */

static void open_gfile_in_text_view(MbAppWindow *self);

/* Widget definition */

struct _MbAppWindow
{
  GtkApplicationWindow parent;
  /* WIDGETS */
	GtkWidget *container;
	GtkWidget *file_browser;
	GtkWidget *gfile_list_view;
	GtkWidget *text_view;
};
G_DEFINE_TYPE(MbAppWindow, mb_app_window, GTK_TYPE_APPLICATION_WINDOW)

/* PROPERTIES */

/* SIGNALS */

static void mb_app_window_init(MbAppWindow *self)
{
	self->container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	self->file_browser = mb_file_browser_new(self);
	self->text_view = mb_text_view_new();
	
	gtk_box_append(GTK_BOX(self->container), self->file_browser);
	gtk_box_append(GTK_BOX(self->container), self->text_view);

	gtk_widget_set_hexpand(self->text_view, TRUE);
	gtk_widget_set_vexpand(self->text_view, TRUE);

	gtk_window_set_child(GTK_WINDOW(self), self->container);
}

static void mb_app_window_class_init(MbAppWindowClass *klass) {}

/* PUBLIC IMPLEMENTATION */

GtkWidget* mb_app_window_new(GtkApplication *application)
{
	return GTK_WIDGET(g_object_new(MB_TYPE_APP_WINDOW, "application", application, NULL));
}

void mb_app_window_open_gfile(MbAppWindow *self, GFile *file)
{
	mb_text_view_set_gfile(MB_TEXT_VIEW(self->text_view), file);
}

/* PRIVATE IMPLEMENTATION */

