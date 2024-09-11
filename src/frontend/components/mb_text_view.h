#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define MB_TYPE_TEXT_VIEW mb_text_view_get_type()
G_DECLARE_FINAL_TYPE(MbTextView, mb_text_view, MB, TEXT_VIEW, GtkWidget)
G_END_DECLS
GtkWidget *mb_text_view_new();
