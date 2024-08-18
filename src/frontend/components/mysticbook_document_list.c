#include "../../backend/repository/document_repository.h"
#include "mysticbook_document_list.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

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

static void mysticbook_document_list_dispose(GObject *object);
static void mysticbook_document_list_finalize(GObject *object);

static void
mysticbook_document_list_init (MysticbookDocumentList *self)
{

	self->scrolled_window = gtk_scrolled_window_new ();
	self->list_box = gtk_list_box_new ();

	gtk_widget_set_hexpand (self->scrolled_window, TRUE);
	gtk_widget_set_vexpand (self->scrolled_window, TRUE);

	gtk_scrolled_window_set_child (
		GTK_SCROLLED_WINDOW(self->scrolled_window), self->list_box
	);

  gtk_widget_set_parent(self->scrolled_window, GTK_WIDGET(self));
}

static void
mysticbook_document_list_class_init (MysticbookDocumentListClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
	object_class->dispose = mysticbook_document_list_dispose;
	object_class->finalize = mysticbook_document_list_finalize;
  gtk_widget_class_set_layout_manager_type (
		GTK_WIDGET_CLASS (klass), GTK_TYPE_BOX_LAYOUT
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
// * Interface
// *********************************************************************

GtkWidget *
mysticbook_document_list_new()
{
  return g_object_new(MYSTICBOOK_TYPE_DOCUMENT_LIST, NULL);
}

int
mysticbook_document_list_add_row(MysticbookDocumentList *self)
{
	int64_t new_id = document_repository_create ();
	char s[21];
	snprintf (s, sizeof (s), "%" PRId64, new_id);
	GtkWidget *label = gtk_label_new(s);
	gtk_list_box_append(GTK_LIST_BOX(self->list_box), label);
	return 0;
}
