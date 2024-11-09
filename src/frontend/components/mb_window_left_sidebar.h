#pragma once
#include <gtk/gtk.h>
#include "./mb_window_left_sidebar_button.h"
G_BEGIN_DECLS
#define MB_TYPE_WINDOW_LEFT_SIDEBAR mb_window_left_sidebar_get_type()
G_DECLARE_FINAL_TYPE(MbWindowLeftSidebar, mb_window_left_sidebar, MB, WINDOW_LEFT_SIDEBAR, GtkWidget)
void mb_window_left_sidebar_append_button(MbWindowLeftSidebar *_self, MbWindowLeftSidebarButton *_button);
GtkWidget* mb_window_left_sidebar_new();
G_END_DECLS
