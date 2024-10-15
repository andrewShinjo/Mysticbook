#include "./mb_application_window.h"
#include "./mb_document_list_row.h"

enum
{
	PROP_CONTENT = 1,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

struct _MbDocumentListRow
{
	GtkWidget parent;
	GtkWidget *id_label;
	GtkWidget *creation_time_label;
	GtkWidget *modification_time_label;
	GtkWidget *content_label;
	GtkWidget *open_button;
	GtkWidget *delete_button;

	gchar *content;
};

G_DEFINE_TYPE(MbDocumentListRow, mb_document_list_row, GTK_TYPE_WIDGET)

static void
mb_document_list_row_set_property(
	GObject *object,
	guint property_id,
	const GValue *value,
	GParamSpec *pspec
)
{
	MbDocumentListRow *self = MB_DOCUMENT_LIST_ROW(object);

	switch(property_id)
	{
		case PROP_CONTENT:
		{
			g_free(self->content);
			self->content = g_value_dup_string(value);
			gtk_label_set_text(GTK_LABEL(self->content_label), self->content);
			g_print("Content: %s\n", self->content);
			break;
		}
		default:
		{
			/* No valid property */
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);	
			break;
		} 
	}
}

static void
mb_document_list_row_get_property(
	GObject *object,
	guint property_id,
	GValue *value,
	GParamSpec *pspec
)
{
	MbDocumentListRow *self = MB_DOCUMENT_LIST_ROW(object);

	switch (property_id)
	{
		case PROP_CONTENT:
		{
			g_value_set_string(value, self->content);
			gtk_label_set_text(GTK_LABEL(self->content_label), self->content);
			break;
		}
		default:
		{
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
			break;
		}
	}	
}

static void
delete_button_clicked(GtkButton *self, gpointer user_data)
{
	g_print("Delete button clicked.\n");
}

static void
open_button_clicked(GtkButton *button, gpointer user_data)
{
  GtkWidget *self = GTK_WIDGET(user_data);
	g_print("Open button clicked.\n");
  GtkWidget *window = gtk_widget_get_ancestor(
    self, 
    MB_TYPE_APPLICATION_WINDOW
  );
}

static void mb_document_list_row_dispose(GObject *object);
static void mb_document_list_row_finalize(GObject *object);

static void mb_document_list_row_init(MbDocumentListRow *self)
{
	self->content_label = gtk_label_new(NULL);
	gtk_widget_set_halign(self->content_label, GTK_ALIGN_START);
	gtk_widget_set_hexpand(self->content_label, TRUE);
	self->open_button = gtk_button_new_with_label("Open");
	self->delete_button = gtk_button_new_with_label("Delete");
	gtk_widget_set_parent(self->content_label, GTK_WIDGET(self));
	gtk_widget_set_parent(self->open_button, GTK_WIDGET(self));
	gtk_widget_set_parent(self->delete_button, GTK_WIDGET(self));

	g_signal_connect(
		self->delete_button,
		"clicked",
		G_CALLBACK(delete_button_clicked),
		NULL
	);
	g_signal_connect(
		self->open_button,
		"clicked",
		G_CALLBACK(open_button_clicked),
    self
	);
}

static void
mb_document_list_row_class_init(MbDocumentListRowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	properties[PROP_CONTENT] = g_param_spec_string(
		"content",                /* Name of property        */
		"Content",                /* Nickname of property    */
		"Text content of block.", /* Description of property */
		"Untitled",               /* Default value           */ 
		G_PARAM_READWRITE         /* Flags                   */
	);

	object_class->dispose      = mb_document_list_row_dispose;
	object_class->finalize     = mb_document_list_row_finalize;
	object_class->set_property = mb_document_list_row_set_property;
	object_class->get_property = mb_document_list_row_get_property;

	gtk_widget_class_set_layout_manager_type(
		GTK_WIDGET_CLASS(klass), 
		GTK_TYPE_BOX_LAYOUT
	);

	g_object_class_install_properties(
		object_class,
		N_PROPERTIES,
		properties
	);
}

static void
mb_document_list_row_dispose (GObject *object)
{
	MbDocumentListRow *self = MB_DOCUMENT_LIST_ROW(object);
	g_clear_pointer(&self->content_label, gtk_widget_unparent);
	g_clear_pointer(&self->open_button, gtk_widget_unparent);
	g_clear_pointer(&self->delete_button, gtk_widget_unparent);
	G_OBJECT_CLASS(mb_document_list_row_parent_class)->dispose(object);
}

static void
mb_document_list_row_finalize(GObject *object)
{
	G_OBJECT_CLASS(mb_document_list_row_parent_class)->finalize(object);
}

// **********************************************************************
// * Public
// **********************************************************************

GtkWidget *
mb_document_list_row_new(gchar *content)
{
	return g_object_new(
		MB_TYPE_DOCUMENT_LIST_ROW,
		"content",
		content,
		NULL
	);
}

