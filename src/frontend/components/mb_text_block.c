#include "./mb_text_block.h"
#include "./mb_root_text_block.h"

struct _MbTextBlock
{
  GtkWidget parent;
  GtkWidget *layout;
  GtkWidget *hbox;
  GtkWidget *bullet_point;
  GtkWidget *text_view;
  GtkWidget *children_blocks;
  GtkEventController *key_controller;
};

G_DEFINE_TYPE(MbTextBlock, mb_text_block, GTK_TYPE_WIDGET)

// Private

static void
append_sibling_after_self(MbTextBlock *self, MbTextBlock *sibling);

static GtkWidget *
get_first_child(MbTextBlock *_self);

static GtkWidget *
get_parent(MbTextBlock *_self);

static GtkWidget *
get_root(MbTextBlock *_self);

static gboolean
is_insert_at_start(MbTextBlock *_self);

static void
remove_child(MbTextBlock *self, MbTextBlock *_child);

static GtkWidget *
get_first_child(MbTextBlock *_self)
{
  return gtk_widget_get_first_child(_self->children_blocks);
}


// Gets self's parent. 
// Could be either a text block or a root text block.
static GtkWidget *
get_parent(MbTextBlock *_self)
{
  GtkWidget *self = GTK_WIDGET(_self);
  GtkWidget *parent = gtk_widget_get_parent(self);
  while(! MB_IS_TEXT_BLOCK(parent) && ! MB_IS_ROOT_TEXT_BLOCK(parent))
  {
    parent = gtk_widget_get_parent(parent);
  }
  return parent;
}

// Get self's root text block.
 
static GtkWidget *
get_root(MbTextBlock *self)
{
  return gtk_widget_get_ancestor(GTK_WIDGET(self), MB_TYPE_ROOT_TEXT_BLOCK);
}

// Returns TRUE if the insert is at the start of the buffer.
static gboolean
is_insert_at_start(MbTextBlock *_self)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter insert, start;

  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_iter_at_mark(
    text_buffer, 
    &insert, 
    gtk_text_buffer_get_insert(text_buffer)
  );
  return gtk_text_iter_equal(&start, &insert);
}

static void
unindent_self(MbTextBlock *_self) 
{
  GtkWidget *parent = get_parent(_self);

  if(MB_IS_ROOT_TEXT_BLOCK(parent))
  {
    return;
  }

  MbTextBlock *_parent = MB_TEXT_BLOCK(parent);

  remove_child(_parent, _self);
  append_sibling_after_self(_parent, _self);

}

// Adds a text block to self's list of children blocks.
static void
append_child(MbTextBlock* self, MbTextBlock *new_child)
{
  GtkBox *children_blocks = GTK_BOX(self->children_blocks);
  GtkWidget *child_widget = GTK_WIDGET(new_child);
  gtk_box_append(children_blocks, child_widget);
}

// Creates a new text block, then append it after self in the block list.
// Returns the new child.
static void
append_sibling_after_self(MbTextBlock *self, MbTextBlock *sibling)
{
  GtkWidget *parent = get_parent(self);
  
  if(MB_IS_TEXT_BLOCK(parent))
  {
    append_child(MB_TEXT_BLOCK(parent), sibling);
  }
  if(MB_IS_ROOT_TEXT_BLOCK(parent))
  {
    mb_root_text_block_insert_child_after(
      MB_ROOT_TEXT_BLOCK(parent),
      sibling,
      self 
    );
  }
}

// Removes child from self's list of children (if exists).
static void
remove_child(MbTextBlock *self, MbTextBlock *_child)
{
  GtkBox *_children_blocks = GTK_BOX(self->children_blocks);
  GtkWidget *child = GTK_WIDGET(_child);

  if(self->children_blocks != gtk_widget_get_parent(child))
  {
    g_print("mb_text_block.c, remove_child(): child is not in _children_blocks.\n");
    return;
  }
  
  gtk_widget_unparent(child); 
}

// Indents self if a previous sibling exists.
static void
indent_self(MbTextBlock *_self)
{
  GtkWidget *self = GTK_WIDGET(_self);
  GtkWidget *parent = get_parent(_self);
  GtkWidget *previous_sibling = gtk_widget_get_prev_sibling(self);

  if(previous_sibling == NULL)
  {
    return;
  }

  assert(MB_IS_TEXT_BLOCK(previous_sibling));

  MbTextBlock *_previous_sibling = MB_TEXT_BLOCK(previous_sibling);

  if(MB_IS_TEXT_BLOCK(previous_sibling) && MB_IS_TEXT_BLOCK(parent)) 
  {
    MbTextBlock *_parent = MB_TEXT_BLOCK(parent);
    remove_child(_parent, _self);
  }
  else if(MB_IS_TEXT_BLOCK(previous_sibling) && MB_IS_ROOT_TEXT_BLOCK(parent))
  {
    MbRootTextBlock *_parent = MB_ROOT_TEXT_BLOCK(parent);
    mb_root_text_block_remove_child(_parent, _self);
  }
  
  append_child(_previous_sibling, _self);
}

