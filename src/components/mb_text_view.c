#include "./mb_text_view.h"

/* Private definition */

static void mb_text_view_class_init(MbTextViewClass *klass);

static void mb_text_view_init(MbTextView *self);

/* Widget definition */

struct _MbTextView
{
	GtkTextView parent;
	/* Child widgets */
	/* Other widgets */
	GFile *file;
	/* Event listeners */
	/* Properties */
	/* Other fields */
};

G_DEFINE_TYPE(MbTextView, mb_text_view, GTK_TYPE_TEXT_VIEW)

/* Properties */

/* Signals */

/* Public implementation */

GtkWidget* mb_text_view_new()
{
	return GTK_WIDGET(g_object_new(MB_TYPE_TEXT_VIEW, NULL));
}

/* Private implementation */

static void mb_text_view_class_init(MbTextViewClass *klass) {}

static void mb_text_view_init(MbTextView *self) {}
