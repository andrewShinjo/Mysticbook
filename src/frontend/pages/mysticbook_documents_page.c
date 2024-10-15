#include "./mysticbook_documents_page.h"
#include "../components/mb_document_list.h"

static void mysticbook_documents_page_dispose(GObject *object);
static void mysticbook_documents_page_finalize(GObject *object);

struct _MysticbookDocumentsPage
{
  GtkWidget parent;
  GtkWidget *vertical_box;
  GtkWidget *document_label;
  GtkWidget *document_list;
  GtkWidget *new_document_button;
};

G_DEFINE_TYPE(
  MysticbookDocumentsPage, 
  mysticbook_documents_page, 
  GTK_TYPE_WIDGET
)

/* Signal start */

static void 
new_document_button_clicked(GtkButton *self, gpointer user_data)
{
	MbDocumentList *document_list = MB_DOCUMENT_LIST(user_data);
	mb_document_list_add_row(document_list);
}

/* Signal end */

static void
mysticbook_documents_page_init(MysticbookDocumentsPage *self)
{
	self->new_document_button = gtk_button_new_with_label("New Document");

	self->document_list = mb_document_list_new();
	gtk_widget_set_hexpand (self->document_list, TRUE);
	gtk_widget_set_vexpand (self->document_list, TRUE);

	self->document_label = gtk_label_new("Documents");
	gtk_widget_set_halign(self->document_label, GTK_ALIGN_START);

	self->vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_hexpand(self->vertical_box, TRUE);
	gtk_widget_set_vexpand(self->vertical_box, TRUE);
	gtk_box_append(GTK_BOX(self->vertical_box), self->document_label);
	gtk_box_append(GTK_BOX(self->vertical_box), self->document_list);
	gtk_box_append(GTK_BOX(self->vertical_box), self->new_document_button);

	mb_document_list_get_rows(MB_DOCUMENT_LIST(self->document_list));

	g_signal_connect(
		self->new_document_button, 
		"clicked", 
		G_CALLBACK(new_document_button_clicked), 
		self->document_list
	);

	gtk_widget_set_parent(self->vertical_box, GTK_WIDGET(self));
}

static void
mysticbook_documents_page_class_init(MysticbookDocumentsPageClass *klass) 
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = mysticbook_documents_page_dispose;
	object_class->finalize = mysticbook_documents_page_finalize;
	gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

static void mysticbook_documents_page_dispose(GObject *object)
{
	MysticbookDocumentsPage *self = MYSTICBOOK_DOCUMENTS_PAGE(object);
	g_clear_pointer(&self->vertical_box, gtk_widget_unparent);
	G_OBJECT_CLASS(mysticbook_documents_page_parent_class)->dispose(object);
}

static void mysticbook_documents_page_finalize(GObject *object)
{
	G_OBJECT_CLASS(mysticbook_documents_page_parent_class)->finalize(object);
}

GtkWidget *
mysticbook_documents_page_new()
{
    return g_object_new(MYSTICBOOK_TYPE_DOCUMENTS_PAGE, NULL);
}
