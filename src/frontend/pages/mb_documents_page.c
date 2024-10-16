#include "./mb_documents_page.h"
#include "../components/mb_document_list_row.h"
#include "../../backend/block.h"

static void dispose(GObject *object);
static void finalize(GObject *object);

struct _MbDocumentsPage
{
  GtkWidget parent;
  GtkWidget *vertical_box;
  GtkWidget *document_label;
  GtkWidget *document_list;
  GtkWidget *new_document_button;
};

G_DEFINE_TYPE(MbDocumentsPage, mb_documents_page, GTK_TYPE_WIDGET)

/* Private interface */

static void open_row_cb(MbDocumentListRow *row, gpointer user_data);

void populate_rows(MbDocumentsPage *_self)
{
  GtkBox *_document_list = GTK_BOX(_self->document_list);
  GArray *blocks         = block_get_all();

  for(guint i=0; i < blocks->len; i++)
  {
    Block block = g_array_index(blocks, Block, i);
    GtkWidget *new_row = mb_document_list_row_new(block.content);
    gtk_box_append(_document_list, new_row);

    g_signal_connect(
      new_row,
      "opened",
      G_CALLBACK(open_row_cb),
      NULL
    );
  }
}

/* Callback */

static void open_row_cb(MbDocumentListRow *row, gpointer user_data)
{
  g_print("Callback to open pressed in list row.\n"); 
}

static void 
new_document_button_clicked(GtkButton *self, gpointer user_data)
{
    
}

static void mb_documents_page_init(MbDocumentsPage *self)
{
	self->new_document_button = 
    gtk_button_new_with_label("New Document");

	self->document_list = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_hexpand(self->document_list, TRUE);
	gtk_widget_set_vexpand(self->document_list, TRUE);

	self->document_label = gtk_label_new("Documents");
	gtk_widget_set_halign(self->document_label, GTK_ALIGN_START);

	self->vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_hexpand(self->vertical_box, TRUE);
	gtk_widget_set_vexpand(self->vertical_box, TRUE);
	gtk_box_append(GTK_BOX(self->vertical_box), self->document_label);
	gtk_box_append(GTK_BOX(self->vertical_box), self->document_list);
	gtk_box_append(
    GTK_BOX(self->vertical_box), 
    self->new_document_button
  );

  populate_rows(self);

  /* Callback */
	g_signal_connect(
		self->new_document_button, 
		"clicked", 
		G_CALLBACK(new_document_button_clicked), 
		self->document_list
	);

	gtk_widget_set_parent(self->vertical_box, GTK_WIDGET(self));
}

static void
mb_documents_page_class_init(MbDocumentsPageClass *klass) 
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

  /* Map vfunc */
	object_class->dispose =  dispose;
	object_class->finalize = finalize;

  /* Layout manager */
	gtk_widget_class_set_layout_manager_type(
    GTK_WIDGET_CLASS(klass), 
    GTK_TYPE_BOX_LAYOUT
  );
}

static void dispose(GObject *object)
{
	MbDocumentsPage *self = MB_DOCUMENTS_PAGE(object);
	g_clear_pointer(&self->vertical_box, gtk_widget_unparent);
	G_OBJECT_CLASS(mb_documents_page_parent_class)->dispose(object);
}

static void finalize(GObject *object)
{
	G_OBJECT_CLASS(mb_documents_page_parent_class)->finalize(object);
}

GtkWidget* mb_documents_page_new()
{
  return g_object_new(MB_TYPE_DOCUMENTS_PAGE, NULL);
}
