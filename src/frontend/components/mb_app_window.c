#include "./mb_app_window.h"
#include "../pages/mb_block_view_page.h"
#include "../pages/mb_documents_page.h"
#include "../../backend/block.h"

struct _MbAppWindow
{
  GtkApplicationWindow parent;
  GtkWidget *active_page;
};

G_DEFINE_TYPE(MbAppWindow, mb_app_window, GTK_TYPE_APPLICATION_WINDOW)

/* Callback */

static void open_doc_cb(MbDocumentsPage *doc_page, gpointer user_data)
{
  GtkWidget *self = GTK_WIDGET(user_data);
  MbAppWindow *_self = MB_APP_WINDOW(user_data);
  g_print("mb_app_window: open_doc_cb\n");
  gint64 id;
  g_object_get(G_OBJECT(doc_page), "id", id, NULL);
  gtk_widget_unparent(_self->active_page);
  _self->active_page = mb_block_view_page_new(id);
  gtk_window_set_child(GTK_WINDOW(self), _self->active_page);
  gtk_widget_allocate(_self->active_page, 0, 0, 0, NULL);
}

static void mb_app_window_init(MbAppWindow *self)
{
  GtkWindow *window = GTK_WINDOW(self);
  self->active_page = mb_documents_page_new();
  gtk_window_set_child(window, self->active_page);

  g_signal_connect(
    self->active_page,
    "open_doc",
    G_CALLBACK(open_doc_cb),
    self
  );
}

static void
mb_app_window_class_init(MbAppWindowClass *klass) 
{

}

void
mb_app_window_change_page(MbAppWindow *_self, GtkWidget *page)
{
  _self->active_page = page;
}

GtkWidget* mb_app_window_new(GtkApplication *application)
{
	return GTK_WIDGET(g_object_new(
      MB_TYPE_APP_WINDOW, 
      "application", 
      application, 
      NULL
    ));
}
