#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEXT_TYPE_BLOCK text_block_get_type()

G_DECLARE_FINAL_TYPE (
    TextBlock, 
    text_block, 
    TEXT, 
    BLOCK, 
    GtkWidget
)

GtkWidget* text_block_new(void);
void text_block_set_default_font_size(TextBlock *text_block, double size);

G_END_DECLS
