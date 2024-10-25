#include "./mb_app_window.h"
#include "./mb_document_list_row.h"

enum property_types
{
	PROP_CONTENT = 1,
  PROP_ID,
	N_PROPERTIES
};

enum signal_types
{
  OPEN,
  LAST_SIGNAL
};

static GParamSpec *properties[N_PROPERTIES];
static guint signals[LAST_SIGNAL];

struct _MbDocumentListRow
{
	GtkWidget parent;

  /* Widgets */
	GtkWidget *id_label;
	GtkWidget *creation_time_label;
	GtkWidget *modification_time_label;
	GtkWidget *content_label;
	GtkWidget *open_button;
	GtkWidget *delete_button;

  /* Properties */
	gchar  *content;
  guint64 id;
};

G_DEFINE_TYPE(MbDocumentListRow, mb_document_list_row, GTK_TYPE_WIDGET)

/* Properties */
static gboolean open_signal_source_func(gpointer user_data)
{
  MbDocumentListRow *_self = MB_DOCUMENT_LIST_ROW(user_data);
  g_signal_emit(_self, signals[OPEN], 0);
  return G_SOURCE_CONTINUE;
}

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
			gtk_label_set_text(
        GTK_LABEL(self->content_label), 
        self->content
      );
			break;
		}
    case PROP_ID:
    {
      self->id = g_value_get_int64(value);
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
mb_document_list_row_get_property(
	GObject *object,
	guint property_id,
	GValue *value,
	GParamSpec *pspec
)
{
	MbDocumentListRow *_self = MB_DOCUMENT_LIST_ROW(object);

	switch(property_id)
	{
		case PROP_CONTENT:
		{
      GtkLabel *content_label = GTK_LABEL(_self->content_label);
			g_value_set_string(value, _self->content);
			gtk_label_set_text(content_label, _self->content);
			break;
		}
    case PROP_ID:
    {
      g_value_set_int64(value, _self->id);
      break;
    }
		default:
		{
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
			break;
		}
	}	
}

/* Callbacks */
static void
delete_button_clicked(GtkButton *self, gpointer user_data)
{
	g_print("Delete button clicked.\n");
}

static void open_button_clicked(GtkButton *button, gpointer user_data)
{
  g_print("mb_doc_list_row: open_button_clicked\n");
  MbDocumentListRow *_self = MB_DOCUMENT_LIST_ROW(user_data);
  open_signal_source_func(_self);
}

static void mb_document_list_row_dispose(GObject *object);
static void mb_document_list_row_finalize(GObject *object);

static void mb_document_list_row_init(MbDocumentListRow *self)
{
  /* Initialize Widgets */
	self->content_label = gtk_label_new(NULL);
	gtk_widget_set_halign(self->content_label, GTK_ALIGN_START);
	gtk_widget_set_hexpand(self->content_label, TRUE);
	self->open_button = gtk_button_new_with_label("Open");
	self->delete_button = gtk_button_new_with_label("Delete");
	gtk_widget_set_parent(self->content_label, GTK_WIDGET(self));
	gtk_widget_set_parent(self->open_button, GTK_WIDGET(self));
	gtk_widget_set_parent(self->delete_button, GTK_WIDGET(self));

  /* Signal Connect */
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

  /* Map vfunc */
	object_class->dispose      = mb_document_list_row_dispose;
	object_class->finalize     = mb_document_list_row_finalize;
	object_class->set_property = mb_document_list_row_set_property;
	object_class->get_property = mb_document_list_row_get_property;

  /* Property */
	properties[PROP_CONTENT] = g_param_spec_string(
		"content",                /* Name of property        */
		"Content",                /* Nickname of property    */
		"Text content of block.", /* Description of property */
		"Untitled",               /* Default value           */ 
		G_PARAM_READWRITE         /* Flags                   */
	);
  properties[PROP_ID] = g_param_spec_int64(
    "id",
    "id",
    "id",
    G_MININT64,
    G_MAXINT64,
    0,
    G_PARAM_READWRITE
  );
	g_object_class_install_properties(
		object_class,
		N_PROPERTIES,
		properties
	);

  /* Signal */
  signals[OPEN] = g_signal_new_class_handler(
    "opened",
    G_OBJECT_CLASS_TYPE(object_class),
    G_SIGNAL_RUN_LAST,
    NULL,
    NULL,
    NULL,
    NULL,
    G_TYPE_NONE,
    0
  );
  /* Layout Manager */
	gtk_widget_class_set_layout_manager_type(
		GTK_WIDGET_CLASS(klass), 
		GTK_TYPE_BOX_LAYOUT
	);
}

static void mb_document_list_row_dispose(GObject *object)
{
	MbDocumentListRow *self = MB_DOCUMENT_LIST_ROW(object);
	g_clear_pointer(&self->content_label, gtk_widget_unparent);
	g_clear_pointer(&self->open_button, gtk_widget_unparent);
	g_clear_pointer(&self->delete_button, gtk_widget_unparent);
	G_OBJECT_CLASS(mb_document_list_row_parent_class)->dispose(object);
}

static void mb_document_list_row_finalize(GObject *object)
{
	G_OBJECT_CLASS(mb_document_list_row_parent_class)->finalize(object);
}

GtkWidget* mb_document_list_row_new(gchar *content, gint64 id)
{
	return g_object_new(
		MB_TYPE_DOCUMENT_LIST_ROW,
		"content", content,
    "id", id,
		NULL
	);
}
