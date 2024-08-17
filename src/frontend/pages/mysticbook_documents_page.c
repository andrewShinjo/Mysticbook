#include "mysticbook_documents_page.h"

static void mysticbook_documents_page_dispose(GObject *object);
static void mysticbook_documents_page_finalize(GObject *object);

struct _MysticbookDocumentsPage
{
    GtkWidget parent;
	GtkWidget *vertical_box;
	GtkWidget *document_label;
	GtkWidget *scrolled_window;
	GtkWidget *new_document_button;
};

G_DEFINE_TYPE(
    MysticbookDocumentsPage,
    mysticbook_documents_page,
    GTK_TYPE_WIDGET
)

static void
mysticbook_documents_page_init(MysticbookDocumentsPage *self)
{
	self->document_label = gtk_label_new("Documents");
	gtk_widget_set_halign(self->document_label, GTK_ALIGN_START);

	self->scrolled_window = gtk_scrolled_window_new();
	gtk_widget_set_hexpand(self->scrolled_window, TRUE);
	gtk_widget_set_vexpand(self->scrolled_window, TRUE);

	self->new_document_button = gtk_button_new_with_label("New Document");

	self->vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_hexpand(self->vertical_box, TRUE);
	gtk_widget_set_vexpand(self->vertical_box, TRUE);
	gtk_box_append(GTK_BOX(self->vertical_box), self->document_label);
	gtk_box_append(GTK_BOX(self->vertical_box), self->scrolled_window);
	gtk_box_append(GTK_BOX(self->vertical_box), self->new_document_button);

	

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
