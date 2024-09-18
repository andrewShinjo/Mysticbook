#include "./mb_text_block.h"

struct _MbTextBlock
{
  GtkWidget parent;
  GtkWidget *layout;
  GtkWidget *expander;
  GtkWidget *bullet_point;
  GtkWidget *text_view;
  GtkEventController *key_controller;
};

G_DEFINE_TYPE(MbTextBlock, mb_text_block, GTK_TYPE_WIDGET)

// * Private

// ** Callback

gboolean
_key_pressed(
  GtkEventControllerKey *self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
)
{

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
  self->layout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  self->expander = gtk_expander_new(NULL);
  self->bullet_point = gtk_label_new("•");
  self->text_view = gtk_text_view_new();
  self->key_controller = gtk_event_controller_key_new();

  gtk_widget_set_hexpand(self->layout, TRUE);
  gtk_widget_set_vexpand(self->layout, TRUE);

  gtk_widget_allocate(self->text_view, 0, 0, 0, NULL);
  gtk_widget_set_hexpand(self->text_view, TRUE);

  gtk_box_append(GTK_BOX(self->layout), self->expander);
  gtk_box_append(GTK_BOX(self->layout), self->bullet_point);
  gtk_box_append(GTK_BOX(self->layout), self->text_view);

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

GtkWidget *mb_text_block_new()
{
  return g_object_new(MB_TYPE_TEXT_BLOCK, NULL);
}

void
mb_text_block_focus(MbTextBlock *self)
{
  gtk_widget_grab_focus(self->text_view);
}
