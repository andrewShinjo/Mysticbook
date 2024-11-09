#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define MB_TYPE_WINDOW_LEFT_SIDEBAR_BUTTON mb_window_left_sidebar_button_get_type()
G_DECLARE_FINAL_TYPE(MbWindowLeftSidebarButton, mb_window_left_sidebar_button, MB, WINDOW_LEFT_SIDEBAR_BUTTON, GtkWidget)
GtkWidget* mb_window_left_sidebar_button_new();
G_END_DECLS
