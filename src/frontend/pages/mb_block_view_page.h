#pragma once
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MB_TYPE_BLOCK_VIEW_PAGE mb_block_view_page_get_type ()

G_DECLARE_FINAL_TYPE (
	MbBlockViewPage,
	mb_block_view_page,
	MB,
	BLOCK_VIEW_PAGE,
	GtkWidget
)

GtkWidget *
mb_block_view_page_new ();

G_END_DECLS
