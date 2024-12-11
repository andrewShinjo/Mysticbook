#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_IMAGE mb_image_get_type()

G_DECLARE_FINAL_TYPE(MbImage, mb_image, MB, IMAGE, GtkWidget)

GtkWidget* mb_image_new(gchar *image_path);

G_END_DECLS
