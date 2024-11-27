#pragma once
#include <gtk/gtk.h>
#include "./mb_app_window.h"
G_BEGIN_DECLS
#define MB_TYPE_BLOCK_SEARCH_WINDOW mb_block_search_window_get_type()
G_DECLARE_FINAL_TYPE(MbBlockSearchWindow, mb_block_search_window, MB, BLOCK_SEARCH_WINDOW, GtkWindow)
GtkWidget* mb_block_search_window_new(MbAppWindow *app_window);
G_END_DECLS
