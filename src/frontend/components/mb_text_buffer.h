#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define MB_TYPE_TEXT_BUFFER mb_text_buffer_get_type()
G_DECLARE_FINAL_TYPE(MbTextBuffer, mb_text_buffer, MB, TEXT_BUFFER, GtkTextBuffer)
GtkTextBuffer* mb_text_buffer_new(gint64 id);
G_END_DECLS
