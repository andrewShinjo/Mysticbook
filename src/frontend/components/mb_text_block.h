#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define MB_TYPE_TEXT_BLOCK mb_text_block_get_type()
G_DECLARE_FINAL_TYPE(MbTextBlock, mb_text_block, MB, TEXT_BLOCK, GtkWidget)
G_END_DECLS

// Public

GtkWidget *mb_text_block_new();
void mb_text_block_grab_focus(MbTextBlock *self);
void mb_text_block_add_child(MbTextBlock *self, GtkWidget *child);