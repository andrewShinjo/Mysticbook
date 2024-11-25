#include "./mb_app_window_key_controller.h"

static gboolean
key_pressed(GtkEventControllerKey* self, guint keyval, guint keycode, GdkModifierType state, gpointer user_data)
{
	return TRUE;
}

GtkEventController *mb_app_window_key_new()
{
	GtkEventController *key = gtk_event_controller_key_new();
	return key;
}
