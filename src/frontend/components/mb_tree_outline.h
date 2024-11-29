#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define MB_TYPE_TREE_OUTLINE mb_tree_outline_get_type()
G_DECLARE_FINAL_TYPE(MbTreeOutline, mb_tree_outline, MB, TREE_OUTLINE, GtkWidget)
GtkWidget* mb_tree_outline_new();
G_END_DECLS
