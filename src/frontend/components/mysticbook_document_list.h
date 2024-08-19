#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MYSTICBOOK_TYPE_DOCUMENT_LIST mysticbook_document_list_get_type()

G_DECLARE_FINAL_TYPE(
    MysticbookDocumentList,
    mysticbook_document_list,
    MYSTICBOOK,
    DOCUMENT_LIST,
    GtkWidget
)

G_END_DECLS

// *********************************************************************
// * Interface
// *********************************************************************

GtkWidget * 
mysticbook_document_list_new ();

int 
mysticbook_document_list_add_row (MysticbookDocumentList *self);

int
mysticbook_document_list_get_rows (MysticbookDocumentList *self);
