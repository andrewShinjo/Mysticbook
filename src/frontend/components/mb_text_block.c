#include "./mb_text_block.h"

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

// Signal

enum signal_types
{
  ADD_SIBLING,
  INDENT_SELF,
  REMOVE_SELF,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

static gboolean 
add_sibling_signal_source_func(gpointer user_data)
{
  MbTextBlock *self = MB_TEXT_BLOCK(user_data);
  g_signal_emit(self, signals[ADD_SIBLING], 0);
  return G_SOURCE_CONTINUE;
}

static gboolean
indent_self_signal_source_func(gpointer user_data)
{
  MbTextBlock *self = MB_TEXT_BLOCK(user_data);
  g_signal_emit(self, signals[INDENT_SELF], 0);
  return G_SOURCE_CONTINUE;
}

static gboolean 
remove_self_signal_source_func(gpointer user_data)
{
  MbTextBlock *self = MB_TEXT_BLOCK(user_data);
  g_signal_emit(self, signals[REMOVE_SELF], 0);
  return G_SOURCE_CONTINUE;
}

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
  MbTextBlock *text_block = MB_TEXT_BLOCK(user_data);

  if(keyval == GDK_KEY_BackSpace)
  {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(
      GTK_TEXT_VIEW(text_block->text_view)
    ); 
    GtkTextIter insert, start;
    gtk_text_buffer_get_iter_at_mark(buffer, &insert, gtk_text_buffer_get_insert(buffer));
    gtk_text_buffer_get_start_iter(buffer, &start);
    if(gtk_text_iter_equal(&insert, &start))
    {
      remove_self_signal_source_func(user_data);
    }
  }
  else if(keyval == GDK_KEY_Tab)
  {
    indent_self_signal_source_func(user_data);
    return TRUE;
  }
  else if(keyval == GDK_KEY_Return)
  {
    if(!(state && GDK_SHIFT_MASK))
    {
      add_sibling_signal_source_func(user_data);
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

static void mb_text_block_finalize(GObject *object) {}

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

  // Signal

  signals[ADD_SIBLING] = g_signal_new_class_handler(
    "add-sibling", 
    G_OBJECT_CLASS_TYPE(object_class), 
    G_SIGNAL_RUN_LAST,
    NULL,
    NULL,
    NULL,
    NULL,
    G_TYPE_NONE,
    0
  );
  signals[INDENT_SELF] = g_signal_new_class_handler(
    "indent-self",
    G_OBJECT_CLASS_TYPE(object_class),
    G_SIGNAL_RUN_LAST,
    NULL,
    NULL,
    NULL,
    NULL,
    G_TYPE_NONE,
    0
  );
  signals[REMOVE_SELF] = g_signal_new_class_handler(
    "remove-self", 
    G_OBJECT_CLASS_TYPE(object_class), 
    G_SIGNAL_RUN_LAST,
    NULL,
    NULL,
    NULL,
    NULL,
    G_TYPE_NONE,
    0
 );
}

// Public

GtkWidget *mb_text_block_new()
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