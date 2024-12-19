#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_TEXT_VIEW2 mb_text_view2_get_type()

G_DECLARE_FINAL_TYPE(MbTextView2, mb_text_view2, MB, TEXT_VIEW2, GtkTextView)

GtkWidget* mb_text_view2_new();

G_END_DECLS
