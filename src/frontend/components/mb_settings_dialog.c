#include "./mb_settings_dialog.h"
/* Widget definition */
struct _MbSettingsDialog
{
	GtkWindow parent;
	/* Widgets */
	/* Event listeners */
	/* Properties */
};
G_DEFINE_TYPE(MbSettingsDialog, mb_settings_dialog, GTK_TYPE_WINDOW)
/* Forward declaration */
/* Callback */
/* Properties */
/* Signals */
/* Widget lifecycle */
static void mb_settings_dialog_init(MbSettingsDialog *self) 
{
	/* Instantiate widgets */
	/* Configure widgets */
	gtk_window_set_title(GTK_WINDOW(self), "Settings");
	/* Connect to signals */
}	
static void mb_settings_dialog_class_init(MbSettingsDialogClass *klass) {}
static void mb_settings_dialog_dispose(GObject *object) {}
static void mb_settings_dialog_finalize(GObject *object) {}
/* Public implementation */
GtkWidget* mb_settings_dialog_new()
{
	return g_object_new(MB_TYPE_SETTINGS_DIALOG, NULL);
}
/* Private implementation */
