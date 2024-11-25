#include "./mb_app_window_key_controller.h"

static gboolean
key_pressed(GtkEventControllerKey* self, guint keyval, guint keycode, GdkModifierType state, gpointer user_data)
{
	gboolean ctrl_o_pressed = (state & GDK_CONTROL_MASK) && keyval == GDK_KEY_o;

	if(ctrl_o_pressed)
	{
		g_print("Ctrl+o pressed.\n");
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
