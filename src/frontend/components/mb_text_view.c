#include "./mb_text_view.h"

struct _MbTextView
{
  GtkTextView parent;
};

G_DEFINE_TYPE(MbTextView, mb_text_view, GTK_TYPE_TEXT_VIEW)

static void mb_text_view_dispose(GObject *object) {}
static void mb_text_view_finalize(GObject *object) {}
static void mb_text_view_init(MbTextView *self) {}
static void mb_text_view_class_init(MbTextViewClass *klass) {}

GtkWidget *mb_text_view_new()
{
  return g_object_new(MB_TYPE_TEXT_VIEW, NULL);
}
