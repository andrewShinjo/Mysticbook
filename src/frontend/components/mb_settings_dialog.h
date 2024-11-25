#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_SETTINGS_DIALOG mb_settings_dialog_get_type()

G_DECLARE_FINAL_TYPE(MbSettingsDialog, mb_settings_dialog, MB, SETTINGS_DIALOG, GtkWindow)

GtkWidget *mb_settings_dialog_new();

G_END_DECLS
