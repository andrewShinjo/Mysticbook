#include "./mb_window_left_sidebar_button.h"
/* Widget definition */
struct _MbWindowLeftSidebarButton
{
  GtkWidget parent;
  /* Widgets */
  GtkWidget *button;
  GtkWidget *image;
  GtkWidget *popover;
  GtkWidget *popover_label;
  /* Event listeners */
  GtkEventController *event;
  /* Properties */
  gchar *image_path;
  gchar *popover_message;
};
G_DEFINE_TYPE(MbWindowLeftSidebarButton, mb_window_left_sidebar_button, GTK_TYPE_WIDGET)
/* Forward declaration */
static void mb_window_left_sidebar_button_dispose(GObject *object);
static void mb_window_left_sidebar_button_finalize(GObject *object);
/* Callbacks */
static void on_button_clicked(GtkButton *_button, gpointer user_data)
{
  g_print("Button clicked\n");
}
static void on_mouse_enter(GtkEventController *controller, gpointer user_data) 
{
  MbWindowLeftSidebarButton *_self = MB_WINDOW_LEFT_SIDEBAR_BUTTON(user_data);
  GtkPopover *_popover = GTK_POPOVER(_self->popover);
  gtk_popover_popup(_popover);
}
static void on_mouse_leave(GtkEventController *controller, gpointer user_data)
{
  MbWindowLeftSidebarButton *_self = MB_WINDOW_LEFT_SIDEBAR_BUTTON(user_data);
  GtkPopover *_popover = GTK_POPOVER(_self->popover);
  gtk_popover_popdown(_popover);
}
/* Properties */
enum property_types
{
  PROP_IMAGE_PATH = 1,
  PROP_POPOVER_MESSAGE,
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
    case PROP_POPOVER_MESSAGE:
    {
      g_value_set_string(value, _self->popover_message);
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
    case PROP_POPOVER_MESSAGE:
    {
      GtkLabel *_popover_label = GTK_LABEL(_self->popover_label);
      g_free(_self->popover_message);
      _self->popover_message = g_value_dup_string(value);
      gtk_label_set_label(_popover_label, _self->popover_message);
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
  _self->popover = gtk_popover_new();
  _self->popover_label = gtk_label_new(NULL);
  _self->event = gtk_event_controller_motion_new();
  /* Configure widgets */
  GtkButton *_button = GTK_BUTTON(_self->button);
  GtkPopover *_popover = GTK_POPOVER(_self->popover);
  gtk_button_set_child(_button, _self->image);
  gtk_widget_set_parent(_self->popover, _self->button);
  gtk_widget_set_parent(_self->button, self);
  gtk_popover_set_autohide(_popover, FALSE);
  gtk_popover_set_child(_popover, _self->popover_label);
  gtk_popover_set_has_arrow(_popover, FALSE);
  gtk_popover_set_position(_popover, GTK_POS_RIGHT);
  /* Connect to signals */
  gtk_widget_add_controller(_self->button, _self->event);
  g_signal_connect(_self->button, "clicked", G_CALLBACK(on_button_clicked), NULL);
  g_signal_connect(_self->event, "enter", G_CALLBACK(on_mouse_enter), _self);
  g_signal_connect(_self->event, "leave", G_CALLBACK(on_mouse_leave), _self);
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
  properties[PROP_POPOVER_MESSAGE] = g_param_spec_string(
    "popover_message", "popover_message", "popover_message", NULL, G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, properties);
  /* Signals */
  /* Layout manager */
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);
}
static void mb_window_left_sidebar_button_dispose(GObject *object)
{
  MbWindowLeftSidebarButton *_self = MB_WINDOW_LEFT_SIDEBAR_BUTTON(object);
  g_clear_pointer(&_self->popover, gtk_widget_unparent);
  g_clear_pointer(&_self->button, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_window_left_sidebar_button_parent_class)->dispose(object);
}
static void mb_window_left_sidebar_button_finalize(GObject *object)
{
  G_OBJECT_CLASS(mb_window_left_sidebar_button_parent_class)->finalize(object);
}
/* Public implementation */
GtkWidget* mb_window_left_sidebar_button_new(gchar *image_path, gchar *popover_message)
{
  return g_object_new(MB_TYPE_WINDOW_LEFT_SIDEBAR_BUTTON, "image_path", image_path, "popover_message", popover_message, NULL);
}
/* Private implementation */
