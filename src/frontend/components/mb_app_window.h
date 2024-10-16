#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_APP_WINDOW mb_app_window_get_type()

G_DECLARE_FINAL_TYPE(
  MbAppWindow,
  mb_app_window,
  MB,
  APP_WINDOW,
  GtkApplicationWindow
)

void mb_app_window_change_page(MbAppWindow *_self, GtkWidget *page);
GtkWidget* mb_app_window_new(GtkApplication *application);

G_END_DECLS
