#include "./mb_link_popover.h"
#include "./mb_text_buffer.h"
#include "./mb_text_view.h"
#include "../../backend/controller/block_controller.h"
/* Global variables */
gboolean link_popover_on = FALSE;
/* WIDGET DEFINITION */
struct _MbTextView
{
  GtkWidget parent;
  /* WIDGETS */
  GtkWidget *text_view;
  GtkWidget *link_popover;
  GtkWidget *list_box;
  /* EVENT LISTENERS */
  /* PROPERTIES */
  gint64 id;
  gboolean is_root;
};
G_DEFINE_TYPE(MbTextView, mb_text_view, GTK_TYPE_WIDGET)
/* FORWARD DECLARATION */
static void mb_text_view_dispose(GObject *object);
static void mb_text_view_finalize(GObject *object);
/* CALLBACK */
static void insert_text(GtkTextBuffer *tb, const GtkTextIter* location, gchar* text, gint len, gpointer user_data)
{
  MbTextView *_self = MB_TEXT_VIEW(user_data);
  gboolean is_left_square_bracket = g_strcmp0(text, "[") == 0;

  if(link_popover_on)
  {
    // Search for first char.
    block_controller_get_10_best_matching_blocks("Untitled");
  }
  else if(!link_popover_on)
  {
    if(is_left_square_bracket)
    {
      GtkTextIter previous_iter = *location;
      gtk_text_iter_backward_char(&previous_iter);
      if(gtk_text_iter_equal(&previous_iter, location))
      {
        return;
      }
      gunichar previous_char = gtk_text_iter_get_char(&previous_iter);
      if(previous_char == '[')
      {
        GtkPopover *_link_popover = GTK_POPOVER(_self->link_popover);
        GtkTextView *_text_view = GTK_TEXT_VIEW(_self->text_view);
        GdkRectangle iter_location;
        gtk_text_view_get_iter_location(_text_view, location, &iter_location);
        gtk_popover_set_pointing_to(_link_popover, &iter_location);
        gtk_popover_popup(_link_popover);
        link_popover_on = TRUE;
      }
    }
  }
}
static gboolean key_pressed(GtkEventControllerKey *key, guint keyval, guint keycode, GdkModifierType state, gpointer user_data)
{
  MbTextView *_self = MB_TEXT_VIEW(user_data);
  if(_self->is_root)
  {

  }
  else if(!_self->is_root)
  {

  }
}
static void notify_id(GObject *object, GParamSpec *pspec, gpointer user_data)
{
  MbTextView *_self = MB_TEXT_VIEW(object);
  GtkTextView *text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *buffer = mb_text_buffer_new(_self->id);
  gtk_text_view_set_buffer(text_view, buffer);
}
static void notify_is_root(GObject *object, GParamSpec *pspec, gpointer user_data)
{

}
/* PROPERTIES */
enum property_types
{
  PROP_ID = 1,
  PROP_IS_ROOT,
  N_PROPERTIES
};
static GParamSpec *properties[N_PROPERTIES];
static void get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  MbTextView *_self = MB_TEXT_VIEW(object);
  switch(property_id)
  {
    case PROP_ID:
    {
      g_value_set_int64(value, _self->id);
      break;
    }
    case PROP_IS_ROOT:
    {
      g_value_set_boolean(value, _self->is_root);
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
  MbTextView *_self = MB_TEXT_VIEW(object);
  switch(property_id)
  {
    case PROP_ID:
    {
      _self->id = g_value_get_int64(value);
      break;
    }
    case PROP_IS_ROOT:
    {
      _self->is_root = g_value_get_boolean(value);
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
static void mb_text_view_init(MbTextView *_self)
{
  GtkWidget *self = GTK_WIDGET(_self);
  /* INSTANTIATE WIDGETS */
  _self->text_view = gtk_text_view_new();
  _self->link_popover = mb_link_popover_new();
  /* CONFIGURE WIDGETS */
  gtk_widget_allocate(_self->text_view, 0, 0, 0, NULL);
  gtk_widget_set_hexpand(_self->text_view, TRUE);
  gtk_widget_set_parent(_self->link_popover, _self->text_view);
  gtk_widget_set_parent(_self->text_view, self);
  /* CONNECT TO SIGNALS */
  GtkTextView *_text_view = GTK_TEXT_VIEW(_self->text_view);
  GtkTextBuffer *_text_buffer = gtk_text_view_get_buffer(_text_view);
  g_signal_connect(_text_buffer, "insert-text", G_CALLBACK(insert_text), _self);
  g_signal_connect(self, "notify::id", G_CALLBACK(notify_id), self);
}
static void mb_text_view_class_init(MbTextViewClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  /* MAP VIRTUAL FUNCTIONS */
  object_class->dispose = mb_text_view_dispose;
  object_class->finalize = mb_text_view_finalize;
  object_class->get_property = get_property;
  object_class->set_property = set_property;
  /* PROPERTIES */
  properties[PROP_ID] = g_param_spec_int64("id", "id", "id", 0, G_MAXINT64, 0, G_PARAM_READWRITE);
  properties[PROP_IS_ROOT] = g_param_spec_boolean("is_root", "is_root", "is_root", FALSE, G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, properties);
  /* SIGNALS */
  /* LAYOUT MANAGER */
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);
}
static void mb_text_view_dispose(GObject *object) 
{
  MbTextView *_self = MB_TEXT_VIEW(object);
  g_clear_pointer(&_self->link_popover, gtk_widget_unparent);
  g_clear_pointer(&_self->text_view, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_text_view_parent_class)->dispose(object);
}
static void mb_text_view_finalize(GObject *object) 
{
  G_OBJECT_CLASS(mb_text_view_parent_class)->finalize(object);
}
/* PUBLIC IMPLEMENTATION */
GtkWidget *mb_text_view_new()
{
  return g_object_new(MB_TYPE_TEXT_VIEW, NULL);
}
/* PRIVATE IMPLEMENTATION */
