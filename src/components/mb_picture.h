#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_PICTURE mb_picture_get_type()

G_DECLARE_FINAL_TYPE(MbPicture, mb_picture, MB, PICTURE, GtkWidget)

GtkWidget* mb_picture_new(const gchar *picture_path);

void mb_picture_set_text_view(MbPicture *self, GtkTextView *text_view);

G_END_DECLS
