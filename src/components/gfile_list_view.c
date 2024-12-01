#include "./gfile_list_view.h"

/* Private declaration */

static void bind(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data);

static GtkListItemFactory* create_factory();

static void populate_store(GListStore *store);

static void setup(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data);

/* Public implementation */

GtkWidget* gfile_list_view_new()
{
	GListStore *store = g_list_store_new(G_TYPE_FILE);
	GtkSelectionModel *selection = GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(store)));
	GtkListItemFactory *factory = create_factory();
	GtkWidget *list_view = gtk_list_view_new(selection, factory);
	populate_store(store);
	return list_view;
}

/* Private implementation */

static void bind(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GFile *file = gtk_list_item_get_item(list_item);
	GtkWidget *child = gtk_list_item_get_child(list_item);
	gtk_label_set_text(GTK_LABEL(child), g_file_get_basename(file));
}

static GtkListItemFactory* create_factory()
{
	GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "bind", G_CALLBACK(bind), NULL);
	g_signal_connect(factory, "setup", G_CALLBACK(setup), NULL);
	return factory;
}

static void populate_store(GListStore *store)
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

	GFileInfo *info;

	while((info = g_file_enumerator_next_file(enumerator, NULL, &error)) != NULL)
	{
		g_autoptr(GFile) file = g_file_enumerator_get_child(enumerator, info);
		g_list_store_append(store, file);
	}

	if(error)
	{
		g_printerr("Error iterating files: %s\n", error->message);
		g_error_free(error);
		exit(EXIT_FAILURE);
	}

	g_object_unref(enumerator);
	g_object_unref(directory);
}

static void setup(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_label_new("");
	gtk_list_item_set_child(list_item, label);
}

