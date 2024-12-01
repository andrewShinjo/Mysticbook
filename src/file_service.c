#include "./file_service.h"

GList* file_service_get_all_org_files()
{
	GFile *directory = g_file_new_for_path("./build/output");
	GError *error = NULL;
	GFileEnumerator *enumerator = 
		g_file_enumerate_children(directory, G_FILE_ATTRIBUTE_STANDARD_NAME, G_FILE_QUERY_INFO_NONE, NULL, &error);

	if(error)
	{
		g_printerr("Error enumerating files: %s\n", error->message);
		g_error_free(error);
		exit(EXIT_FAILURE);
	}

	GList *file_list = NULL;
	GFileInfo *info;

	while((info = g_file_enumerator_next_file(enumerator, NULL, &error)) != NULL)
	{
		GFile *file = g_file_enumerator_get_child(enumerator, info);
		file_list = g_list_prepend(file_list, file);
		g_object_unref(info);
	}

	if(error)
	{
		g_printerr("Error iterating files: %s\n", error->message);
		g_error_free(error);
		g_list_free_full(file_list, g_object_unref);
		exit(EXIT_FAILURE);
	}

	g_object_unref(enumerator);
	g_object_unref(directory);
	return file_list;
}
