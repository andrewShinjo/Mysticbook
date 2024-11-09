#include "./mb_text_view.h"
/* WIDGET DEFINITION */
struct _MbTextView
{
  GtkWidget parent;
  /* WIDGETS */
  GtkWidget *text_view;
  /* EVENT LISTENERS */
  /* PROPERTIES */
};
G_DEFINE_TYPE(MbTextView, mb_text_view, GTK_TYPE_WIDGET)
/* FORWARD DECLARATION */
static void mb_text_view_dispose(GObject *object);
static void mb_text_view_finalize(GObject *object);
/* CALLBACK */
/* PROPERTIES */
/* SIGNALS */ 
/* WIDGET LIFECYCLE */
static void mb_text_view_init(MbTextView *_self)
{
  /* INSTANTIATE WIDGETS */
  _self->text_view = gtk_text_view_new();
  /* CONFIGURE WIDGETS */
  /* CONNECT TO SIGNALS */
}
static void mb_text_view_class_init(MbTextViewClass *klass)
{
  /* MAP VIRTUAL FUNCTIONS */
  /* PROPERTIES */
  /* SIGNALS */
  /* LAYOUT MANAGER */
}
static void mb_text_view_dispose(GObject *object) {}
static void mb_text_view_finalize(GObject *object) {}
/* PUBLIC IMPLEMENTATION */
GtkWidget *mb_text_view_new()
{
  return g_object_new(MB_TYPE_TEXT_VIEW, NULL);
}
/* PRIVATE IMPLEMENTATION */
