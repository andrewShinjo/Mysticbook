#include "./file_service.h"

gboolean file_service_create_file(gchar *filename)
{
	GFile *file = g_file_new_for_path(filename);
	GError *error = NULL;
	GFileOutputStream *output_stream = g_file_create(file, G_FILE_CREATE_NONE, NULL, &error);

	if(error != NULL)
	{
		g_print("Error creating file: %s\n", error->message);
		g_clear_error(&error);
		return FALSE;
	}

	g_output_stream_close(G_OUTPUT_STREAM(output_stream), NULL, &error);

	if(error != NULL)
	{
		g_print("Error closing file: %s\n", error->message);
		g_clear_error(&error);
		return FALSE;
	}

	g_object_unref(output_stream);
	g_object_unref(file);
	return TRUE;
}
