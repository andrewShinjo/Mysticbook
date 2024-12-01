#include "./file_service.h"

void file_service_update_file(GFile *file, gchar *text, gsize length)
{
	GFileOutputStream *output_stream = g_file_replace(file, NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL);
	g_output_stream_write(G_OUTPUT_STREAM(output_stream), text, length, NULL, NULL);
	g_object_unref(output_stream);
}
