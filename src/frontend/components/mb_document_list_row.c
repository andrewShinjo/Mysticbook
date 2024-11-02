#include "./mb_app_window.h"
#include "./mb_document_list_row.h"
#include "../../backend/block.h"
/* WIDGET DEFINITION */
struct _MbDocumentListRow
{
	GtkWidget parent;
  /* WIDGETS */
	GtkWidget *id_label;
	GtkWidget *creation_time_label;
	GtkWidget *modification_time_label;
	GtkWidget *content_label;
	GtkWidget *open_button;
	GtkWidget *delete_button;
  /* EVENT LISTENERS */
  /* PROPERTIES */
  guint64 id;
	gchar  *content;
};
G_DEFINE_TYPE(MbDocumentListRow, mb_document_list_row, GTK_TYPE_WIDGET)
/* FORWARD DECLARATION */
static void 
mb_document_list_row_dispose(GObject *object);
static void
mb_document_list_row_finalize(GObject *object);
static gboolean 
open_signal_source_func(gpointer user_data);
/* CALLBACK */
static void
delete_button_clicked(GtkButton *self, gpointer user_data)
{
	g_print("Delete button clicked.\n");
}
static void
notify_id(
  GObject *object,
  GParamSpec *pspec,
  gpointer user_data
)
{
  MbDocumentListRow *_self = MB_DOCUMENT_LIST_ROW(object);
  const gchar *content = ""; 
  g_print("listrow notify_id: content=%s\n", content);
  if(content != NULL)
  {
    g_object_set(G_OBJECT(_self), "content", content, NULL); 
  }
}
static void 
open_button_clicked(GtkButton *button, gpointer user_data)
{
  g_print("document_list_row, open_button_clicked\n");
  MbDocumentListRow *_self = MB_DOCUMENT_LIST_ROW(user_data);
  open_signal_source_func(_self);
}

/* PROPERTIES */
enum property_types
{
  PROP_ID = 1,
	PROP_CONTENT,
	N_PROPERTIES
};
static GParamSpec *properties[N_PROPERTIES];
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
/* SIGNALS */
enum signal_types
{
  OPEN,
  LAST_SIGNAL
};
static guint signals[LAST_SIGNAL];
static gboolean 
open_signal_source_func(gpointer user_data)
{
  MbDocumentListRow *_self = MB_DOCUMENT_LIST_ROW(user_data);
  g_signal_emit(_self, signals[OPEN], 0);
  return G_SOURCE_CONTINUE;
}
/* WIDGET LIFECYCLE */
static void 
mb_document_list_row_init(MbDocumentListRow *self)
{
  /* INSTANTIATE WIDGETS */
	self->content_label = gtk_label_new(NULL);
	self->open_button = gtk_button_new_with_label("Open");
	self->delete_button = gtk_button_new_with_label("Delete");
  /* CONFIGURE WIDGETS */
	gtk_widget_set_parent(self->content_label, GTK_WIDGET(self));
	gtk_widget_set_parent(self->open_button, GTK_WIDGET(self));
	gtk_widget_set_parent(self->delete_button, GTK_WIDGET(self));
	gtk_widget_set_halign(self->content_label, GTK_ALIGN_START);
	gtk_widget_set_hexpand(self->content_label, TRUE);
  /* CONNECT TO SIGNALS */
  g_signal_connect(
    self,
    "notify::id",
    G_CALLBACK(notify_id),
    self
  );
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
  /* MAP VIRTUAL FUNCTIONS */
	object_class->dispose      = mb_document_list_row_dispose;
	object_class->finalize     = mb_document_list_row_finalize;
	object_class->set_property = mb_document_list_row_set_property;
	object_class->get_property = mb_document_list_row_get_property;
  /* PROPERTIES */
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
  /* SIGNALS */
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
  /* LAYOUT MANAGER */
	gtk_widget_class_set_layout_manager_type(
		GTK_WIDGET_CLASS(klass), 
		GTK_TYPE_BOX_LAYOUT
	);
}
static void 
mb_document_list_row_dispose(GObject *object)
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
/* PUBLIC IMPLEMENTATION */
GtkWidget* 
mb_document_list_row_new(gint64 id)
{
	return g_object_new(
		MB_TYPE_DOCUMENT_LIST_ROW,
    "id", 
    id,
		NULL
	);
}
/* PRIVATE IMPLEMENTATION */
