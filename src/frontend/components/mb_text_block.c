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

  gboolean selected;
};

G_DEFINE_TYPE(MbTextBlock, mb_text_block, GTK_TYPE_WIDGET)

/* PRIVATE INTERFACE */

static void
append_content(MbTextBlock *_self, gchar *content);

static void
append_sibling_after_self(MbTextBlock *self, MbTextBlock *sibling);

static void
force_redraw_cursor(MbTextBlock *self);

static gchar *
get_content(MbTextBlock *_self);

static GtkWidget *
get_first_child(MbTextBlock *_self);

static MbTextBlock *
get_last_child(MbTextBlock *_self);

static GtkWidget *
get_parent(MbTextBlock *_self);

static GtkWidget *
get_root(MbTextBlock *_self);

static gboolean
has_child(MbTextBlock *_self);

static void
insert_child_after(
  MbTextBlock *_self,
  MbTextBlock *_child,
  MbTextBlock *_sibling
);

static gboolean
is_all_text_highlighted(MbTextBlock *_self);

static gboolean
is_empty(MbTextBlock *_self);

static gboolean
is_insert_at_start(MbTextBlock *_self);

static void
remove_child(MbTextBlock *self, MbTextBlock *_child);

static void
snapshot(GtkWidget *widget, GtkSnapshot *snapshot);

/* PRIVATE IMPLEMENTATION */

static void
append_content(MbTextBlock *_self, gchar *content)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  gtk_text_buffer_insert(text_buffer, &end, content, -1);
}

static void
force_redraw_cursor(MbTextBlock *_self)
{
  if(!is_empty(_self))
  {
    return;
  }
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  gtk_text_buffer_set_text(text_buffer, " ", -1);
  gtk_text_buffer_set_text(text_buffer, "", -1);
}

static gchar *
get_content(MbTextBlock *_self)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  gchar *content = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);
  return content;
}

static GtkWidget *
get_first_child(MbTextBlock *_self)
{
  return gtk_widget_get_first_child(_self->children_blocks);
}

static MbTextBlock *
get_last_child(MbTextBlock *_self)
{
  GtkWidget *last_child = gtk_widget_get_last_child(_self->children_blocks);
  return MB_TEXT_BLOCK(last_child);
}

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

static GtkWidget *
get_root(MbTextBlock *self)
{
  return gtk_widget_get_ancestor(GTK_WIDGET(self), MB_TYPE_ROOT_TEXT_BLOCK);
}

static gboolean
has_child(MbTextBlock *_self)
{
  return gtk_widget_get_first_child(_self->children_blocks) != NULL;
}

static void
insert_child_after(
  MbTextBlock *_self,
  MbTextBlock *_child,
  MbTextBlock *_sibling
)
{
  // gtk_widget_insert_after
  GtkBox *_children_blocks = GTK_BOX(_self->children_blocks);
  GtkWidget *child = GTK_WIDGET(_child);
  GtkWidget *sibling = GTK_WIDGET(_sibling);
  gtk_box_insert_child_after(_children_blocks, child, sibling);

}

static gboolean
is_all_text_highlighted(MbTextBlock *_self)
{
  GtkTextView *_text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(_text_view);
  GtkTextIter start, end, highlight_start, highlight_end;
  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  gtk_text_buffer_get_selection_bounds(text_buffer, &highlight_start, &highlight_end);
  return gtk_text_iter_equal(&start, &highlight_start) 
    && gtk_text_iter_equal(&end, &highlight_end)
    && !gtk_text_iter_equal(&highlight_start, &highlight_end);
}

static gboolean
is_empty(MbTextBlock *_self)
{
  GtkTextView *_text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(_text_view);
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  return gtk_text_iter_equal(&start, &end);
}

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
prepend_child(MbTextBlock *_self, MbTextBlock *_child)
{
  GtkBox *_children_blocks = GTK_BOX(_self->children_blocks);
  GtkWidget *child = GTK_WIDGET(_child);
  gtk_box_prepend(_children_blocks, child);
}

