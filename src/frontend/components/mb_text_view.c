#include "./mb_link_popover.h"
#include "./mb_text_view.h"
/* WIDGET DEFINITION */
struct _MbTextView
{
  GtkWidget parent;
  /* WIDGETS */
  GtkWidget *text_view;
  GtkWidget *link_popover;
  /* EVENT LISTENERS */
  /* PROPERTIES */
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
      MbLinkPopover *_link_popover = MB_LINK_POPOVER(_self->link_popover);
      mb_link_popover_popup(_link_popover);
    }
  }
}
/* PROPERTIES */
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
}
static void mb_text_view_class_init(MbTextViewClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  /* MAP VIRTUAL FUNCTIONS */
  object_class->dispose = mb_text_view_dispose;
  object_class->finalize = mb_text_view_finalize;
  /* PROPERTIES */
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
