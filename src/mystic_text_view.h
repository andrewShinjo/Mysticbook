#pragma once
#include<gtk/gtk.h>

G_BEGIN_DECLS

#define MYSTIC_TYPE_TEXT_VIEW mystic_text_view_get_type()

G_DECLARE_FINAL_TYPE(
	MysticTextView, 
	mystic_text_view, 
	MYSTIC, 
	TEXT_VIEW, 
	GtkTextView)

GtkWidget * mystic_text_view_new();
GtkWidget * mystic_text_view_new_with_file(GFile *file);
void mystic_text_view_save_file(MysticTextView *tv);

G_END_DECLS
