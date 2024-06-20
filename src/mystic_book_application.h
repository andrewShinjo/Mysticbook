#pragma once

#include<gtk/gtk.h>

#define MYSTIC_BOOK_TYPE_APPLICATION mystic_book_application_get_type()

G_DECLARE_FINAL_TYPE(
	MysticBookApplication, 
	mystic_book_application, 
	MYSTIC_BOOK, APPLICATION, 
	GtkApplication
)

MysticBookApplication *
mystic_book_application_new(
	const char *application_id, 
	GApplicationFlags application_flags
);
