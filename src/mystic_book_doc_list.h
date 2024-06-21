#pragma once

#include <gtk/gtk.h>

#define MYSTIC_BOOK_TYPE_DOC_LIST mystic_book_doc_list_get_type()

G_DECLARE_FINAL_TYPE(
	MysticBookDocList,
	mystic_book_doc_list,
	MYSTIC_BOOK,
	DOC_LIST,
	GtkWidget
)

GtkWidget *
mystic_book_doc_list_new();
