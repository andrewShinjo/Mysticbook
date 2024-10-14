#include "../../backend/repository/block_repository.h"
#include "./mysticbook_document_list.h"
#include "./mb_document_list_row.h"

struct _MysticbookDocumentList
{
  GtkWidget parent;
  GtkWidget *scrolled_window;
  GtkWidget *list_box;
};

G_DEFINE_TYPE(
  MysticbookDocumentList,
  mysticbook_document_list,
  GTK_TYPE_WIDGET
)

// *********************************************************************
// * Widget lifecycle
// *********************************************************************

static void 
mysticbook_document_list_dispose (GObject *object);

static void 
mysticbook_document_list_finalize (GObject *object);

static void
mysticbook_document_list_init (MysticbookDocumentList *self)
{
	self->scrolled_window = gtk_scrolled_window_new ();
	self->list_box = gtk_list_box_new ();
	gtk_widget_set_hexpand (self->scrolled_window, TRUE);
	gtk_widget_set_vexpand (self->scrolled_window, TRUE);
	gtk_scrolled_window_set_child (
		GTK_SCROLLED_WINDOW (self->scrolled_window), 
		self->list_box
	);
  gtk_widget_set_parent (self->scrolled_window, GTK_WIDGET (self));
}

static void
mysticbook_document_list_class_init (MysticbookDocumentListClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
	object_class->dispose = mysticbook_document_list_dispose;
	object_class->finalize = mysticbook_document_list_finalize;
  gtk_widget_class_set_layout_manager_type (
		GTK_WIDGET_CLASS (klass), 
		GTK_TYPE_BOX_LAYOUT
	);
}

static void 
mysticbook_document_list_dispose(GObject *object)
{
	MysticbookDocumentList *self = MYSTICBOOK_DOCUMENT_LIST (object);
	g_clear_pointer (&self->scrolled_window, gtk_widget_unparent);
	G_OBJECT_CLASS (
		mysticbook_document_list_parent_class
	)->dispose (object);
}

static void
mysticbook_document_list_finalize (GObject *object)
{
    G_OBJECT_CLASS (mysticbook_document_list_parent_class)
			->finalize (object);
}

// *********************************************************************
// * Public
// *********************************************************************

GtkWidget *
mysticbook_document_list_new()
{
  return g_object_new(MYSTICBOOK_TYPE_DOCUMENT_LIST, NULL);
}

int
mysticbook_document_list_add_row(MysticbookDocumentList *self)
{
	gint64 new_id = block_repository_create();
	gchar s[21];
	g_snprintf(s, sizeof(s), "%ld", new_id);
	GtkWidget *list_row = mb_document_list_row_new("Untitled");
	gtk_list_box_append(GTK_LIST_BOX(self->list_box), list_row);
	return 0;
}

int
mysticbook_document_list_get_rows (MysticbookDocumentList *self)
{
	GArray *documents = block_repository_find_all();
	for(guint i = 0; i < documents->len; i++)
	{
		Block b = g_array_index(documents, Block, i);
		GtkWidget *list_row = mb_document_list_row_new(b.content);
		gtk_list_box_append (GTK_LIST_BOX(self->list_box), list_row);
	}
	return 0;
}

