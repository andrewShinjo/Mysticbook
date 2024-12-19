#include "./mb_text_view2.h"

/* Private definition */

static void mb_text_view2_class_init(MbTextView2Class *klass);

static void mb_text_view2_dispose(GObject *object);

static void mb_text_view2_init(MbTextView2 *self);

/* Widget definition */

struct _MbTextView2
{
	GtkTextView parent;
};

G_DEFINE_TYPE(MbTextView2, mb_text_view2, GTK_TYPE_TEXT_VIEW)

/* Public implementation */

GtkWidget *mb_text_view2_new()
{
	return GTK_WIDGET(g_object_new(MB_TYPE_TEXT_VIEW2, NULL));
}

/* Private implementation */

static void mb_text_view2_class_init(MbTextView2Class *klass)
{

}

static void mb_text_view2_dispose(GObject *object)
{

}

static void mb_text_view2_init(MbTextView2 *self)
{

}

