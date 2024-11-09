#include "./mb_window_left_sidebar.h"
#define DEFAULT_WIDTH 32
/* WIDGET DEFINITION */
struct _MbWindowLeftSidebar
{
  GtkWidget parent;
  /* WIDGETS */
  GtkWidget *list_box;
  /* EVENT LISTENERS */
  /* PROPERTIES */
};
G_DEFINE_TYPE(MbWindowLeftSidebar, mb_window_left_sidebar, GTK_TYPE_WIDGET)
/* FORWARD DECLARATION */
static void mb_window_left_sidebar_dispose(GObject *object);
static void mb_window_left_sidebar_finalize(GObject *object);
/* CALLBACK */
/* PROPERTIES */
/* SIGNALS */
/* WIDGET LIFECYCLE */
static void mb_window_left_sidebar_init(MbWindowLeftSidebar *_self)
{
  GtkWidget *self = GTK_WIDGET(_self);
  /* INSTANTIATE WIDGETS */
  _self->list_box = gtk_list_box_new();
  /* CONFIGURE WIDGETS */
  gtk_widget_set_parent(_self->list_box, self);

  gtk_widget_set_size_request(_self->list_box, DEFAULT_WIDTH, -1);
  gtk_widget_set_vexpand(_self->list_box, TRUE);
  /* CONNECT TO SIGNALS */
}
static void mb_window_left_sidebar_class_init(MbWindowLeftSidebarClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  /* Map virtual functions */
  object_class->dispose = mb_window_left_sidebar_dispose;
  object_class->finalize = mb_window_left_sidebar_finalize;
  /* Properties */
  /* Signals */
  /* Layout manager */
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);
}
static void mb_window_left_sidebar_dispose(GObject *object) 
{
  MbWindowLeftSidebar *_self = MB_WINDOW_LEFT_SIDEBAR(object);
  g_clear_pointer(&_self->list_box, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_window_left_sidebar_parent_class)->dispose(object); 
}
static void mb_window_left_sidebar_finalize(GObject *object) 
{
  G_OBJECT_CLASS(mb_window_left_sidebar_parent_class)->finalize(object);
}
/* Public implementation */
GtkWidget* mb_window_left_sidebar_new()
{
  return g_object_new(MB_TYPE_WINDOW_LEFT_SIDEBAR, NULL);
}
/* Private implementation */
