#pragma once
#include <gtk/gtk.h>
#include "mb_text_block.h"
G_BEGIN_DECLS
#define MB_TYPE_ROOT_TEXT_BLOCK mb_root_text_block_get_type()
G_DECLARE_FINAL_TYPE(MbRootTextBlock, mb_root_text_block, MB, ROOT_TEXT_BLOCK, GtkWidget)
G_END_DECLS

// Public

void
mb_root_text_block_insert_child_after(
  MbRootTextBlock *self,
  MbTextBlock *child,
  MbTextBlock *sibling
);

GtkWidget *
mb_root_text_block_new();

void 
mb_root_text_block_grab_focus();

void
mb_root_text_block_remove_child(MbRootTextBlock *self, MbTextBlock *child);
