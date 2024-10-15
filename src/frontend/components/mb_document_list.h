#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_DOCUMENT_LIST mb_document_list_get_type()

G_DECLARE_FINAL_TYPE(
  MbDocumentList,
  mb_document_list,
  MB,
  DOCUMENT_LIST,
  GtkWidget
)

G_END_DECLS

GtkWidget* mb_document_list_new();
int mb_document_list_add_row(MbDocumentList *self);
int mb_document_list_get_rows(MbDocumentList *self);
