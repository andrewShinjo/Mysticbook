#include "./mb_app_window.h"

/* Private definition */

static void apply_heading_tag(GtkTextBuffer *buffer, gint line_number, gint heading_level);

static void changed(GtkTextBuffer *buffer, gpointer user_data);

static void clear_tags(GtkTextBuffer *buffer, gint line_number);

static gint get_heading_level(GtkTextBuffer *buffer, int line_number);

static gint get_line_number(GtkTextBuffer *buffer);

/* Widget definition */
struct _MbAppWindow
{
  GtkApplicationWindow parent;
  /* WIDGETS */
	GtkWidget *text_view;
};
G_DEFINE_TYPE(MbAppWindow, mb_app_window, GTK_TYPE_APPLICATION_WINDOW)
/* PROPERTIES */
/* SIGNALS */

static void mb_app_window_init(MbAppWindow *self)
{
	self->text_view = gtk_text_view_new();
	gtk_text_view_set_monospace(GTK_TEXT_VIEW(self->text_view), TRUE);
	gtk_window_set_child(GTK_WINDOW(self), self->text_view);
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
	GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);

	// Initialize tags
	GtkTextTag *h1_tag = gtk_text_buffer_create_tag(buffer, "h1", "foreground", "blue", "scale", 2.0, NULL);
	gtk_text_tag_table_add(tag_table, h1_tag);
	g_signal_connect(buffer, "changed", G_CALLBACK(changed), NULL);
}

static void mb_app_window_class_init(MbAppWindowClass *klass) {}

/* PUBLIC IMPLEMENTATION */

GtkWidget* mb_app_window_new(GtkApplication *application)
{
	return GTK_WIDGET(g_object_new(MB_TYPE_APP_WINDOW, "application", application, NULL));
}

/* PRIVATE IMPLEMENTATION */

static void apply_heading_tag(GtkTextBuffer *buffer, gint line_number, gint heading_level)
{
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_start_iter(buffer, &end);
	gtk_text_iter_set_line(&start, line_number);
	gtk_text_iter_set_line(&end, line_number);
	gtk_text_iter_forward_to_line_end(&end);
	gtk_text_buffer_apply_tag_by_name(buffer, "h1", &start, &end);
}

static void changed(GtkTextBuffer *buffer, gpointer user_data)
{
	gint line_number = get_line_number(buffer);

	g_print("Line number: %d\n", line_number);

	GtkTextIter iter;

	gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));

	if(gtk_text_iter_backward_char(&iter) && gtk_text_iter_get_char(&iter) == '\n')
	{
		int heading_level;
		clear_tags(buffer, line_number - 1);
		heading_level = get_heading_level(buffer, line_number - 1);
		if(heading_level > 0)
		{
			apply_heading_tag(buffer, line_number - 1, heading_level);
		}
	}
	clear_tags(buffer, line_number);
	gint heading_level = get_heading_level(buffer, line_number);
	if(heading_level > 0)
	{
		apply_heading_tag(buffer, line_number, heading_level);
	}
}

static void clear_tags(GtkTextBuffer *buffer, gint line_number)
{
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_start_iter(buffer, &end);
	gtk_text_iter_set_line(&start, line_number);
	gtk_text_iter_set_line(&end, line_number);
	if(gtk_text_iter_ends_line(&end))
	{
		return;
	}
	gtk_text_iter_forward_to_line_end(&end);
	gtk_text_buffer_remove_all_tags(buffer, &start, &end);
}

static gint get_line_number(GtkTextBuffer *buffer)
{
	GtkTextIter iter;
	GtkTextMark *mark = gtk_text_buffer_get_insert(buffer);
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, mark);
	return gtk_text_iter_get_line(&iter);
}

static gint get_heading_level(GtkTextBuffer *buffer, int line_number)
{
	GtkTextIter iter;
	gtk_text_buffer_get_start_iter(buffer, &iter);
	gtk_text_iter_set_line(&iter, line_number);
	gunichar c = gtk_text_iter_get_char(&iter);
	gint level = 0;

	while(c == '*')
	{	
		if(gtk_text_iter_ends_line(&iter))
		{
			return 0;
		}

		level++;
		gtk_text_iter_forward_char(&iter);
		c = gtk_text_iter_get_char(&iter);
	}

	if(c == ' ')
	{
		return level;
	}
	else
	{
		return 0;
	}
}
