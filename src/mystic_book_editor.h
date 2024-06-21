#pragma once
#include<gtk/gtk.h>

G_BEGIN_DECLS

#define MYSTIC_BOOK_TYPE_EDITOR mystic_book_editor_get_type()

G_DECLARE_FINAL_TYPE(
	MysticBookEditor, 
	mystic_book_editor, 
	MYSTIC_BOOK, 
	EDITOR, 
	GtkWidget
)

GtkWidget * mystic_book_editor_new();

G_END_DECLS
