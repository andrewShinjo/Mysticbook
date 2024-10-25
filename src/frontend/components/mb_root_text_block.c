#include "./mb_root_text_block.h"
#include "./mb_text_block.h"
#include "../pages/mb_block_view_page.h"
#include "../../backend/block.h"
#include "../../backend/util.h"
/* WIDGET DEFINITION */
struct _MbRootTextBlock
{
  GtkWidget parent;
  /* WIDGETS */
  GtkWidget *layout;
  GtkWidget *hbox;
  GtkWidget *text_view;
  GtkWidget *children_blocks;
  /* EVENT LISTENERS */
  GtkEventController *key_controller;
  /* PROPERTIES */
  gint64 id;
};
G_DEFINE_TYPE(MbRootTextBlock, mb_root_text_block, GTK_TYPE_WIDGET)
/* FORWARD DECLARATION */
static void dispose(GObject *object);
static void finalize(GObject *object);
static void get_property(
  GObject *object,
  guint property_id,
  GValue *value,
  GParamSpec *pspec
);
static void set_property(
  GObject *object,
  guint property_id,
  const GValue *value,
  GParamSpec *pspec
);
/* CALLBACK */
static void changed(GtkTextBuffer *text_buffer, gpointer user_data)
{
  MbRootTextBlock *_self = MB_ROOT_TEXT_BLOCK(user_data);
  GtkWidget *self = GTK_WIDGET(user_data);
  GtkWidget *ancestor = gtk_widget_get_ancestor(
    self, 
    MB_TYPE_BLOCK_VIEW_PAGE
  );
  MbBlockViewPage *block_view_page = MB_BLOCK_VIEW_PAGE(ancestor);
  gint64 id;
  g_object_get(block_view_page, "id", &id, NULL);
  // Update block's content in SQL.
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  gchar *content = gtk_text_buffer_get_text(
    text_buffer, 
    &start, 
    &end, 
    FALSE
  );
  block_update_content(id, content);
  g_free(content);
}
static gboolean key_pressed(
  GtkEventControllerKey *key, 
  guint keyval, 
  guint keycode, 
  GdkModifierType state, 
  gpointer user_data
)
{
  MbRootTextBlock *_self = MB_ROOT_TEXT_BLOCK(user_data);
  GtkWidget *self = GTK_WIDGET(user_data);

  if(keyval == GDK_KEY_Return)
  {
    block_increment_all_position();
    // Create new block in GUI.
    GtkWidget *child = mb_text_block_new();
    gtk_box_prepend(GTK_BOX(_self->children_blocks), child);
    mb_text_block_grab_focus(MB_TEXT_BLOCK(child));
    // Create new block in SQL.
    gint64 creation_time = get_current_timestamp();
    gint64 is_document = 0;
    gint64 modification_time = creation_time;
    gint64 parent_id = _self->id;
    gint64 position = 1;
    gchar *content = "";
    block_new_all_fields(
      &creation_time, 
      &is_document, 
      NULL, 
      &position, 
      &parent_id, 
      content
    );
    return TRUE;
  }
  return FALSE;
}
static void 
notify_id(GObject *object, GParamSpec *pspec, gpointer user_data)
{
  MbRootTextBlock *_self = MB_ROOT_TEXT_BLOCK(object);

  // Get block content.
  const gchar *content = block_get_content(_self->id);
  if(content != NULL)
  {
    mb_root_text_block_set_content(_self, content);
  }
  // Get list of children ids.
  GArray *children_ids = block_get_all_children_ids(_self->id);
  for(guint i = 0; i < children_ids->len; i++)
  {
    gint64 child_id = g_array_index(children_ids, gint64, i);
    GtkWidget *child_block = mb_text_block_new(child_id);
    gtk_box_append(GTK_BOX(_self->children_blocks), child_block);
  }
}
/* PROPERTIES */
enum property_types
{
  PROP_ID = 1,
  N_PROPERTIES
};
static GParamSpec *properties[N_PROPERTIES];
static void get_property(
  GObject *object,
  guint property_id,
  GValue *value,
  GParamSpec *pspec
)
{
  MbRootTextBlock *_self = MB_ROOT_TEXT_BLOCK(object);
  switch(property_id)
  {
    case PROP_ID:
    {
      _self->id = g_value_get_int64(value);
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}
static void set_property(
  GObject *object,
  guint property_id,
  const GValue *value,
  GParamSpec *pspec
)
{
  MbRootTextBlock *_self = MB_ROOT_TEXT_BLOCK(object);
  switch(property_id)
  {
    case PROP_ID:
    {
      _self->id = g_value_get_int64(value);
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}

/* WIDGET LIFECYCLE */
static void mb_root_text_block_init(MbRootTextBlock *_self) 
{
  GtkWidget *self = GTK_WIDGET(_self);
  g_print("mb_root_text_block_init\n");
  /* INSTANTIATE WIDGETS */
  _self->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  _self->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  _self->text_view = gtk_text_view_new();
  _self->children_blocks = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  _self->key_controller = gtk_event_controller_key_new();
  /* CONFIGURE WIDGETS */
  gtk_box_append(GTK_BOX(_self->hbox), _self->text_view);
  gtk_box_append(GTK_BOX(_self->layout), _self->hbox);
  gtk_box_append(GTK_BOX(_self->layout), _self->children_blocks);
  gtk_widget_set_hexpand(_self->layout, TRUE);
  gtk_widget_set_vexpand(_self->layout, TRUE);
  gtk_widget_set_hexpand(_self->text_view, TRUE);
  gtk_widget_set_parent(_self->layout, self);
  /** GET CHILDREN BLOCKS **/
  g_print("mb_root_text_block: get_all_children_ids\n");
  /* CONNECT TO SIGNALS */
  g_signal_connect(self, "notify::id", G_CALLBACK(notify_id), NULL);
  gtk_widget_add_controller(_self->text_view, _self->key_controller);
  g_signal_connect(
    _self->key_controller, 
    "key-pressed", 
    G_CALLBACK(key_pressed), 
    _self
  );
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(
    GTK_TEXT_VIEW(_self->text_view)
  );
  g_signal_connect(
    text_buffer, 
    "changed", 
    G_CALLBACK(changed), 
    _self
  );
}
static void mb_root_text_block_class_init(MbRootTextBlockClass *klass) 
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  /* MAP VIRTUAL FUNCTIONS */
  object_class->dispose = dispose;
  object_class->finalize = finalize;
  object_class->get_property = get_property;
  object_class->set_property = set_property;
  /* PROPERTY */
  properties[PROP_ID] = g_param_spec_int64(
    "id",
    "id",
    "id",
    0,
    G_MAXINT64,
    0,
    G_PARAM_READWRITE
  );
  g_object_class_install_properties(
    object_class,
    N_PROPERTIES,
    properties
  );
  /* SIGNAL */
  /* LAYOUT MANAGER */
  gtk_widget_class_set_layout_manager_type(
    GTK_WIDGET_CLASS(klass), 
    GTK_TYPE_BOX_LAYOUT
  );
}
static void dispose(GObject *object) 
{
  MbRootTextBlock *self = MB_ROOT_TEXT_BLOCK(object);
  g_clear_pointer(&self->layout, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_root_text_block_parent_class)->dispose(object);
}
static void finalize(GObject *object) 
{

}
/* SIGNALS */
/* PUBLIC IMPLEMENTATION */
void mb_root_text_block_append_content(
  MbRootTextBlock *_self, 
  gchar *content
)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  gtk_text_buffer_insert(text_buffer, &end, content, -1);
}

void mb_root_text_block_insert_child_after(
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

GtkWidget* mb_root_text_block_new(gint64 id)
{
  return g_object_new(
    MB_TYPE_ROOT_TEXT_BLOCK, 
    "id",
    id,
    NULL
  );
}

void mb_root_text_block_grab_focus(MbRootTextBlock *self)
{
  gtk_widget_grab_focus(self->text_view);
}

void mb_root_text_block_remove_child(
  MbRootTextBlock *_self, 
  MbTextBlock *_child
)
{
  GtkBox *_children_blocks = GTK_BOX(_self->children_blocks);
  GtkWidget *child = GTK_WIDGET(_child);

  if(_self->children_blocks != gtk_widget_get_parent(child))
  {
    return;
  }
  gtk_box_remove(_children_blocks, child);
}

void 
mb_root_text_block_set_content(
  MbRootTextBlock *_self,
  const gchar *content
)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  gtk_text_buffer_set_text(text_buffer, content, -1);
}
