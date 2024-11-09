#include "./mb_window_left_sidebar_button.h"
/* Widget definition */
struct _MbWindowLeftSidebarButton
{
  GtkWidget parent;
  /* Widgets */
  GtkWidget *button;
  GtkWidget *image;
  GtkWidget *popover;
  /* Event listeners */
  /* Properties */
  gchar *image_path;
};
G_DEFINE_TYPE(MbWindowLeftSidebarButton, mb_window_left_sidebar_button, GTK_TYPE_WIDGET)
/* Forward declaration */
static void mb_window_left_sidebar_button_dispose(GObject *object);
static void mb_window_left_sidebar_button_finalize(GObject *object);
/* Callbacks */
/* Properties */
enum property_types
{
  PROP_IMAGE_PATH = 1,
  N_PROPERTIES
};
static GParamSpec *properties[N_PROPERTIES];
static void mb_window_left_sidebar_button_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  MbWindowLeftSidebarButton *_self = MB_WINDOW_LEFT_SIDEBAR_BUTTON(object);
  switch(property_id)
  {
    case PROP_IMAGE_PATH:
    {
      g_value_set_string(value, _self->image_path);
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}
static void mb_window_left_sidebar_button_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  MbWindowLeftSidebarButton *_self = MB_WINDOW_LEFT_SIDEBAR_BUTTON(object);
  switch(property_id)
  {
    case PROP_IMAGE_PATH:
    {
      g_free(_self->image_path);
      _self->image_path = g_value_dup_string(value); 
      if(_self->image != NULL)
      {
        GtkImage *_image = GTK_IMAGE(_self->image);
        gtk_image_clear(_image);
        gtk_image_set_from_file(_image, _self->image_path);
      }
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}
/* Signals */
/* Widget lifecycle */
static void mb_window_left_sidebar_button_init(MbWindowLeftSidebarButton *_self)
{
  GtkWidget *self = GTK_WIDGET(_self);
  /* Instantiate widgets */
  _self->button = gtk_button_new();
  _self->image = gtk_image_new();
  /* Configure widgets */
  GtkButton *_button = GTK_BUTTON(_self->button);
  gtk_button_set_child(_button, _self->image);
  gtk_widget_set_parent(_self->button, self);
  /* Connect to signals */
}
static void mb_window_left_sidebar_button_class_init(MbWindowLeftSidebarButtonClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  /* Map virtual functions */
  object_class->dispose = mb_window_left_sidebar_button_dispose;
  object_class->finalize = mb_window_left_sidebar_button_finalize;
  object_class->get_property = mb_window_left_sidebar_button_get_property;
  object_class->set_property = mb_window_left_sidebar_button_set_property;
  /* Properties */
  properties[PROP_IMAGE_PATH] = g_param_spec_string("image_path", "image_path", "image_path", NULL, G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, properties);
  /* Signals */
  /* Layout manager */
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);
}
static void mb_window_left_sidebar_button_dispose(GObject *object)
{
  MbWindowLeftSidebarButton *_self = MB_WINDOW_LEFT_SIDEBAR_BUTTON(object);
  g_clear_pointer(&_self->button, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_window_left_sidebar_button_parent_class)->dispose(object);
}
static void mb_window_left_sidebar_button_finalize(GObject *object)
{
  G_OBJECT_CLASS(mb_window_left_sidebar_button_parent_class)->finalize(object);
}
/* Public implementation */
GtkWidget* mb_window_left_sidebar_button_new(gchar *image_path)
{
  return g_object_new(MB_TYPE_WINDOW_LEFT_SIDEBAR_BUTTON, "image_path", image_path, NULL);
}
/* Private implementation */