static void
snapshot(GtkWidget *widget, GtkSnapshot *snapshot)
{
  MbTextBlock* _self = MB_TEXT_BLOCK(widget);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(mb_text_block_parent_class);
  widget_class->snapshot(widget, snapshot);

  if(_self->selected)
  {
    int height = gtk_widget_get_height(widget);
    int width = gtk_widget_get_width(widget);
    graphene_rect_t graphene_rect;
    if(gtk_widget_compute_bounds(widget, widget, &graphene_rect))
    {
      GdkRGBA color;
      gdk_rgba_parse(&color, "rgba(255, 0, 0, 0.25)");
      gtk_snapshot_append_color(snapshot, &color, &graphene_rect);
    }
  }
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

static void
append_child(MbTextBlock* self, MbTextBlock *_child)
{
  GtkBox *_children_blocks = GTK_BOX(self->children_blocks);
  GtkWidget *child = GTK_WIDGET(_child);
  gtk_box_append(_children_blocks, child);
}

static void
append_sibling_after_self(MbTextBlock *_self, MbTextBlock *_sibling)
{
  GtkWidget *parent = get_parent(_self);
  
  if(MB_IS_TEXT_BLOCK(parent))
  {
    MbTextBlock *_parent = MB_TEXT_BLOCK(parent);
    insert_child_after(
      _parent,
      _sibling,
      _self
    );
  }
  if(MB_IS_ROOT_TEXT_BLOCK(parent))
  {
    MbRootTextBlock *_parent = MB_ROOT_TEXT_BLOCK(parent);
    mb_root_text_block_insert_child_after(
      _parent,
      _sibling,
      _self 
    );
  }
}

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

static void
remove_self(MbTextBlock *_self)
{
  GtkWidget *parent = get_parent(_self);
  GtkWidget *self = GTK_WIDGET(_self);
  GtkWidget *previous_sibling = gtk_widget_get_prev_sibling(self);
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

  gchar *content = get_content(_self);

  if(previous_sibling != NULL)
  {
    MbTextBlock *_last_child = MB_TEXT_BLOCK(previous_sibling);
    MbTextBlock *_previous = NULL;
    while(_last_child != NULL)
    {
      _previous = _last_child;
      _last_child = get_last_child(_last_child);
    }
    assert(MB_IS_TEXT_BLOCK(_previous));
    append_content(_previous, content);
    mb_text_block_grab_focus(_previous);
    force_redraw_cursor(_previous);

  }
  else if(MB_IS_TEXT_BLOCK(parent))
  {
    MbTextBlock *_parent = MB_TEXT_BLOCK(parent);
    append_content(_parent, content);
    mb_text_block_grab_focus(_parent);
  }
  else if(MB_IS_ROOT_TEXT_BLOCK(parent))
  {
    MbRootTextBlock *_parent = MB_ROOT_TEXT_BLOCK(parent);
    mb_root_text_block_append_content(_parent, content);
    mb_root_text_block_grab_focus(_parent);
  }
  g_free(content);
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

  if(keyval == GDK_KEY_a)
  {
    if(state & GDK_CONTROL_MASK)
    {
      gboolean should_highlight_block = is_all_text_highlighted(_self) || is_empty(_self);
      if(should_highlight_block)
      {
        g_print("Highlight block.\n");
        _self->selected = TRUE;
        gtk_widget_queue_draw(GTK_WIDGET(_self));
      }
    }
  }
  else if(keyval == GDK_KEY_BackSpace)
  {
    if(is_insert_at_start(_self) && ! is_all_text_highlighted(_self))
    {
      remove_self(_self);
    }
  }
  else if(keyval == GDK_KEY_ISO_Left_Tab && state && GDK_SHIFT_MASK)
  {
    unindent_self(_self);
    mb_text_block_grab_focus(_self);
    return TRUE;
  }
  else if(keyval == GDK_KEY_Tab)
  {
    indent_self(_self);
    mb_text_block_grab_focus(_self);
    return TRUE;
  }
  else if(keyval == GDK_KEY_Return)
  {
    if(! (state && GDK_SHIFT_MASK) )
    {
      if(has_child(_self))
      {
        GtkWidget *child = mb_text_block_new();
        MbTextBlock *_child = MB_TEXT_BLOCK(child);
        prepend_child(_self, _child);
        mb_text_block_grab_focus(_child);
      }
      else
      {
        GtkWidget *sibling = mb_text_block_new();
        MbTextBlock *_sibling = MB_TEXT_BLOCK(sibling);
        append_sibling_after_self(_self, _sibling);
        mb_text_block_grab_focus(_sibling);
      }
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
  self->selected = FALSE;

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
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  object_class->dispose = mb_text_block_dispose;
  object_class->finalize = mb_text_block_finalize;
  widget_class->snapshot = snapshot;
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
