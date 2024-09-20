#include "./mb_block_view_page.h"
#include "../components/mb_text_block.h"
#include "../components/mb_root_text_block.h"

static void mb_block_view_page_dispose(GObject *object);
static void mb_block_view_page_finalize(GObject *object);

struct _MbBlockViewPage
{
	GtkWidget parent;
  GtkWidget *scrolled_window;
  GtkWidget *layout;
  GtkWidget *root_block;
};

G_DEFINE_TYPE(MbBlockViewPage, mb_block_view_page, GTK_TYPE_WIDGET)

// Private

gboolean 
prepend_block(MbBlockViewPage *self, GtkWidget *block)
{
  return TRUE;
}

gboolean 
insert_block_after(MbBlockViewPage *self, GtkWidget *sibling, GtkWidget *insert)
{
  return TRUE;
}

static void mb_block_view_page_dispose(GObject *object) 
{
  MbBlockViewPage *self = MB_BLOCK_VIEW_PAGE(object);
  g_clear_pointer(&self->scrolled_window, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->dispose(object);
}

static void mb_block_view_page_finalize(GObject *object)
{
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->finalize(object);
}

static void mb_block_view_page_init(MbBlockViewPage *self)
{
  self->scrolled_window = gtk_scrolled_window_new();
  self->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);  
  self->root_block = mb_root_text_block_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(self->scrolled_window), self->layout);
  gtk_box_append(GTK_BOX(self->layout), self->root_block);
  gtk_widget_set_hexpand(self->layout, TRUE);
  gtk_widget_set_vexpand(self->layout, TRUE);
  gtk_widget_set_parent(self->scrolled_window, GTK_WIDGET(self));
}
static void mb_block_view_page_class_init(MbBlockViewPageClass *klass) 
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = mb_block_view_page_dispose;
  object_class->finalize = mb_block_view_page_finalize;
  gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

// Public

GtkWidget *mb_block_view_page_new()
{
	return g_object_new(MB_TYPE_BLOCK_VIEW_PAGE, NULL);
}
