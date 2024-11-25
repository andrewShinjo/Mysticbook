#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_DOCUMENT_LIST_ROW mb_document_list_row_get_type()

G_DECLARE_FINAL_TYPE(MbDocumentListRow, mb_document_list_row, MB, DOCUMENT_LIST_ROW, GtkWidget)

GtkWidget* mb_document_list_row_new();
G_END_DECLS
