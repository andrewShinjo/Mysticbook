#include "./mysticbook_document_list_row.h"

struct _MysticbookDocumentListRow
{
	GtkWidget parent;
};

G_DEFINE_TYPE (
	MysticbookDocumentListRow,
	mysticbook_document_list_row,
	GTK_TYPE_WIDGET
)

// **********************************************************************
// * Widget lifecycle
// **********************************************************************

static void
mysticbook_document_list_row_dispose (GObject *object);

static void
mysticbook_document_list_row_finalize (GObject *object);

static void
mysticbook_document_list_row_init (MysticbookDocumentListRow *self)
{}

static void
mysticbook_document_list_row_class_init (
	MysticbookDocumentListRowClass *klass
)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	object_class->dispose = mysticbook_document_list_row_dispose;
	object_class->finalize = mysticbook_document_list_row_finalize;
	gtk_widget_class_set_layout_manager_type (
		GTK_WIDGET_CLASS (klass),
		GTK_TYPE_BOX_LAYOUT
	);
}

static void
mysticbook_document_list_row_dispose (GObject *object)
{
	MysticbookDocumentListRow *self = MYSTICBOOK_DOCUMENT_LIST_ROW (object);
	G_OBJECT_CLASS (
		mysticbook_document_list_row_parent_class
	)->dispose (object);
}

static void
mysticbook_document_list_row_finalize (GObject *object)
{
	G_OBJECT_CLASS (mysticbook_document_list_row_parent_class)
		->finalize (object);
}

// **********************************************************************
// * Public
// **********************************************************************

GtkWidget *
mysticbook_document_list_row_new ()
{
	return g_object_new (MYSTICBOOK_TYPE_DOCUMENT_LIST_ROW, NULL);
}
