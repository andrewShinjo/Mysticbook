#include "./mb_root_text_block.h"
#include "./mb_text_block.h"

struct _MbRootTextBlock
{
  GtkWidget parent;
  GtkWidget *layout;
  GtkWidget *hbox;
  GtkWidget *text_view;
  GtkWidget *children_blocks;
  GtkEventController *key_controller;
};

G_DEFINE_TYPE(MbRootTextBlock, mb_root_text_block, GTK_TYPE_WIDGET)

// Private

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
    gtk_box_prepend(GTK_BOX(root->children_blocks), child);
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
  self->text_view = gtk_text_view_new();
  self->children_blocks = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  self->key_controller = gtk_event_controller_key_new();

  gtk_widget_add_controller(self->text_view, self->key_controller);
  g_signal_connect(
    self->key_controller,
    "key-pressed",
    G_CALLBACK(key_pressed),
    self
  );

  gtk_box_append(GTK_BOX(self->hbox), self->text_view);
  gtk_box_append(GTK_BOX(self->layout), self->hbox);
  gtk_box_append(GTK_BOX(self->layout), self->children_blocks);
  gtk_widget_set_hexpand(self->layout, TRUE);
  gtk_widget_set_vexpand(self->layout, TRUE);
  gtk_widget_set_hexpand(self->text_view, TRUE);
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

void
mb_root_text_block_append_content(MbRootTextBlock *_self, gchar *content)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  gtk_text_buffer_insert(text_buffer, &end, content, -1);
}

void
mb_root_text_block_insert_child_after(
  MbRootTextBlock *self,
  MbTextBlock *child,
  MbTextBlock *sibling
)
{
  gtk_box_insert_child_after(
    GTK_BOX(self->children_blocks),
    GTK_WIDGET(child),
    GTK_WIDGET(sibling)
  );
}

GtkWidget *mb_root_text_block_new()
{
  return g_object_new(MB_TYPE_ROOT_TEXT_BLOCK, NULL);
}

void
mb_root_text_block_grab_focus(MbRootTextBlock *self)
{
  gtk_widget_grab_focus(self->text_view);
}

// Removes child from self's list of children blocks.
void
mb_root_text_block_remove_child(MbRootTextBlock *_self, MbTextBlock *_child)
{
  GtkBox *_children_blocks = GTK_BOX(_self->children_blocks);
  GtkWidget *child = GTK_WIDGET(_child);

  if(_self->children_blocks != gtk_widget_get_parent(child))
  {
    g_print(
      "mb_root_text_block.c, mb_root_text_block_remove_child(): "
      "child is not in self->children_blocks.\n"
    );
    return;
  }

  gtk_box_remove(_children_blocks, child);
}
