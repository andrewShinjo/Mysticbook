#include "./mb_block_view_page.h"
#include "../components/mb_text_block.h"

static void mb_block_view_page_dispose(GObject *object);
static void mb_block_view_page_finalize(GObject *object);

struct _MbBlockViewPage
{
	GtkWidget parent;
  GtkWidget *vbox;
  GtkWidget *main_text_view;
  GtkEventController *key_controller;
  GtkWidget *children_box;

  GtkWidget *hbox;
  GtkWidget *expander;
  GtkWidget *child_tv;
};

G_DEFINE_TYPE(MbBlockViewPage, mb_block_view_page, GTK_TYPE_WIDGET)


// *********************************************************************
// * Signals
// *********************************************************************

gboolean child_tv_key_pressed(
  GtkEventControllerKey *self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
)
{}

gboolean
main_tv_key_pressed(
  GtkEventControllerKey* self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
)
{

  if(keyval == GDK_KEY_Return)
  {
    GtkBox *children_box = GTK_BOX(user_data);
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *expander = gtk_expander_new(NULL);
    GtkWidget *textview = gtk_text_view_new();
    gtk_widget_set_hexpand(textview, TRUE);
    gtk_box_append(GTK_BOX(hbox), expander);
    gtk_box_append(GTK_BOX(hbox), textview);
    gtk_box_prepend(children_box, hbox);
    gtk_widget_allocate(textview, 0, 0, 0, NULL);
    gtk_widget_grab_focus(textview);
    g_print("Return pressed.\n");
    return TRUE;
  }

  return FALSE;
}

// *********************************************************************
// * Widget lifecycle
// *********************************************************************

static void mb_block_view_page_dispose(GObject *object) 
{
  MbBlockViewPage *self = MB_BLOCK_VIEW_PAGE(object);
  g_clear_pointer(&self->vbox, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->dispose(object);
}

static void mb_block_view_page_finalize(GObject *object)
{
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->finalize(object);
}

static void mb_block_view_page_init(MbBlockViewPage *self)
{
  self->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);  
  self->main_text_view = gtk_text_view_new();
  self->key_controller = gtk_event_controller_key_new();
  self->children_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_add_controller(self->main_text_view, self->key_controller);
  g_signal_connect(
    self->key_controller, 
    "key-pressed", 
    G_CALLBACK(main_tv_key_pressed), 
    self->children_box
  );
  gtk_widget_set_name(self->main_text_view, "main-text-view");

  /*
  self->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  self->expander = gtk_expander_new(NULL);
  self->child_tv = gtk_text_view_new();

  gtk_box_append(GTK_BOX(self->hbox), self->expander);
  gtk_box_append(GTK_BOX(self->hbox), self->child_tv);
  gtk_widget_set_hexpand(self->child_tv, TRUE);
  */

  gtk_box_append(GTK_BOX(self->vbox), self->main_text_view);
  gtk_box_append(GTK_BOX(self->vbox), self->children_box);
  //gtk_box_append(GTK_BOX(self->vbox), self->hbox);
  gtk_widget_set_hexpand(self->vbox, TRUE);
  gtk_widget_set_vexpand(self->vbox, TRUE);

  gtk_widget_set_parent(self->vbox, GTK_WIDGET(self));
}
static void mb_block_view_page_class_init(MbBlockViewPageClass *klass) 
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = mb_block_view_page_dispose;
  object_class->finalize = mb_block_view_page_finalize;
  gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

// *********************************************************************
// * Public
// *********************************************************************

GtkWidget *
mb_block_view_page_new()
{
	return g_object_new(MB_TYPE_BLOCK_VIEW_PAGE, NULL);
}
