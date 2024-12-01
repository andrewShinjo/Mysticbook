#pragma once
#include <gio/gio.h>
#include <glib.h>

/* Public declaration */

GFile* file_service_create_file(gchar *filename);

gchar* file_service_create_unique_filename();

void file_service_update_file(GFile *file, gchar *text, gsize length);
