#pragma once
#include <gtk/gtk.h>
#include "./mb_app_window.h"
G_BEGIN_DECLS
#define MB_TYPE_BLOCK_SEARCH_ENTRY mb_block_search_entry_get_type()
G_DECLARE_FINAL_TYPE(MbBlockSearchEntry, mb_block_search_entry, MB, BLOCK_SEARCH_ENTRY, GtkWidget)
GtkWidget* mb_block_search_entry_new(const gchar *content, gint64 id, MbAppWindow *app_window);
G_END_DECLS
