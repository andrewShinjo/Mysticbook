#include "./mb_link_popover.h"
/* Widget definition */
struct _MbLinkPopover
{
  GtkWidget popover;
  /* Widgets */
  /* Event listeners */
  /* Properties */
};
G_DEFINE_TYPE(MbLinkPopover, mb_link_popover, GTK_TYPE_POPOVER)
/* Forward declaration */
static void mb_link_popover_dispose(GObject *object);
static void mb_link_popover_finalize(GObject *object);
/* Callbacks */
/* Properties */
/* Signals */
/* Widget lifecycle */
static void mb_link_popover_init(MbLinkPopover *_self)
{
  /* Instantiate widgets */
  /* Configure widgets */
  GtkWidget *self = GTK_WIDGET(_self);
  GtkPopover *_popover = GTK_POPOVER(_self);
  gtk_popover_set_autohide(_popover, FALSE);
  /* Connect to signals */
}
static void mb_link_popover_class_init(MbLinkPopoverClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  /* Map virtual functions */
  object_class->dispose = mb_link_popover_dispose;
  object_class->finalize = mb_link_popover_finalize;
  /* Properties */
  /* Signals */
  /* Layout manager */
}
static void mb_link_popover_dispose(GObject *object)
{
  G_OBJECT_CLASS(mb_link_popover_parent_class)->dispose(object);
}
static void mb_link_popover_finalize(GObject *object)
{
  G_OBJECT_CLASS(mb_link_popover_parent_class)->finalize(object);
}
/* Public implementation */
GtkWidget *mb_link_popover_new()
{
  return g_object_new(MB_TYPE_LINK_POPOVER, NULL);
}
void mb_link_popover_popup(MbLinkPopover *_self)
{
  GtkPopover *_popover = GTK_POPOVER(_self);  
  gtk_popover_popup(_popover);
}
void mb_link_popover_popdown(MbLinkPopover *_self)
{
  GtkPopover *_popover = GTK_POPOVER(_self);  
  gtk_popover_popdown(_popover);
}
