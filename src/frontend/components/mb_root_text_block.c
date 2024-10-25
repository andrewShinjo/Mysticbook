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
};

G_DEFINE_TYPE(MbRootTextBlock, mb_root_text_block, GTK_TYPE_WIDGET)

/* FORWARD DECLARATION */
static void dispose(GObject *object);
static void finalize(GObject *object);
static gint64 get_id(MbRootTextBlock* _self);

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
    gint64 parent_id = get_id(_self);
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

/* WIDGET LIFECYCLE */

static void mb_root_text_block_init(MbRootTextBlock *self) 
{
  /* INSTANTIATE WIDGETS */
  self->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  self->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  self->text_view = gtk_text_view_new();
  self->children_blocks = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  self->key_controller = gtk_event_controller_key_new();
  /* CONFIGURE WIDGETS */
  gtk_box_append(GTK_BOX(self->hbox), self->text_view);
  gtk_box_append(GTK_BOX(self->layout), self->hbox);
  gtk_box_append(GTK_BOX(self->layout), self->children_blocks);
  gtk_widget_set_hexpand(self->layout, TRUE);
  gtk_widget_set_vexpand(self->layout, TRUE);
  gtk_widget_set_hexpand(self->text_view, TRUE);
  gtk_widget_set_parent(self->layout, GTK_WIDGET(self));
  /** GET CHILDREN BLOCKS **/
  /* CONNECT TO SIGNALS */
  gtk_widget_add_controller(self->text_view, self->key_controller);
  g_signal_connect(
    self->key_controller, 
    "key-pressed", 
    G_CALLBACK(key_pressed), 
    self
  );
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(
    GTK_TEXT_VIEW(self->text_view)
  );
  g_signal_connect(
    text_buffer, 
    "changed", 
    G_CALLBACK(changed), 
    self
  );
}

static void mb_root_text_block_class_init(MbRootTextBlockClass *klass) 
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  /* MAP VIRTUAL FUNCTIONS */
  object_class->dispose = dispose;
  object_class->finalize = finalize;
  /* PROPERTY */
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

static void finalize(GObject *object) {}

/* PROPERTIES */
/* SIGNALS */
/* PUBLIC IMPLEMENTATION */

void
mb_root_text_block_append_content(
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

GtkWidget*
mb_root_text_block_new()
{
  return g_object_new(MB_TYPE_ROOT_TEXT_BLOCK, NULL);
}

void
mb_root_text_block_grab_focus(MbRootTextBlock *self)
{
  gtk_widget_grab_focus(self->text_view);
}

void
mb_root_text_block_remove_child(
  MbRootTextBlock *_self, 
  MbTextBlock *_child
)
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

void mb_root_text_block_set_content(
  MbRootTextBlock *_self,
  const gchar *content
)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
  gtk_text_buffer_set_text(text_buffer, content, -1);
}

/* PRIVATE IMPLEMENTATION */

static gint64 get_id(MbRootTextBlock* _self)
{
  GtkWidget *self = GTK_WIDGET(_self);
  GtkWidget *ancestor = gtk_widget_get_ancestor(
    self, 
    MB_TYPE_BLOCK_VIEW_PAGE
  );
  MbBlockViewPage *block_view_page = MB_BLOCK_VIEW_PAGE(ancestor);
  gint64 id;
  g_object_get(block_view_page, "id", &id, NULL);
  return id; 
}
