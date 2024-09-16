#include "./mb_block_view_page.h"
#include "../components/mb_text_block.h"

static void mb_block_view_page_dispose(GObject *object);
static void mb_block_view_page_finalize(GObject *object);

struct _MbBlockViewPage
{
	GtkWidget parent;
  GtkWidget *layout;
  GtkWidget *root_block;
  GtkWidget *descendent_blocks;
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

/*
gboolean
main_tv_key_pressed(
  GtkEventControllerKey* self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
)
{

  if(keyval == GDK_KEY_Return)
  {
    GtkBox *children_box = GTK_BOX(user_data);
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *expander = gtk_expander_new(NULL);
    GtkWidget *textview = gtk_text_view_new();
    gtk_widget_set_hexpand(textview, TRUE);
    gtk_box_append(GTK_BOX(hbox), expander);
    gtk_box_append(GTK_BOX(hbox), textview);
    gtk_box_prepend(children_box, hbox);
    gtk_widget_allocate(textview, 0, 0, 0, NULL);
    gtk_widget_grab_focus(textview);
    g_print("Return pressed.\n");
    return TRUE;
  }

  return FALSE;
}
*/

static void mb_block_view_page_dispose(GObject *object) 
{
  MbBlockViewPage *self = MB_BLOCK_VIEW_PAGE(object);
  g_clear_pointer(&self->layout, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->dispose(object);
}

static void mb_block_view_page_finalize(GObject *object)
{
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->finalize(object);
}

static void mb_block_view_page_init(MbBlockViewPage *self)
{
  self->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);  
  self->root_block = mb_text_block_new();
  self->descendent_blocks = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_box_append(GTK_BOX(self->layout), self->root_block);
  gtk_box_append(GTK_BOX(self->layout), self->descendent_blocks);
  gtk_widget_set_hexpand(self->layout, TRUE);
  gtk_widget_set_vexpand(self->layout, TRUE);
  gtk_widget_set_parent(self->layout, GTK_WIDGET(self));
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
