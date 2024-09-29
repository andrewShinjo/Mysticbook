#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define MB_TYPE_ROOT_TEXT_BLOCK mb_root_text_block_get_type()
G_DECLARE_FINAL_TYPE(MbRootTextBlock, mb_root_text_block, MB, ROOT_TEXT_BLOCK, GtkWidget)
G_END_DECLS

// Public

GtkWidget *mb_root_text_block_new();
void mb_root_text_block_grab_focus();
