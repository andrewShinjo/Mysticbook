#include "./mb_documents_page.h"
#include "../components/mb_document_list_row.h"
#include "../../backend/block.h"
/* WIDGET DEFINITION */
struct _MbDocumentsPage
{
  GtkWidget parent;
  /* WIDGETS */
  GtkWidget *vertical_box;
  GtkWidget *document_label;
  GtkWidget *document_list;
  GtkWidget *new_document_button;
  /* EVENT LISTENERS */
  /* PROPERTIES */
  gint64 id;
};
G_DEFINE_TYPE(MbDocumentsPage, mb_documents_page, GTK_TYPE_WIDGET)
/* FORWARD DECLARATION */
static void 
dispose(GObject *object);
static void 
finalize(GObject *object);
static gboolean
open_document_signal_source_func(gpointer user_data);
/* CALLBACK */
static void
open_row_cb(MbDocumentListRow *_row, gpointer user_data)
{
  MbDocumentsPage *_self = MB_DOCUMENTS_PAGE(user_data);
  gint64 id;
  gint64 set_id;
  g_object_get(G_OBJECT(_row), "id", &id, NULL);
  g_object_set(G_OBJECT(_self), "id", id, NULL);
  open_document_signal_source_func(_self);
}
static void 
new_document_button_clicked(GtkButton *button, gpointer user_data)
{
  MbDocumentsPage *_self = MB_DOCUMENTS_PAGE(user_data);
  GtkBox *document_list = GTK_BOX(_self->document_list);
  // Create document in SQL.
  // Create document list row in GUI.
  GtkWidget *new_document_list_row = mb_document_list_row_new();
  gtk_box_append(document_list, new_document_list_row);
  g_signal_connect(
    new_document_list_row,
    "opened",
    G_CALLBACK(open_row_cb),
    _self
  );
}
/* PROPERTIES */
enum property_types
{
  PROP_ID = 1,
  N_PROPERTIES
};
static GParamSpec *properties[N_PROPERTIES];
static void
get_property(
  GObject *object,
  guint property_id,
  GValue *value,
  GParamSpec *pspec
)
{
  MbDocumentsPage *_self = MB_DOCUMENTS_PAGE(object);
  switch(property_id)
  {
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
set_property(
  GObject *object,
  guint property_id,
  const GValue *value,
  GParamSpec *pspec
)
{
  MbDocumentsPage *_self = MB_DOCUMENTS_PAGE(object);
  switch(property_id)
  {
    case PROP_ID:
    {
      _self->id = g_value_get_int64(value);
      g_print("documents_page, set_property: %ld\n", _self->id);
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
  OPEN_DOC,
  LAST_SIGNAL
};
static guint signals[LAST_SIGNAL];
static gboolean
open_document_signal_source_func(gpointer user_data)
{
  MbDocumentsPage *_self = MB_DOCUMENTS_PAGE(user_data);
  g_signal_emit(_self, signals[OPEN_DOC], 0);
  return G_SOURCE_CONTINUE;
}
/* WIDGET LIFECYCLE */
static void 
mb_documents_page_init(MbDocumentsPage *self)
{
  /* INSTANTIATE WIDGETS */
	self->new_document_button = 
    gtk_button_new_with_label("New Document");
	self->document_list = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	self->document_label = gtk_label_new("Documents");
	self->vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  /** Get document ids to populate list rows. **/
  /* CONFIGURE WIDGETS */
  GtkBox *vertical_box = GTK_BOX(self->vertical_box);
	gtk_widget_set_hexpand(self->vertical_box, TRUE);
	gtk_widget_set_vexpand(self->vertical_box, TRUE);
	gtk_box_append(vertical_box, self->document_label);
	gtk_box_append(vertical_box, self->document_list);
	gtk_box_append(
    vertical_box,
    self->new_document_button
  );
	gtk_widget_set_hexpand(self->document_list, TRUE);
	gtk_widget_set_vexpand(self->document_list, TRUE);
	gtk_widget_set_halign(self->document_label, GTK_ALIGN_START);
	gtk_widget_set_parent(self->vertical_box, GTK_WIDGET(self));
  /* CONNECT TO SIGNALS */
	g_signal_connect(
		self->new_document_button, 
		"clicked", 
		G_CALLBACK(new_document_button_clicked), 
		self
	);
}
static void
mb_documents_page_class_init(MbDocumentsPageClass *klass) 
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
  /* MAP VIRTUAL FUNCTIONS */
	object_class->dispose =  dispose;
	object_class->finalize = finalize;
  object_class->get_property = get_property;
  object_class->set_property = set_property;
  /* PROPERTIES */
  properties[PROP_ID] = g_param_spec_int64(
    "id",
    "id of block to open",
    "id of block to open",
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
  signals[OPEN_DOC] = g_signal_new_class_handler(
    "open_doc",
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
dispose(GObject *object)
{
	MbDocumentsPage *self = MB_DOCUMENTS_PAGE(object);
	g_clear_pointer(&self->vertical_box, gtk_widget_unparent);
	G_OBJECT_CLASS(mb_documents_page_parent_class)->dispose(object);
}
static void 
finalize(GObject *object)
{
	G_OBJECT_CLASS(mb_documents_page_parent_class)->finalize(object);
}
/* PUBLIC IMPLEMENTATION */
GtkWidget* 
mb_documents_page_new()
{
  return g_object_new(MB_TYPE_DOCUMENTS_PAGE, NULL);
}
/* PRIVATE IMPLEMENTATION */
