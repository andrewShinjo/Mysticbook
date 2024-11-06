#include "./mb_app_window.h"
#include "../pages/mb_block_view_page.h"
#include "../pages/mb_documents_page.h"
#include "../../backend/block.h"
/* WIDGET DEFINITION */
struct _MbAppWindow
{
  GtkApplicationWindow parent;
  /* WIDGETS */
  GtkWidget *active_page;
  GtkWidget *home_button;
  GtkWidget *horizontal_box;
  /* EVENT LISTENERS */
  /* PROPERTIES */
};
G_DEFINE_TYPE(MbAppWindow, mb_app_window, GTK_TYPE_APPLICATION_WINDOW)
/* FORWARD DECLARATION */
/* CALLBACK */
static void open_document_cb(MbDocumentsPage *documents_page, gpointer user_data)
{
  GtkWidget *self = GTK_WIDGET(user_data);
  MbAppWindow *_self = MB_APP_WINDOW(user_data);
  gint64 id;
  g_object_get(G_OBJECT(documents_page), "id", &id, NULL);
  gtk_widget_unparent(_self->active_page);
  _self->active_page = mb_block_view_page_new(id);
  gtk_window_set_child(GTK_WINDOW(self), _self->active_page);
  gtk_widget_allocate(_self->active_page, 0, 0, 0, NULL);
}
/* PROPERTIES */
/* SIGNALS */
/* WIDGET LIFEYCLE */
static void mb_app_window_init(MbAppWindow *self)
{
  /* INSTANTIATE WIDGETS */
  self->active_page = mb_documents_page_new();
  self->home_button = gtk_button_new_with_label("⌂");
  self->horizontal_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  /* CONFIGURE WIDGETS */
  GtkBox *_horizontal_box = GTK_BOX(self->horizontal_box);
  GtkWindow *window = GTK_WINDOW(self);
  gtk_box_append(_horizontal_box, self->home_button);
  gtk_box_append(_horizontal_box, self->active_page);
  gtk_widget_set_hexpand(self->active_page, TRUE);
  gtk_widget_set_valign(self->home_button, GTK_ALIGN_START);
  gtk_window_set_child(window, self->horizontal_box);
  /* CONNECT TO SIGNALS */
  g_signal_connect(self->active_page, "open_doc", G_CALLBACK(open_document_cb), self);
}
static void
mb_app_window_class_init(MbAppWindowClass *klass) {}
/* PUBLIC IMPLEMENTATION */
void mb_app_window_change_page(MbAppWindow *_self, GtkWidget *page)
{
  _self->active_page = page;
}
GtkWidget* mb_app_window_new(GtkApplication *application)
{
	return GTK_WIDGET(g_object_new(MB_TYPE_APP_WINDOW, "application", application, NULL));
}
/* PRIVATE IMPLEMENTATION */
