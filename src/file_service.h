#pragma once
#include <gio/gio.h>
#include <glib.h>

/* Public declaration */

void file_service_update_file(GFile *file, gchar *text, gsize length);
