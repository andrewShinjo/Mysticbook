#include <time.h>
#include "./file_service.h"

/* Private declaration */

/* Public implementation */

GFile* file_service_create_file(gchar *filename)
{
	gchar *path = g_strconcat("build/output/", filename, NULL);
	GFile *file = g_file_new_for_path(path);
	g_file_replace_contents(file, "", 0, NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL, NULL);
	g_free(path);
	return file;
}

gchar* file_service_create_unique_filename()
{
	time_t current_time;
	time(&current_time);
	gchar *timestamp = g_strdup_printf("%" G_GINT64_FORMAT, current_time);
	return timestamp;
}

void file_service_update_file(GFile *file, gchar *text, gsize length)
{
	GFileOutputStream *output_stream = g_file_replace(file, NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL);
	g_output_stream_write(G_OUTPUT_STREAM(output_stream), text, length, NULL, NULL);
	g_object_unref(output_stream);
}

/* Private implementation */
