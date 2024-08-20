#include "./mysticbook_document_list_row.h"

struct _MysticbookDocumentListRow
{
	GtkWidget parent;
	GtkWidget *id_label;
	GtkWidget *creation_time_label;
	GtkWidget *modification_time_label;
	GtkWidget *content_label;

	GtkWidget *open_button;
	GtkWidget *delete_button;
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
{
	GtkWidget *widget = gtk_image_new_from_file ("asset/paper.png");
	gtk_widget_set_size_request(widget, 100, 100);
	GtkWidget *label = gtk_label_new ("Label");
	gtk_widget_set_hexpand (label, TRUE);
	self->open_button = gtk_button_new_with_label ("Open");
	self->delete_button = gtk_button_new_with_label ("Delete");
	gtk_widget_set_parent (widget, GTK_WIDGET(self));
	gtk_widget_set_parent (label, GTK_WIDGET (self));
	gtk_widget_set_parent (self->open_button, GTK_WIDGET (self));
	gtk_widget_set_parent (self->delete_button, GTK_WIDGET (self));
}

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
