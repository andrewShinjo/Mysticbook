#include "./mb_root_text_block.h"
#include "./mb_text_block.h"

struct _MbRootTextBlock
{
  GtkWidget parent;
  GtkWidget *layout;
  GtkWidget *textview;
  GtkWidget *children_blocks;
  GtkEventController *key_controller;
};

G_DEFINE_TYPE(MbRootTextBlock, mb_root_text_block, GTK_TYPE_WIDGET)

// * Private

void
prepend_child(MbRootTextBlock *self, GtkWidget *child)
{
  gtk_box_prepend(GTK_BOX(self->children_blocks), child);
}

void
append_child_after_sibling(MbRootTextBlock *self, GtkWidget *sibling, GtkWidget *child);

// ** Callback

gboolean
key_pressed(
  GtkEventControllerKey *self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
)
{
  MbRootTextBlock *root = MB_ROOT_TEXT_BLOCK(user_data);
  if(keyval == GDK_KEY_Return)
  {
    g_print("Prepend child\n");
    prepend_child(MB_ROOT_TEXT_BLOCK(user_data), mb_text_block_new());
    return TRUE;
  }
  return FALSE;
}

// ** Widget lifecycle

static void 
mb_root_text_block_dispose(GObject *object) 
{
  MbRootTextBlock *self = MB_ROOT_TEXT_BLOCK(object);
  g_clear_pointer(&self->layout, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_root_text_block_parent_class)->dispose(object);
}

static void 
mb_root_text_block_finalize(GObject *object) {}

static void
mb_root_text_block_init(MbRootTextBlock *self) 
{
  self->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  self->textview = gtk_text_view_new();
  self->children_blocks = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  self->key_controller = gtk_event_controller_key_new();

  gtk_widget_add_controller(self->textview, self->key_controller);
  g_signal_connect(
    self->key_controller,
    "key-pressed",
    G_CALLBACK(key_pressed),
    self
  );

  gtk_box_append(GTK_BOX(self->layout), self->textview);
  gtk_box_append(GTK_BOX(self->layout), self->children_blocks);
  gtk_widget_set_hexpand(self->layout, TRUE);
  gtk_widget_set_vexpand(self->layout, TRUE);
  gtk_widget_set_parent(self->layout, GTK_WIDGET(self));
}

static void 
mb_root_text_block_class_init(MbRootTextBlockClass *klass) 
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = mb_root_text_block_dispose;
  object_class->finalize = mb_root_text_block_finalize;
  gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

// Public

GtkWidget *mb_root_text_block_new()
{
  return g_object_new(MB_TYPE_ROOT_TEXT_BLOCK, NULL);
}
