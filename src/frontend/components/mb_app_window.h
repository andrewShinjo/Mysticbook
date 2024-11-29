#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define MB_TYPE_APP_WINDOW mb_app_window_get_type()
G_DECLARE_FINAL_TYPE(MbAppWindow, mb_app_window, MB, APP_WINDOW, GtkApplicationWindow)
GtkWidget* mb_app_window_new(GtkApplication *application);
G_END_DECLS
