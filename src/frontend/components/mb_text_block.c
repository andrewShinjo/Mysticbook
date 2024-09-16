#include "./mb_text_block.h"

struct _MbTextBlock
{
  GtkTextView parent;
  GtkEventController *key_controller;
};

G_DEFINE_TYPE(MbTextBlock, mb_text_block, GTK_TYPE_TEXT_VIEW)

// Private

gboolean root_key_pressed(
  GtkEventControllerKey *self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
)
{
  if(keyval == GDK_KEY_Return)
  {
    return TRUE;
  }
  return FALSE;
}

static void mb_text_block_dispose(GObject *object) {}
static void mb_text_block_finalize(GObject *object) {}

static void mb_text_block_init(MbTextBlock *self) 
{
  self->key_controller = gtk_event_controller_key_new();
  gtk_widget_add_controller(GTK_WIDGET(self), self->key_controller);
  g_signal_connect(
    self->key_controller,
    "key-pressed",
    G_CALLBACK(root_key_pressed),
    NULL
  );
}

static void mb_text_block_class_init(MbTextBlockClass *klass) {}

// Public

GtkWidget *mb_text_block_new()
{
  return g_object_new(MB_TYPE_TEXT_BLOCK, NULL);
}
