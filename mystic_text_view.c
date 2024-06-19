#include "mystic_text_view.h"

struct _MysticTextView
{
	GtkTextView tv;
};

G_DEFINE_TYPE(MysticTextView, mystic_text_view, GTK_TYPE_TEXT_VIEW)

// Called everytime an instance of the class is created
static void mystic_text_view_init(MysticTextView *self) {}

// Called only the first time the class is being used
static void mystic_text_view_class_init(MysticTextViewClass *klass) {}

// The constructor for the MysticTextView widget
GtkWidget * mystic_text_view_new()
{
	return g_object_new(MYSTIC_TYPE_TEXT_VIEW, /* Properties can be specified here, if needed */ NULL);
}
