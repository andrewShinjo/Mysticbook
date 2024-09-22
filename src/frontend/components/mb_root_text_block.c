#include "./mb_root_text_block.h"
#include "./mb_text_block.h"

struct _MbRootTextBlock
{
  GtkWidget parent;
  GtkWidget *layout;
  GtkWidget *hbox;
  GtkWidget *textview;
  GtkWidget *children_blocks;
  GtkEventController *key_controller;
};

G_DEFINE_TYPE(MbRootTextBlock, mb_root_text_block, GTK_TYPE_WIDGET)

// * Private

static void
indent_child(MbRootTextBlock *self, GtkWidget *child)
{
  GtkWidget *previous_sibling = gtk_widget_get_prev_sibling(child);
  MbTextBlock *previous_text_block = MB_TEXT_BLOCK(previous_sibling);
  mb_text_block_add_child(previous_text_block, child);
}

static void
prepend_child(MbRootTextBlock *self, GtkWidget *child)
{
  gtk_box_prepend(GTK_BOX(self->children_blocks), child);
}

static void
append_child_after_sibling(MbRootTextBlock *self, GtkWidget *child, GtkWidget *sibling)
{
  gtk_box_insert_child_after(GTK_BOX(self->children_blocks), child, sibling);
}

static void
indent_self(MbTextBlock *self, gpointer user_data)
{
  g_print("indent_self\n");
  MbRootTextBlock *root = MB_ROOT_TEXT_BLOCK(user_data);
  GtkWidget *child = GTK_WIDGET(self);
  GtkWidget *previous_sibling = gtk_widget_get_prev_sibling(child);

  if(previous_sibling != NULL)
  {
    gtk_box_remove(GTK_BOX(root->children_blocks), child);
    mb_text_block_add_child(MB_TEXT_BLOCK(previous_sibling), child);
  }
}

static void
remove_child(MbTextBlock *self, gpointer user_data)
{
  MbRootTextBlock *root = MB_ROOT_TEXT_BLOCK(user_data);
  GtkWidget *child = GTK_WIDGET(self);
  GtkWidget *previous_sibling = gtk_widget_get_prev_sibling(child);
  gtk_box_remove(GTK_BOX(root->children_blocks), child);
  if(previous_sibling != NULL)
  {
    mb_text_block_grab_focus(MB_TEXT_BLOCK(previous_sibling));
  }
  else
  {
    gtk_widget_grab_focus(root->textview);
  }
}

// ** Callback

static void 
add_sibling(MbTextBlock *self, gpointer user_data)
{
  MbRootTextBlock *root = MB_ROOT_TEXT_BLOCK(user_data);
  GtkWidget *child = mb_text_block_new();
  g_signal_connect(child, "add-sibling", G_CALLBACK(add_sibling), user_data);
  g_signal_connect(child, "indent-self", G_CALLBACK(indent_self), user_data);
  g_signal_connect(child, "remove-self", G_CALLBACK(remove_child), user_data);
  gtk_box_insert_child_after(GTK_BOX(root->children_blocks), child, GTK_WIDGET(self));
  mb_text_block_grab_focus(MB_TEXT_BLOCK(child));
}

static gboolean
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
    GtkWidget *child = mb_text_block_new();
    g_signal_connect(child, "add-sibling", G_CALLBACK(add_sibling), user_data);
    g_signal_connect(child, "indent-self", G_CALLBACK(indent_self), user_data);
    g_signal_connect(child, "remove-self", G_CALLBACK(remove_child), user_data);
    prepend_child(MB_ROOT_TEXT_BLOCK(user_data), child);
    mb_text_block_grab_focus(MB_TEXT_BLOCK(child));
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
  self->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
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

  gtk_box_append(GTK_BOX(self->hbox), self->textview);
  gtk_box_append(GTK_BOX(self->layout), self->hbox);
  gtk_box_append(GTK_BOX(self->layout), self->children_blocks);
  gtk_widget_set_hexpand(self->layout, TRUE);
  gtk_widget_set_vexpand(self->layout, TRUE);
  gtk_widget_set_hexpand(self->textview, TRUE);
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
