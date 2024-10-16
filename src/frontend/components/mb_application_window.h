#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_APPLICATION_WINDOW mb_application_window_get_type()

G_DECLARE_FINAL_TYPE(
  MbApplicationWindow,
  mb_application_window,
  MB,
  APPLICATION_WINDOW,
  GtkApplicationWindow
)

void 
mb_app_window_change_page(
  MbApplicationWindow *_self, 
  GtkWidget *page
);
GtkWidget* mb_application_window_new(GtkApplication *application);
G_END_DECLS
