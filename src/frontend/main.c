#include <gtk/gtk.h>

static void on_open_file_complete(GObject *source, GAsyncResult *result, gpointer data)
{
	g_print("on_open_file_complete\n");
	GtkFileDialog *file_dialog = GTK_FILE_DIALOG(source);
	GtkTextView *textview = GTK_TEXT_VIEW(data);
	GtkTextBuffer *textbuffer = gtk_text_view_get_buffer(textview);
	GFile *opened_file = gtk_file_dialog_open_finish(file_dialog, result, NULL);

	if(opened_file)
	{
		char *contents;
		gsize length;
		GError *err = NULL;

		if(g_file_load_contents(opened_file, NULL, &contents, &length, NULL, &err))
		{
			g_print("File contents: %s\n", contents);
			gtk_text_buffer_set_text(textbuffer, contents, -1);
		}
		g_object_unref(opened_file);
	}
	else
	{
		g_print("No file selected or operation cancelled.\n");
	}
}

static void on_new_file_activate(GSimpleAction *action, GVariant *parameter, GApplication *app)
{
	g_print("on_new_file_activate\n");
}

static void on_open_file_activate(GSimpleAction *action, GVariant *parameter, GApplication *app)
{
	GtkApplication *gtk_app = GTK_APPLICATION(app);
	GtkWindow *active_window = gtk_application_get_active_window(gtk_app);
	GtkWidget *scrolled_window = gtk_window_get_child(active_window);
	GtkWidget *textview = gtk_scrolled_window_get_child(GTK_SCROLLED_WINDOW(scrolled_window));
	GtkFileDialog *file_dialog = gtk_file_dialog_new();
	gtk_file_dialog_open(file_dialog, active_window, NULL, on_open_file_complete, textview);
}

static void on_save_file_activate(GSimpleAction *action, GVariant *parameter, GApplication *app)
{
	g_print("on_save_file_activate\n");	
}

static void on_quit_activate(GSimpleAction *action, GVariant *parameter, GApplication *app)
{
	g_application_quit(app);
}

static void app_startup(GApplication *application) 
{
	GtkApplication *app = GTK_APPLICATION(application);

	// Define actions.
	GSimpleAction *act_new_file =  g_simple_action_new("new-file",  NULL);
	GSimpleAction *act_open_file = g_simple_action_new("open-file", NULL);
	GSimpleAction *act_save_file = g_simple_action_new("save-file", NULL);
	GSimpleAction *act_quit =      g_simple_action_new("quit",      NULL);
	g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(act_new_file));
	g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(act_open_file));
	g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(act_save_file));
	g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(act_quit));
	g_signal_connect(act_new_file,  "activate", G_CALLBACK(on_new_file_activate),  app);
	g_signal_connect(act_open_file, "activate", G_CALLBACK(on_open_file_activate), app);
	g_signal_connect(act_save_file, "activate", G_CALLBACK(on_save_file_activate), app);
	g_signal_connect(act_quit,      "activate", G_CALLBACK(on_quit_activate),      app);

	GMenu *menubar = g_menu_new();

	// Add "File" to menu bar.
	GMenuItem *file_menu_item = g_menu_item_new("File", NULL);
	GMenu *file_menu = g_menu_new();
	g_menu_item_set_submenu(file_menu_item, G_MENU_MODEL(file_menu));
	g_menu_append_item(menubar, file_menu_item);
	g_object_unref(file_menu);
	g_object_unref(file_menu_item);

	// Add menu items to "File"
	GMenuItem *file_menu_item_new_file =  g_menu_item_new("New File", "app.new-file");
	GMenuItem *file_menu_item_open_file = g_menu_item_new("Open File", "app.open-file");
	GMenuItem *file_menu_item_save_file = g_menu_item_new("Save File", "app.save-file");
	GMenuItem *file_menu_item_quit =      g_menu_item_new("Quit", "app.quit");
	g_menu_append_item(file_menu, file_menu_item_new_file);
	g_menu_append_item(file_menu, file_menu_item_open_file);
	g_menu_append_item(file_menu, file_menu_item_save_file);
	g_menu_append_item(file_menu, file_menu_item_quit);
	g_object_unref(file_menu_item_new_file);
	g_object_unref(file_menu_item_open_file);
	g_object_unref(file_menu_item_save_file);
	g_object_unref(file_menu_item_quit);

	gtk_application_set_menubar(GTK_APPLICATION(app), G_MENU_MODEL(menubar));
}

static void app_activate(GApplication *app, gpointer *user_data)
{
	GtkWidget *scrolled_window;
	GtkWidget *textview;
	GtkTextBuffer *textbuffer;
	GtkWidget *window;

	textview = gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);

	textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

	scrolled_window = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), textview);

	window = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(window), TRUE);
	gtk_window_maximize(GTK_WINDOW(window));
	gtk_window_present(GTK_WINDOW(window));
	gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "Mysticbook");
	gtk_window_set_child(GTK_WINDOW(window), scrolled_window);
}

int main(int argc, char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new("com.ashinjo.mystic.mysticbook", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "startup", G_CALLBACK(app_startup), NULL);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}
