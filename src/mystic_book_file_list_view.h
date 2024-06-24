#pragma once

#include <gtk/gtk.h>

#define MYSTIC_BOOK_TYPE_FILE_LIST_VIEW mystic_book_file_list_view_get_type()

G_DECLARE_FINAL_TYPE(
	MysticBookFileListView,
	mystic_book_file_list_view,
	MYSTIC_BOOK,
	FILE_LIST_VIEW,
	GtkWidget
)

GtkWidget *
mystic_book_file_list_view_new();
