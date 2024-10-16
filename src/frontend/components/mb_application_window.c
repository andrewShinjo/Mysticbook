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

/* Callback */

static void open_doc_cb(MbDocumentsPage *doc_page, gpointer user_data)
{
  g_print("mb_app_window: open_doc_cb\n");
}

static void mb_application_window_init(MbApplicationWindow *self)
{
  GtkWindow *window = GTK_WINDOW(self);
  self->active_page = mb_documents_page_new();
  gtk_window_set_child(window, self->active_page);

  g_signal_connect(
    self->active_page,
    "open_doc",
    G_CALLBACK(open_doc_cb),
    NULL
  );
}

static void
mb_application_window_class_init(MbApplicationWindowClass *klass) 
{

}

void
mb_app_window_change_page(
  MbApplicationWindow *_self,
  GtkWidget *page)
{
  _self->active_page = page;
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
