#pragma once
#include <gtk/gtk.h>

GtkWidget* mb_notebook_new();

int mb_notebook_append_page(GtkNotebook *notebook, GtkWidget *page);

void mb_notebook_replace_page_content(GtkNotebook *notebook, GtkWidget *page);
