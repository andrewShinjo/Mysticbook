#include "mystic_text_view.h"

// GObject definition
struct _MysticTextView
{
	GtkTextView tv;
	GFile *file;
};

GtkWidget * 
mystic_text_view_new()
{
	return GTK_WIDGET(
		g_object_new(
			MYSTIC_TYPE_TEXT_VIEW,
			"wrap-mode",
			GTK_WRAP_CHAR,
			NULL
		)
	);
}

GtkWidget *
mystic_text_view_new_with_file(GFile *file)
{
	g_return_val_if_fail(G_IS_FILE(file), NULL);

	GtkWidget *tv;
	GtkTextBuffer *tb;
	char *contents;
	gsize length;

	if(
		!g_file_load_contents(
			file, 
			NULL, 
			&contents, 
			&length, 
			NULL, 
			NULL
		)
	)
	{
		return NULL;
	}

	tv = mystic_text_view_new();
	tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
	gtk_text_buffer_set_text(tb, contents, length);
	MYSTIC_TEXT_VIEW(tv)->file = g_file_dup(file);
	gtk_text_buffer_set_modified(tb, FALSE);
	g_free(contents);
	return tv;
}

void mystic_text_view_save_file(MysticTextView *tv) {}

/* Private */

static void mystic_text_view_dispose(GObject *object);
static void mystic_text_view_finalize(GObject *object);

G_DEFINE_TYPE(MysticTextView, mystic_text_view, GTK_TYPE_TEXT_VIEW)

// Called everytime an instance of the class is created
static void mystic_text_view_init(MysticTextView *self) {}

// Called only the first time the class is being used
static void mystic_text_view_class_init(MysticTextViewClass *klass) {}

