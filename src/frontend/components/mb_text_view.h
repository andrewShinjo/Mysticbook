#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define MB_TYPE_TEXT_VIEW mb_text_view_get_type()
G_DECLARE_FINAL_TYPE(MbTextView, mb_text_view, MB, TEXT_VIEW, GtkWidget)
void mb_text_view_append_content(MbTextView *_self, gchar *content);
void mb_text_view_force_redraw_cursor(MbTextView *_self);
gchar* mb_text_view_get_content(MbTextView *_self);
GtkWidget* mb_text_view_get_text_view(MbTextView *_self);
gboolean mb_text_view_is_all_text_highlighted(MbTextView *_self);
gboolean mb_text_view_is_empty(MbTextView *_self);
gboolean mb_text_view_is_insert_at_start(MbTextView *_self);
GtkWidget* mb_text_view_new();
G_END_DECLS
