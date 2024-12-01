#pragma once
#include <gtk/gtk.h>
#include "./mb_app_window.h"

/* Public declaration */

void gfile_list_view_insert_gfile(GtkListView *self, GFile *file);

GtkWidget* gfile_list_view_new(MbAppWindow *app_window);
