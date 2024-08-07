#include "mystic_text_view.h"

struct _MysticTextView
{
	GtkTextView parent;
	GFile *file;
};

G_DEFINE_TYPE(MysticTextView, mystic_text_view, GTK_TYPE_TEXT_VIEW)

static void 
mystic_text_view_init(MysticTextView *self)
{
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(self), GTK_WRAP_CHAR);
}

static void 
mystic_text_view_class_init(MysticTextViewClass *klass) {}

GtkWidget * 
mystic_text_view_new()
{
	return GTK_WIDGET(g_object_new(MYSTIC_TYPE_TEXT_VIEW, NULL));
}

// Additional functions

GFile *
mystic_text_view_get_file(
	MysticTextView *self)
{
	return self->file;
}

void
mystic_text_view_open_file(
	MysticTextView *self
)
{
	char *contents;
	gsize length;
	GError *error = NULL;

	if(g_file_load_contents(self->file, NULL, &contents, &length, NULL, &error))
	{
		gtk_text_buffer_set_text(
			gtk_text_view_get_buffer(GTK_TEXT_VIEW(self)), 
			contents, 
			-1
		);

		g_free(contents);
	}
}

void
mystic_text_view_set_file(
	MysticTextView *self,
	GFile *file)
{
	self->file = file;
}