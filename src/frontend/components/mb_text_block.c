#include "./mb_text_block.h"
#include "./mb_root_text_block.h"
#include "../../backend/block.h"
#include "../../backend/controller/block_controller.h"
/* WIDGET DEFINITION */
struct _MbTextBlock
{
  GtkWidget parent;
  /* WIDGETS */
  GtkWidget *layout;
  GtkWidget *hbox;
  GtkWidget *icon;
  GtkWidget *icon_button;
  GtkWidget *bullet_point;
  GtkWidget *bp_icon;
  GtkWidget *bp_button;
  GtkWidget *text_view;
  GtkWidget *children_blocks;
  /* EVENT LISTENERS */
  GtkEventController *key_controller;
  GtkEventController *focus_controller;
  gboolean selected;
  /* PROPERTIES */
  gint64 id;
  gboolean expanded;
};
G_DEFINE_TYPE(MbTextBlock, mb_text_block, GTK_TYPE_WIDGET)
/* FORWARD DECLARATION */
static void append_sibling_after_self(MbTextBlock *_self, MbTextBlock *_sibling);
static void dispose(GObject *object);
static void finalize(GObject *object);
static GtkWidget* get_parent(MbTextBlock *_self);
static void get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static GtkWidget* get_root(MbTextBlock *self);
static gboolean has_child(MbTextBlock *_self);
static void indent_self(MbTextBlock *_self);
static gboolean is_all_text_highlighted(MbTextBlock *_self);
static gboolean is_empty(MbTextBlock *_self);
static gboolean is_insert_at_start(MbTextBlock *_self);
static void prepend_child(MbTextBlock *_self, MbTextBlock *_child);
static void remove_child(MbTextBlock *self, MbTextBlock *_child);
static void remove_self(MbTextBlock *_self);
static void set_content(MbTextBlock *_self, const gchar *content);
static void set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void snapshot(GtkWidget *widget, GtkSnapshot *snapshot);
static void leave(GtkEventControllerFocus *focus, gpointer user_data);
static void unindent_self(MbTextBlock *_self);
/* CALLBACK */
static void changed(GtkTextBuffer *text_buffer, gpointer user_data)
{
  MbTextBlock *_self = MB_TEXT_BLOCK(user_data);
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  gchar *content = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);
  block_controller_update_content(_self->id, (const unsigned char*) content);
  g_free(content);
}
static void notify_expanded(GObject *object, GParamSpec *pspec, gpointer user_data)
{
  MbTextBlock *_self = MB_TEXT_BLOCK(object);
  GtkWidget *self = GTK_WIDGET(object);
  GtkImage *_icon_image = GTK_IMAGE(_self->icon);
  g_print("notify_expanded: %d\n", _self->expanded);
  if(_self->expanded)
  {
    gtk_image_set_from_file(_icon_image, "./resources/white_arrow_expand.png");
  }
  else
  {
    gtk_image_set_from_file(_icon_image, "./resources/white_arrow_hide.png"); 
  }
  gtk_widget_set_visible(_self->children_blocks, _self->expanded);
  block_controller_update_expanded(_self->id, _self->expanded);
}
static void notify_id(GObject *object, GParamSpec *pspec, gpointer user_data)
{
  g_print("notify_id\n");
  MbTextBlock *_self = MB_TEXT_BLOCK(object);
  gint64 id = _self->id;
  const unsigned char *content = block_controller_get_block_content(id);
  if(content != NULL)
  {
    set_content(_self, content);     
  }
  // Get block children.
  GtkBox *_children_blocks = GTK_BOX(_self->children_blocks);
  GArray *children_ids = block_controller_get_children_ids(id);
  guint count = children_ids->len;
  for(guint i = 0; i < count; i++)
  {
    gint64 child_id = g_array_index(children_ids, gint64, i);
    GtkWidget *child = mb_text_block_new(child_id);
    MbTextBlock *_child = MB_TEXT_BLOCK(child);
    gtk_box_append(_children_blocks, child);
    mb_text_block_grab_focus(_child);
  }
  g_free(children_ids);
}
static void expand_clicked(GtkButton *button, gpointer user_data)
{
  MbTextBlock *_self = MB_TEXT_BLOCK(user_data);
  gboolean is_expanded;
  g_object_get(_self, "expanded", &is_expanded, NULL);
  g_object_set(_self, "expanded", !is_expanded, NULL);
}
static gboolean key_pressed(GtkEventControllerKey *key, guint keyval, guint keycode, GdkModifierType state, gpointer user_data)
{
  MbTextBlock *_self = MB_TEXT_BLOCK(user_data);
  GtkWidget *self  = GTK_WIDGET(user_data);

  if(keyval == GDK_KEY_a)
  {
    if(state & GDK_CONTROL_MASK)
    {
      gboolean should_highlight_block = is_all_text_highlighted(_self) || is_empty(_self);
      if(should_highlight_block)
      {
        // _self->selected = TRUE;
        // gtk_text_view_set_cursor_visible(
        //   GTK_TEXT_VIEW(_self->text_view), 
        //   FALSE
        // );
        // gtk_text_view_set_editable(
        //   GTK_TEXT_VIEW(_self->text_view), 
        //   FALSE
        // );
        //gtk_widget_queue_draw(GTK_WIDGET(_self));
      }
    }
  }
  else if(keyval == GDK_KEY_BackSpace)
  {
    if(is_insert_at_start(_self) && !is_all_text_highlighted(_self))
    {
      g_print("Remove self.\n");
      // Remove self in GUI.
      remove_self(_self);
      // Remove self in SQL.
      block_controller_delete_block(_self->id); 
    }
  }
  else if(keyval == GDK_KEY_ISO_Left_Tab && state && GDK_SHIFT_MASK)
  {
    // Unindent in GUI.
    unindent_self(_self);
    mb_text_block_grab_focus(_self);
    // Unindent in SQL.
    block_controller_unindent_block(_self->id);
    return TRUE;
  }
  else if(keyval == GDK_KEY_Tab)
  {
    // Indent in GUI.
    indent_self(_self);
    mb_text_block_grab_focus(_self);
    // Indent in SQL.
    block_controller_indent_block(_self->id);
    return TRUE;
  }
  else if(keyval == GDK_KEY_Return)
  {
    _self->selected = FALSE;
    GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
    gtk_text_view_set_cursor_visible(text_view, TRUE);
    gtk_text_view_set_editable(text_view, TRUE);
    gtk_widget_queue_draw(self);
    // Add new block.
    if(!state || !GDK_SHIFT_MASK)
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
        // Create a new block in SQL.
        g_print("ID=%ld\n", _self->id);
        gint64 new_id = block_controller_append_sibling(_self->id);
        // Create a new block in GUI.
        GtkWidget *sibling = mb_text_block_new(new_id);
        MbTextBlock *_sibling = MB_TEXT_BLOCK(sibling);
        append_sibling_after_self(_self, _sibling);
        mb_text_block_grab_focus(_sibling);
      }
      return TRUE;
    }
  }
  return FALSE;
}

