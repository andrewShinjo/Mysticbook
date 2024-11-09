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
};
G_DEFINE_TYPE(MbWindowLeftSidebarButton, mb_window_left_sidebar_button, GTK_TYPE_WIDGET)
/* Forward declaration */
static void mb_window_left_sidebar_button_dispose(GObject *object);
static void mb_window_left_sidebar_button_finalize(GObject *object);
/* Callbacks */
/* Properties */
/* Signals */
/* Widget lifecycle */
static void mb_window_left_sidebar_button_init(MbWindowLeftSidebarButton *_self)
{
  GtkWidget *self = GTK_WIDGET(_self);
  /* Instantiate widgets */
  /* Configure widgets */
  /* Connect to signals */
}
static void mb_window_left_sidebar_button_class_init(MbWindowLeftSidebarButtonClass *klass)
{
  /* Map virtual functions */
  /* Properties */
  /* Signals */
  /* Layout manager */
}
static void mb_window_left_sidebar_button_dispose(GObject *object)
{

}
static void mb_window_left_sidebar_button_finalize(GObject *object)
{

}
/* Public implementation */
/* Private implementation */
