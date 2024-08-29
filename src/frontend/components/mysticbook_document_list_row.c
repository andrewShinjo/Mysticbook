#include "./mysticbook_document_list_row.h"

// **********************************************************************
// * Properties
// **********************************************************************

enum
{
	PROP_CONTENT = 1,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

// **********************************************************************
// * GObject Definition
// **********************************************************************

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
// * Properties - Getters and Setters
// **********************************************************************

static void
mysticbook_document_list_row_set_property (
	GObject *object,
	guint property_id,
	const GValue *value,
	GParamSpec *pspec
)
{
	MysticbookDocumentListRow *self = MYSTICBOOK_DOCUMENT_LIST_ROW(object);

	switch (property_id)
	{
		case PROP_CONTENT:
		{
			// TODO(andy): implement this.
			break;
		}
		default:
		{
			/* No valid property */
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);	
			break;
		} 
	}
}

// **********************************************************************
// * Callbacks
// **********************************************************************

static void
delete_button_clicked (GtkButton *self, gpointer user_data)
{
	g_print ("Delete button clicked.\n");
}

static void
open_button_clicked (GtkButton *self, gpointer user_data)
{
	g_print ("Open button clicked.\n");
}

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
	self->content_label = gtk_label_new ("Label");
	gtk_widget_set_halign (self->content_label, GTK_ALIGN_START);
	gtk_widget_set_hexpand (self->content_label, TRUE);
	self->open_button = gtk_button_new_with_label ("Open");
	self->delete_button = gtk_button_new_with_label ("Delete");
	gtk_widget_set_parent (self->content_label, GTK_WIDGET (self));
	gtk_widget_set_parent (self->open_button, GTK_WIDGET (self));
	gtk_widget_set_parent (self->delete_button, GTK_WIDGET (self));

	g_signal_connect (
		self->delete_button,
		"clicked",
		G_CALLBACK (delete_button_clicked),
		NULL
	);
	g_signal_connect (
		self->open_button,
		"clicked",
		G_CALLBACK (open_button_clicked),
		NULL
	);
}

static void
mysticbook_document_list_row_class_init (
	MysticbookDocumentListRowClass *klass
)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GParamFlags default_flags = 
		G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY;
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
	g_clear_pointer (&self->content_label, gtk_widget_unparent);
	g_clear_pointer (&self->open_button, gtk_widget_unparent);
	g_clear_pointer (&self->delete_button, gtk_widget_unparent);
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

