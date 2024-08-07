#pragma once
#include<gtk/gtk.h>

G_BEGIN_DECLS

#define MYSTIC_TYPE_TEXT_VIEW mystic_text_view_get_type()
G_DECLARE_FINAL_TYPE(MysticTextView, mystic_text_view, MYSTIC, TEXT_VIEW, GtkTextView)

GtkWidget * mystic_text_view_new();

// Additional functions

void    mystic_text_view_set_file(MysticTextView *self, GFile *file);
GFile * mystic_text_view_get_file(MysticTextView *self);

void    mystic_text_view_open_file(MysticTextView *self);

G_END_DECLS