// Removes self from list of blocks.
static void
remove_self(MbTextBlock *_self)
{
  GtkWidget *parent = get_parent(_self);
  GtkWidget *self = GTK_WIDGET(_self);
  GtkWidget *previous_sibling = gtk_widget_get_prev_sibling(self);

  // Remove all of my children.
  // Add the children to my parent.

  GtkWidget *first_child = get_first_child(_self);

  if(MB_IS_TEXT_BLOCK(parent))
  {
    MbTextBlock *_parent = MB_TEXT_BLOCK(parent);
    while(first_child != NULL)
    {
      MbTextBlock *_first_child = MB_TEXT_BLOCK(first_child);
      g_object_ref(first_child);
      remove_child(_self, _first_child); 
      append_child(_parent, _first_child);
      g_object_unref(first_child);
      first_child = get_first_child(_self);
    }
  }
  else if(MB_IS_ROOT_TEXT_BLOCK(parent))
  {
    MbRootTextBlock *_parent = MB_ROOT_TEXT_BLOCK(parent); 
    while(first_child != NULL)
    {
      MbTextBlock *_first_child = MB_TEXT_BLOCK(first_child);
      g_object_ref(first_child);
      remove_child(_self, _first_child);
      mb_root_text_block_insert_child_after(_parent, _first_child, _self);
      g_object_unref(first_child);
      first_child = get_first_child(_self);
    }
  }

  if(MB_IS_TEXT_BLOCK(parent)) 
  {
    MbTextBlock *_parent = MB_TEXT_BLOCK(parent);
    remove_child(_parent, _self);

  }
  else if(MB_IS_ROOT_TEXT_BLOCK(parent)) 
  {
    MbRootTextBlock *_parent = MB_ROOT_TEXT_BLOCK(parent);
    mb_root_text_block_remove_child(_parent, _self);
  }

  if(previous_sibling != NULL)
  {
    MbTextBlock *_previous_sibling = MB_TEXT_BLOCK(previous_sibling);
    mb_text_block_grab_focus(_previous_sibling);
  }
  else if(MB_IS_TEXT_BLOCK(parent))
  {
    MbTextBlock *_parent = MB_TEXT_BLOCK(parent);
    mb_text_block_grab_focus(_parent);
  }
  else if(MB_IS_ROOT_TEXT_BLOCK(parent))
  {
    MbRootTextBlock *_parent = MB_ROOT_TEXT_BLOCK(parent);
    mb_root_text_block_grab_focus(_parent);
  }
}

static gboolean 
key_pressed(
  GtkEventControllerKey *event_controller_key,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
)
{
  MbTextBlock *_self = MB_TEXT_BLOCK(user_data);

  if(keyval == GDK_KEY_BackSpace)
  {
    if(is_insert_at_start(_self))
    {
      remove_self(_self);
    }
  }
  else if(keyval == GDK_KEY_Tab)
  {
    if(state && GDK_CONTROL_MASK)
    {
      unindent_self(_self);
      mb_text_block_grab_focus(_self);
    }
    else
    {
      indent_self(_self);
      mb_text_block_grab_focus(_self);
    }
    return TRUE;
  }
  else if(keyval == GDK_KEY_Return)
  {
    if(!(state && GDK_SHIFT_MASK))
    {
      GtkWidget *sibling = mb_text_block_new();
      MbTextBlock *_sibling = MB_TEXT_BLOCK(sibling);
      append_sibling_after_self(_self, _sibling);
      mb_text_block_grab_focus(_sibling);
      return TRUE;
    }
  }
  return FALSE;
}


// ** Widget lifecycle

static void mb_text_block_dispose(GObject *object) 
{
  MbTextBlock *self = MB_TEXT_BLOCK(object);
  g_clear_pointer(&self->layout, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_text_block_parent_class)->dispose(object);
}

static void mb_text_block_finalize(GObject *object) 
{}

static void mb_text_block_init(MbTextBlock *self) 
{
  self->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  self->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  self->bullet_point = gtk_label_new("  •  ");
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

  gtk_widget_set_hexpand(self->layout, TRUE);
  gtk_widget_set_vexpand(self->layout, TRUE);
  gtk_widget_set_hexpand(self->hbox, TRUE);
  gtk_widget_set_vexpand(self->hbox, TRUE);

  gtk_widget_allocate(self->text_view, 0, 0, 0, NULL);
  gtk_widget_set_hexpand(self->text_view, TRUE);

  gtk_widget_set_valign(self->bullet_point, GTK_ALIGN_START);

  gtk_box_append(GTK_BOX(self->hbox), self->bullet_point);
  gtk_box_append(GTK_BOX(self->hbox), self->text_view);

  gtk_widget_set_margin_start(self->children_blocks, 32);

  gtk_box_append(GTK_BOX(self->layout), self->hbox);
  gtk_box_append(GTK_BOX(self->layout), self->children_blocks);
  gtk_widget_set_parent(self->layout, GTK_WIDGET(self));
}

static void mb_text_block_class_init(MbTextBlockClass *klass) 
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = mb_text_block_dispose;
  object_class->finalize = mb_text_block_finalize;
  gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

// Public

GtkWidget*
mb_text_block_new()
{
  return g_object_new(MB_TYPE_TEXT_BLOCK, NULL);
}

void mb_text_block_grab_focus(MbTextBlock *self)
{
  gtk_widget_grab_focus(self->text_view);
}

void
mb_text_block_add_child(MbTextBlock *self, GtkWidget *child)
{
  gtk_box_append(GTK_BOX(self->children_blocks), child);
}

void
mb_text_block_remove_child(MbTextBlock *self, GtkWidget *child)
{
  GtkBox *_children_blocks = GTK_BOX(self->children_blocks);
  gtk_box_remove(_children_blocks, child);
}
