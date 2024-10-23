#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_XXX mb_xxx_get_type()

G_DECLARE_FINAL_TYPE(
  MbXXX,
  mb_xxx,
  MB,
  XXX,
  GtkWidget
)

/* Public constructor */

GtkWidget*
mb_xxx_new();

/* Public functions */

G_END_DECLS
