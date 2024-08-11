#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MYSTICBOOK_TYPE_APPLICATION_WINDOW mysticbook_application_window_get_type()

G_DECLARE_FINAL_TYPE(
    MysticbookApplicationWindow,
    mysticbook_application_window,
    MYSTICBOOK,
    APPLICATION_WINDOW,
    GtkApplicationWindow
)

GtkWidget *
mysticbook_application_window_new(GtkApplication *application);

G_END_DECLS