#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_DOCUMENTS_PAGE mb_documents_page_get_type()

G_DECLARE_FINAL_TYPE(
  MbDocumentsPage,
  mb_documents_page,
  MB,
  DOCUMENTS_PAGE,
  GtkWidget
)

GtkWidget* mb_documents_page_new();

G_END_DECLS
