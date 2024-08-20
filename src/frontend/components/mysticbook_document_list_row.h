#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MYSTICBOOK_TYPE_DOCUMENT_LIST_ROW mysticbook_document_list_row_get_type()

G_DECLARE_FINAL_TYPE (
	MysticbookDocumentListRow,
	mysticbook_document_list_row,
	MYSTICBOOK,
	DOCUMENT_LIST_ROW,
	GtkWidget
)

G_END_DECLS

// **********************************************************************
// * Interface
// **********************************************************************

GtkWidget *
mysticbook_document_list_row_new ();
