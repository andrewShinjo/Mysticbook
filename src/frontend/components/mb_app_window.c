#include "./mb_app_window.h"
#include "./mb_settings_dialog.h"
#include "./mb_window_left_sidebar.h"
#include "./mb_window_left_sidebar_button.h"
#include "../mb_notebook.h"
#include "../pages/mb_block_view_page.h"
#include "../pages/mb_documents_page.h"
#include "../../backend/block.h"
/* WIDGET DEFINITION */
struct _MbAppWindow
{
  GtkApplicationWindow parent;
  /* WIDGETS */
  GtkWidget *left_sidebar;
	GtkWidget *notebook;
  GtkWidget *active_page;
	GtkWidget *settings_dialog;
  GtkWidget *horizontal_box;
  /** Left sidebar buttons **/
  GtkWidget *home_button;
  GtkWidget *settings_button;
  /* EVENT LISTENERS */
  /* PROPERTIES */
};
G_DEFINE_TYPE(MbAppWindow, mb_app_window, GTK_TYPE_APPLICATION_WINDOW)
/* FORWARD DECLARATION */
static void open_document_cb(MbDocumentsPage *documents_page, gpointer user_data);
/* CALLBACK */
static void home_button_clicked(MbWindowLeftSidebarButton *_button, gpointer user_data)
{
  MbAppWindow *_self = MB_APP_WINDOW(user_data);
  gtk_widget_unparent(_self->active_page);
  _self->active_page = mb_documents_page_new();
  gtk_box_append(GTK_BOX(_self->horizontal_box), _self->active_page);
  gtk_widget_set_hexpand(_self->active_page, TRUE);
  g_signal_connect(_self->active_page, "open_doc", G_CALLBACK(open_document_cb), _self);
}
static void settings_button_clicked(MbWindowLeftSidebarButton *_button, gpointer user_data)
{
	MbAppWindow *self = MB_APP_WINDOW(user_data);
	gtk_window_present(GTK_WINDOW(self->settings_dialog));
  g_print("Settings button clicked.\n");
}
static void open_document_cb(MbDocumentsPage *documents_page, gpointer user_data)
{
  GtkWidget *self = GTK_WIDGET(user_data);
  MbAppWindow *_self = MB_APP_WINDOW(user_data);
  gint64 id;
  g_object_get(G_OBJECT(documents_page), "id", &id, NULL);
  gtk_widget_unparent(_self->active_page);
  _self->active_page = mb_block_view_page_new(id);
  gtk_box_append(GTK_BOX(_self->horizontal_box), _self->active_page);
  gtk_widget_set_hexpand(_self->active_page, TRUE);
  gtk_widget_allocate(_self->active_page, 0, 0, 0, NULL);
}
/* PROPERTIES */
/* SIGNALS */
/* WIDGET LIFEYCLE */
static void mb_app_window_init(MbAppWindow *self)
{
  /* INSTANTIATE WIDGETS */
  self->left_sidebar = mb_window_left_sidebar_new();
	self->notebook = mb_notebook_new();
  self->active_page = mb_documents_page_new();
	self->settings_dialog = mb_settings_dialog_new();
  self->home_button = mb_window_left_sidebar_button_new("./resources/light_home.svg", "Go home");
  self->settings_button = mb_window_left_sidebar_button_new("./resources/settings.svg", "Settings");
  self->horizontal_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  /* CONFIGURE WIDGETS */
	/** Settings dialog **/
	gtk_window_set_hide_on_close(GTK_WINDOW(self->settings_dialog), TRUE);

  GtkBox *_horizontal_box = GTK_BOX(self->horizontal_box);
  MbWindowLeftSidebar *_left_sidebar = MB_WINDOW_LEFT_SIDEBAR(self->left_sidebar);
  MbWindowLeftSidebarButton *_home_button = MB_WINDOW_LEFT_SIDEBAR_BUTTON(self->home_button);
  MbWindowLeftSidebarButton *_settings_button = MB_WINDOW_LEFT_SIDEBAR_BUTTON(self->settings_button);
  GtkWindow *window = GTK_WINDOW(self);
  mb_window_left_sidebar_append_button(_left_sidebar, _home_button);
  mb_window_left_sidebar_append_button(_left_sidebar, _settings_button);
  gtk_box_append(_horizontal_box, self->left_sidebar);
  gtk_box_append(_horizontal_box, self->notebook);
  gtk_widget_set_hexpand(self->active_page, TRUE);
  gtk_window_set_child(window, self->horizontal_box);
  /* CONNECT TO SIGNALS */
  g_signal_connect(self->active_page, "open_doc", G_CALLBACK(open_document_cb), self);
  g_signal_connect(self->home_button, "clicked", G_CALLBACK(home_button_clicked), self);
  g_signal_connect(self->settings_button, "clicked", G_CALLBACK(settings_button_clicked), self);
}
static void mb_app_window_class_init(MbAppWindowClass *klass) {}
/* PUBLIC IMPLEMENTATION */
void mb_app_window_change_page(MbAppWindow *_self, GtkWidget *page)
{
  _self->active_page = page;
}
GtkWidget* mb_app_window_new(GtkApplication *application)
{
	return GTK_WIDGET(g_object_new(MB_TYPE_APP_WINDOW, "application", application, NULL));
}
void mb_app_window_open_block(MbAppWindow *_self, gint64 id)
{
  gtk_widget_unparent(_self->active_page);
  _self->active_page = mb_block_view_page_new(id);
  gtk_box_append(GTK_BOX(_self->horizontal_box), _self->active_page);
  gtk_widget_set_hexpand(_self->active_page, TRUE);
  gtk_widget_allocate(_self->active_page, 0, 0, 0, NULL);
}
/* PRIVATE IMPLEMENTATION */
