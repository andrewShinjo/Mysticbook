#include "../../backend/block.h"
#include "./mb_document_list.h"
#include "./mb_document_list_row.h"

struct _MbDocumentList
{
  GtkWidget parent;
  GtkWidget *scrolled_window;
  GtkWidget *list_box;
};

G_DEFINE_TYPE(
  MbDocumentList,
  mb_document_list,
  GTK_TYPE_WIDGET
)

static void mb_document_list_dispose (GObject *object);
static void mb_document_list_finalize (GObject *object);

static void 
mb_document_list_init (MbDocumentList *self)
{
	self->scrolled_window = gtk_scrolled_window_new();
	self->list_box = gtk_list_box_new();
	gtk_widget_set_hexpand(self->scrolled_window, TRUE);
	gtk_widget_set_vexpand(self->scrolled_window, TRUE);
	gtk_scrolled_window_set_child(
		GTK_SCROLLED_WINDOW (self->scrolled_window), 
		self->list_box
	);
  gtk_widget_set_parent(self->scrolled_window, GTK_WIDGET (self));
}

static void
mb_document_list_class_init(MbDocumentListClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = mb_document_list_dispose;
	object_class->finalize = mb_document_list_finalize;
  gtk_widget_class_set_layout_manager_type(
		GTK_WIDGET_CLASS(klass), 
		GTK_TYPE_BOX_LAYOUT
	);
}

static void mb_document_list_dispose(GObject *object)
{
	MbDocumentList *self = MB_DOCUMENT_LIST(object);
	g_clear_pointer(&self->scrolled_window, gtk_widget_unparent);
	G_OBJECT_CLASS(mb_document_list_parent_class)->dispose(object);
}

static void mb_document_list_finalize(GObject *object)
{
  G_OBJECT_CLASS(mb_document_list_parent_class)->finalize(
    object
  );
}

GtkWidget* mb_document_list_new()
{
  return g_object_new(MB_TYPE_DOCUMENT_LIST, NULL);
}

int mb_document_list_add_row(MbDocumentList *self)
{
	gint64 new_id = block_new();
	//gchar s[21];
	//g_snprintf(s, sizeof(s), "%ld", new_id);
	GtkWidget *list_row = mb_document_list_row_new("Untitled");
	gtk_list_box_append(GTK_LIST_BOX(self->list_box), list_row);
	return 0;
}

int mb_document_list_get_rows(MbDocumentList *self)
{
	GArray *documents = block_get_all();
	for(guint i = 0; i < documents->len; i++)
	{
		Block b = g_array_index(documents, Block, i);
		GtkWidget *list_row = mb_document_list_row_new(b.content);
		gtk_list_box_append (GTK_LIST_BOX(self->list_box), list_row);
	}
	return 0;
}

