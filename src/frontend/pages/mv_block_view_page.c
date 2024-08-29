#include "./mb_block_view_page.h"

static void
mb_block_view_page_dispose (GObject *object);

static void
mb_block_view_page_finalize (GObject *object);

struct _MbBlockViewPage
{
	GtkWidget parent;
};

G_DEFINE_TYPE (MbBlockViewPage, mb_block_view_page, GTK_TYPE_WIDGET)

// *********************************************************************
// * Widget lifecycle
// *********************************************************************

static void
mb_block_view_page_init (MbBlockViewPage *self) {}

static void
mb_block_view_page_class_init (MbBlockViewPageClass *klass) {}

static void
mb_block_view_page_dispose (GObject *object);

static void
mb_block_view_page_finalize (GObject *object);

// *********************************************************************
// * Public
// *********************************************************************

GtkWidget *
mb_block_view_page_new ()
{
	return g_object_new (MB_TYPE_BLOCK_VIEW_PAGE, NULL);
}