/* PROPERTIES */
enum property_types
{
  PROP_ID = 1,
  PROP_EXPANDED,
  N_PROPERTIES
};
static GParamSpec *properties[N_PROPERTIES];
static void get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  MbTextBlock *_self = MB_TEXT_BLOCK(object);
  switch(property_id)
  {
    case PROP_ID:
    {
      g_value_set_int64(value, _self->id);
      break;
    }
    case PROP_EXPANDED:
    {
      g_value_set_boolean(value, _self->expanded);
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}
static void set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  MbTextBlock *_self = MB_TEXT_BLOCK(object);
  switch(property_id)
  {
    case PROP_ID:
    {
      _self->id = g_value_get_int64(value);
      break;
    }
    case PROP_EXPANDED:
    {
      _self->expanded = g_value_get_boolean(value);
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}
/* SIGNALS */
/* WIDGET LIFECYCLE */
static void 
mb_text_block_init(MbTextBlock *self) 
{
  /* INSTANTIATE WIDGETS */
  self->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  self->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  self->icon = gtk_image_new_from_file("./resources/white_arrow_hide.png");
  self->icon_button = gtk_button_new();
  self->bullet_point = gtk_label_new("  •  ");
  self->bp_icon = gtk_image_new_from_file("./resources/bp_default.svg");
  self->bp_button = gtk_button_new();
  self->text_view = gtk_text_view_new();
  self->children_blocks = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  self->key_controller = gtk_event_controller_key_new();
  self->focus_controller = gtk_event_controller_focus_new();
  self->selected = FALSE;
  /* CONFIGURE WIDGETS */
  GtkButton *_icon_button = GTK_BUTTON(self->icon_button);
  GtkButton *_bp_button = GTK_BUTTON(self->bp_button);
  gtk_button_set_child(_icon_button, self->icon);
  gtk_button_set_child(_bp_button, self->bp_icon);
  gtk_widget_set_hexpand(self->layout, TRUE);
  gtk_widget_set_vexpand(self->layout, TRUE);
  gtk_widget_set_hexpand(self->hbox, TRUE);
  gtk_widget_set_vexpand(self->hbox, TRUE);
  gtk_widget_allocate(self->text_view, 0, 0, 0, NULL);
  gtk_widget_set_hexpand(self->text_view, TRUE);
  gtk_widget_set_valign(self->bullet_point, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(self->hbox), self->icon_button);
  gtk_box_append(GTK_BOX(self->hbox), self->bp_button);
  //gtk_box_append(GTK_BOX(self->hbox), self->bullet_point);
  gtk_box_append(GTK_BOX(self->hbox), self->text_view);
  gtk_widget_set_margin_start(self->children_blocks, 32);
  gtk_box_append(GTK_BOX(self->layout), self->hbox);
  gtk_box_append(GTK_BOX(self->layout), self->children_blocks);
  gtk_widget_set_parent(self->layout, GTK_WIDGET(self)); 
  /* CONNECT TO SIGNALS */
  g_signal_connect(GTK_WIDGET(self), "notify::id", G_CALLBACK(notify_id), self);
  g_signal_connect(GTK_WIDGET(self), "notify::expanded", G_CALLBACK(notify_expanded), self);
  /** Key controller **/
  gtk_widget_add_controller(self->text_view, self->key_controller);
  g_signal_connect(self->key_controller, "key-pressed", G_CALLBACK(key_pressed), self);
  /** Focus controller **/
  gtk_widget_add_controller(self->text_view, self->focus_controller);
  g_signal_connect(self->focus_controller, "leave", G_CALLBACK(leave), self);
  g_signal_connect(self->icon_button, "clicked", G_CALLBACK(expand_clicked), self);
  /** Text buffer **/
  GtkTextView *text_view = GTK_TEXT_VIEW(self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  g_signal_connect(text_buffer, "changed", G_CALLBACK(changed), self);
}
static void mb_text_block_class_init(MbTextBlockClass *klass) 
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  /* MAP VIRTUAL FUNCTIONS */
  object_class->dispose = dispose;
  object_class->finalize = finalize;
  object_class->get_property = get_property;
  object_class->set_property = set_property;
  widget_class->snapshot = snapshot;
  /* PROPERTIES */
  properties[PROP_ID] = g_param_spec_int64("id", "id", "id", 0, G_MAXINT64, 0, G_PARAM_READWRITE);
  properties[PROP_EXPANDED] = g_param_spec_boolean("expanded", "expanded", "expanded", TRUE, G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, properties);
  /* SIGNALS */
  /* LAYOUT MANAGER */
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);
}
static void dispose(GObject *object) 
{
  MbTextBlock *self = MB_TEXT_BLOCK(object);
  g_clear_pointer(&self->layout, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_text_block_parent_class)->dispose(object);
}
static void finalize(GObject *object) 
{

}
/* PUBLIC IMPLEMENTATION */
void mb_text_block_grab_focus(MbTextBlock *self)
{
  gtk_widget_grab_focus(self->text_view);
}
GtkWidget* mb_text_block_new(gint64 id)
{
  gboolean expanded = block_controller_get_expanded(id);
  return g_object_new(MB_TYPE_TEXT_BLOCK, "id", id, "expanded", expanded, NULL);
}
void mb_text_block_add_child(MbTextBlock *self, GtkWidget *child)
{
  gtk_box_append(GTK_BOX(self->children_blocks), child);
}
void mb_text_block_remove_child(MbTextBlock *self, GtkWidget *child)
{
  GtkBox *_children_blocks = GTK_BOX(self->children_blocks);
  gtk_box_remove(_children_blocks, child);
}
/* PRIVATE IMPLEMENTATION */
static void append_content(MbTextBlock *_self, gchar *content)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  gtk_text_buffer_insert(text_buffer, &end, content, -1);
}
static void force_redraw_cursor(MbTextBlock *_self)
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
static gchar* get_content(MbTextBlock *_self)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  gchar *content = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);
  return content;
}
static GtkWidget* get_first_child(MbTextBlock *_self)
{
  return gtk_widget_get_first_child(_self->children_blocks);
}
static GtkWidget* get_last_child(MbTextBlock *_self)
{
  GtkWidget *last_child = gtk_widget_get_last_child(_self->children_blocks);
  return last_child;
}
static GtkWidget* get_parent(MbTextBlock *_self)
{
  // gtk_widget_get_ancestor won't work because it will return self.
  GtkWidget *self = GTK_WIDGET(_self);
  GtkWidget *parent = gtk_widget_get_parent(self);
  while(!MB_IS_TEXT_BLOCK(parent) && !MB_IS_ROOT_TEXT_BLOCK(parent) || parent == NULL)
  {
    parent = gtk_widget_get_parent(parent);
  }
  return parent;
}
static GtkWidget* get_root(MbTextBlock *self)
{
  return gtk_widget_get_ancestor(GTK_WIDGET(self), MB_TYPE_ROOT_TEXT_BLOCK);
}
static gboolean has_child(MbTextBlock *_self)
{
  return gtk_widget_get_first_child(_self->children_blocks) != NULL;
}
static void insert_child_after(MbTextBlock *_self, MbTextBlock *_child, MbTextBlock *_sibling)
{
  // gtk_widget_insert_after
  GtkBox *_children_blocks = GTK_BOX(_self->children_blocks);
  GtkWidget *child = GTK_WIDGET(_child);
  GtkWidget *sibling = GTK_WIDGET(_sibling);
  gtk_box_insert_child_after(_children_blocks, child, sibling);
}
static gboolean is_all_text_highlighted(MbTextBlock *_self)
{
  GtkTextView *_text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(_text_view);
  GtkTextIter start, end, highlight_start, highlight_end;
  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  gtk_text_buffer_get_selection_bounds(text_buffer, &highlight_start, &highlight_end);
  return gtk_text_iter_equal(&start, &highlight_start) && gtk_text_iter_equal(&end, &highlight_end) 
    && !gtk_text_iter_equal(&highlight_start, &highlight_end);
}
static gboolean is_empty(MbTextBlock *_self)
{
  GtkTextView *_text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(_text_view);
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  return gtk_text_iter_equal(&start, &end);
}
static gboolean is_insert_at_start(MbTextBlock *_self)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter insert, start;
  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_iter_at_mark(text_buffer, &insert, gtk_text_buffer_get_insert(text_buffer));
  return gtk_text_iter_equal(&start, &insert);
}
static void leave(GtkEventControllerFocus *focus, gpointer user_data)
{
  MbTextBlock *_self = MB_TEXT_BLOCK(user_data);
  GtkWidget *self = GTK_WIDGET(user_data);
  _self->selected = FALSE;
  gtk_widget_queue_draw(self);
}
static void prepend_child(MbTextBlock *_self, MbTextBlock *_child)
{
  GtkBox *_children_blocks = GTK_BOX(_self->children_blocks);
  GtkWidget *child = GTK_WIDGET(_child);
  gtk_box_prepend(_children_blocks, child);
}
static void set_content(MbTextBlock *_self, const gchar *content)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  gtk_text_buffer_set_text(text_buffer, content, -1);
}
static void snapshot(GtkWidget *widget, GtkSnapshot *snapshot)
{
  MbTextBlock* _self = MB_TEXT_BLOCK(widget);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(mb_text_block_parent_class);
  widget_class->snapshot(widget, snapshot);
  if(_self->selected)
  {
    int height = gtk_widget_get_height(widget);
    int width = gtk_widget_get_width(widget);
    graphene_rect_t graphene_rect;
    if(gtk_widget_compute_bounds(_self->hbox, _self->hbox, &graphene_rect))
    {
      GdkRGBA color;
      gdk_rgba_parse(&color, "rgba(255, 0, 0, 0.25)");
      gtk_snapshot_append_color(snapshot, &color, &graphene_rect);
    }
  }
}
static void unindent_self(MbTextBlock *_self) 
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
static void append_child(MbTextBlock* self, MbTextBlock *_child)
{
  GtkBox *_children_blocks = GTK_BOX(self->children_blocks);
  GtkWidget *child = GTK_WIDGET(_child);
  gtk_box_append(_children_blocks, child);
}
static void append_sibling_after_self(MbTextBlock *_self, MbTextBlock *_sibling)
{
  GtkWidget *parent = get_parent(_self);
  
  if(MB_IS_TEXT_BLOCK(parent))
  {
    MbTextBlock *_parent = MB_TEXT_BLOCK(parent);
    insert_child_after(_parent, _sibling, _self);
  }
  if(MB_IS_ROOT_TEXT_BLOCK(parent))
  {
    MbRootTextBlock *_parent = MB_ROOT_TEXT_BLOCK(parent);
    mb_root_text_block_insert_child_after(_parent, _sibling, _self);
  }
}
static void remove_child(MbTextBlock *_self, MbTextBlock *_child)
{
  GtkBox *_children_blocks = GTK_BOX(_self->children_blocks);
  GtkWidget *child = GTK_WIDGET(_child);
  if(_self->children_blocks != gtk_widget_get_parent(child))
  {
    return;
  }
  gtk_widget_unparent(child); 
}
static void indent_self(MbTextBlock *_self)
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
  g_object_set(previous_sibling, "expanded", TRUE, NULL);
  append_child(_previous_sibling, _self);
}
static void remove_self(MbTextBlock *_self)
{
  GtkWidget *parent = get_parent(_self);
  GtkWidget *self = GTK_WIDGET(_self);
  GtkWidget *previous_sibling = gtk_widget_get_prev_sibling(self);

  if(MB_IS_TEXT_BLOCK(parent))
  {
    MbTextBlock *_parent = MB_TEXT_BLOCK(parent);
    GtkWidget *first_child = get_first_child(_self);
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
    GtkWidget *last_child = get_last_child(_self);
    while(last_child != NULL)
    {
      MbTextBlock *_last_child = MB_TEXT_BLOCK(last_child);
      g_object_ref(last_child);
      remove_child(_self, _last_child);
      mb_root_text_block_insert_child_after(_parent, _last_child, _self);
      g_object_unref(last_child);
      last_child = get_last_child(_self);
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
      _last_child = MB_TEXT_BLOCK(get_last_child(_last_child));
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
