#include "./mb_tree_outline.h"

struct _MbTreeOutline
{
	GtkWidget parent;

	GtkWidget *container;
	GtkWidget *new_document_button;
	GtkWidget *outline;
};

G_DEFINE_TYPE(MbTreeOutline, mb_tree_outline, GTK_TYPE_WIDGET)

/* Callback */

static void clicked(GtkButton *button, gpointer user_data)
{
	g_print("Clicked\n");
	// Create a new file named "Untitled" in the file system.
	// Add the "Untitled" document to the tree outline.
	// On click, the document should open.
}

/* Widget lifecycle */

static void dispose(GObject *object)
{
	MbTreeOutline *self = MB_TREE_OUTLINE(object);
	g_clear_pointer(&self->container, gtk_widget_unparent);
	G_OBJECT_CLASS(mb_tree_outline_parent_class)->dispose(object);
}

static void mb_tree_outline_init(MbTreeOutline *self) 
{
	self->container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	self->new_document_button = gtk_button_new_with_label("New Document");
	self->outline = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	gtk_box_append(GTK_BOX(self->container), self->new_document_button);
	gtk_box_append(GTK_BOX(self->container), self->outline);
	gtk_widget_set_vexpand(self->outline, TRUE);
	gtk_widget_set_parent(self->container, GTK_WIDGET(self));

	g_signal_connect(self->new_document_button, "clicked", G_CALLBACK(clicked), NULL);
}

static void mb_tree_outline_class_init(MbTreeOutlineClass *klass) 
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

GtkWidget* mb_tree_outline_new()
{
	return g_object_new(MB_TYPE_TREE_OUTLINE, NULL);
}
