#include "../../backend/service/block_service.h"
#include "../pages/mb_block_view_page.h"
#include "./mb_app_window_key_controller.h"
#include "./mb_block_search_window.h"

static gboolean
key_pressed(GtkEventControllerKey* self, guint keyval, guint keycode, GdkModifierType state, gpointer user_data)
{
	MbAppWindow *app_window = MB_APP_WINDOW(user_data);

	gboolean ctrl_n_pressed = (state & GDK_CONTROL_MASK) && keyval == GDK_KEY_n;
	gboolean ctrl_o_pressed = (state & GDK_CONTROL_MASK) && keyval == GDK_KEY_o;

	static GtkWidget *block_search;

	if(ctrl_n_pressed)
	{
		// Create a new document, and open it.
		gint64 new_id = block_service_create_document("Untitled");
		GtkWidget *block_view_page = mb_block_view_page_new(new_id);
		mb_app_window_append_page_to_notebook(app_window, block_view_page);
	}
	else if(ctrl_o_pressed)
	{
		if(block_search == NULL)
		{
			block_search = mb_block_search_window_new(app_window);
			gtk_window_set_hide_on_close(GTK_WINDOW(block_search), TRUE);
		}
		gtk_window_present(GTK_WINDOW(block_search));
		return TRUE;
	}
	return FALSE;
}

GtkEventController* mb_app_window_key_new(MbAppWindow *app_window)
{
	GtkEventController *self = gtk_event_controller_key_new();
	gtk_widget_add_controller(GTK_WIDGET(app_window), self);
	g_signal_connect(self, "key-pressed", G_CALLBACK(key_pressed), app_window);
	return self;
}
