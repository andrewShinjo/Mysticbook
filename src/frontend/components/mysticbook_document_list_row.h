#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MYSTICBOOK_TYPE_DOCUMENT_LIST_ROW mysticbook_application_list_row_get_type()

G_DECLARE_FINAL_TYPE(
    MysticbookDocumentListRow,
    mysticbook_document_list_row,
    
)

G_END_DECLS