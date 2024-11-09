#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define MB_TYPE_WINDOW_LEFT_SIDEBAR mb_window_left_sidebar_get_type()
G_DECLARE_FINAL_TYPE(MbWindowLeftSidebar, mb_window_left_sidebar, MB, WINDOW_LEFT_SIDEBAR, GtkWidget)
GtkWidget* mb_window_left_sidebar_new();
G_END_DECLS
