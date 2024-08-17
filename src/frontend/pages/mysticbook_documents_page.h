#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MYSTICBOOK_TYPE_DOCUMENTS_PAGE mysticbook_documents_page_get_type()

G_DECLARE_FINAL_TYPE(
    MysticbookDocumentsPage,
    mysticbook_documents_page,
    MYSTICBOOK,
    DOCUMENTS_PAGE,
    GtkWidget
)

GtkWidget *
mysticbook_documents_page_new();

G_END_DECLS