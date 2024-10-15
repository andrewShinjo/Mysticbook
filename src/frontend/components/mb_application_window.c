#include "./mb_application_window.h"
#include "../pages/mb_block_view_page.h"
#include "../pages/mb_documents_page.h"

struct _MbApplicationWindow
{
  GtkApplicationWindow parent;
  GtkWidget *active_page;
};

G_DEFINE_TYPE(
  MbApplicationWindow, 
  mb_application_window, 
  GTK_TYPE_APPLICATION_WINDOW
)

static void mb_application_window_init(MbApplicationWindow *self)
{
  GtkWindow *window = GTK_WINDOW(self);
  self->active_page = mb_documents_page_new();
  gtk_window_set_child(window, self->active_page);
}

static void
mb_application_window_class_init(MbApplicationWindowClass *klass) 
{

}

GtkWidget* mb_application_window_new(GtkApplication *application)
{
	return GTK_WIDGET(g_object_new(
      MB_TYPE_APPLICATION_WINDOW, 
      "application", 
      application, 
      NULL
    ));
}
