#pragma once
#include <gtk/gtk.h>
#include "./mb_app_window.h"
G_BEGIN_DECLS
#define MB_TYPE_BLOCK_SEARCH mb_block_search_get_type()
G_DECLARE_FINAL_TYPE(MbBlockSearch, mb_block_search, MB, BLOCK_SEARCH, GtkWidget)
GtkWidget* mb_block_search_new(MbAppWindow *app_window);
GtkWidget* mb_block_search_open(MbAppWindow *app_window);
G_END_DECLS
