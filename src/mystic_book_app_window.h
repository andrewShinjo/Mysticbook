#pragma once

#include <gtk/gtk.h>

#define MYSTIC_BOOK_TYPE_APP_WINDOW mystic_book_app_window_get_type()

G_DECLARE_FINAL_TYPE(
	MysticBookAppWindow,
	mystic_book_app_window,
	MYSTIC_BOOK,
	APP_WINDOW,
	GtkApplicationWindow
)

GtkWidget *
mystic_book_app_window_new(GtkApplication *gtk_app);
