#include "./mb_text_view.h"
#include "../file_service.h"

/* Private definition */

static void apply_heading_tag(GtkTextBuffer *buffer, gint line_number, gint heading_level);

static void apply_paragraph_tag(GtkTextBuffer *buffer, gint line_number);

static void changed(GtkTextBuffer *buffer, gpointer user_data);

static void clear_tags(GtkTextBuffer *buffer, gint line_number);

static gint get_heading_level(GtkTextBuffer *buffer, int line_number);

static gint get_line_number(GtkTextBuffer *buffer);

static void mb_text_view_class_init(MbTextViewClass *klass);

static void mb_text_view_dispose(GObject *object);

static void mb_text_view_init(MbTextView *self);

/* Widget definition */

struct _MbTextView
{
	GtkWidget parent;
	/* Child widgets */
	GtkWidget *container;
	GtkWidget *label;
	GtkWidget *scrolled_window;
	GtkWidget *text_view;
	/* Event listeners */
	/* Properties */
	/* Other fields */
	GFile *file;
};

G_DEFINE_TYPE(MbTextView, mb_text_view, GTK_TYPE_WIDGET)

/* Properties */

/* Signals */

/* Public implementation */

GtkWidget* mb_text_view_new()
{
	return GTK_WIDGET(g_object_new(MB_TYPE_TEXT_VIEW, NULL));
}

void mb_text_view_set_gfile(MbTextView *self, GFile *file)
{
	GFileInputStream *stream;
	GDataInputStream *data_stream;
	GError *error = NULL;
	gchar *contents;
	gsize length;

	self->file = file;

	// Set filename label.
	gchar *basename = g_file_get_basename(file);
	gtk_label_set_text(GTK_LABEL(self->label), basename);

	// Read file contents into text buffer.
	stream = g_file_read(file, NULL, &error);

	if(error != NULL)
	{
		g_printerr("Error opening file: %s\n", error->message);
		g_error_free(error);
		exit(EXIT_FAILURE);
	}

	data_stream = g_data_input_stream_new(G_INPUT_STREAM(stream));
	contents = g_data_input_stream_read_upto(data_stream, "\0", -1, &length, NULL, &error);

	if(error != NULL)
	{
		g_printerr("Error reading file: %s\n", error->message);
		g_error_free(error);
		exit(EXIT_FAILURE);
	}

	gtk_text_view_set_editable(GTK_TEXT_VIEW(self->text_view), TRUE);

	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
	if(contents == NULL)
	{
		gtk_text_buffer_set_text(buffer, "", -1);
	}
	else if(contents != NULL)
	{
		gtk_text_buffer_set_text(buffer, contents, length);
	}

	g_free(contents);
	g_object_unref(data_stream);
	g_object_unref(stream);
}

/* Private implementation */

static void apply_heading_tag(GtkTextBuffer *buffer, gint line_number, gint heading_level)
{
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_start_iter(buffer, &end);
	gtk_text_iter_set_line(&start, line_number);
	gtk_text_iter_set_line(&end, line_number);
	gtk_text_iter_forward_to_line_end(&end);
	GtkTextTag *heading_tag = gtk_text_buffer_create_tag(buffer, NULL, "font", "Monospace 20", NULL);
	gtk_text_buffer_apply_tag(buffer, heading_tag, &start, &end);
}

// Currently not being used.
static void apply_paragraph_tag(GtkTextBuffer *buffer, gint line_number)
{
	if(line_number == 0)
	{
		return;
	}

	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_start_iter(buffer, &end);
	gtk_text_iter_set_line(&start, line_number);
	gtk_text_iter_set_line(&end, line_number);
	gtk_text_iter_forward_to_line_end(&end);

	line_number--;
	int heading_level = get_heading_level(buffer, line_number);
	while(line_number > 0 && heading_level == 0)
	{
		line_number--;
		heading_level = get_heading_level(buffer, line_number);
	}
	if(heading_level > 0)
	{
		GtkTextTag *margin_tag = gtk_text_buffer_create_tag(
			buffer, NULL, "left-margin", (heading_level + 1) * 19, NULL);	
		gtk_text_buffer_apply_tag(buffer, margin_tag, &start, &end);
	}
}

static void changed(GtkTextBuffer *buffer, gpointer user_data)
{
	MbTextView *self = MB_TEXT_VIEW(user_data);

	// Save text to disk.
	
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
	gsize length = gtk_text_iter_get_offset(&end) - gtk_text_iter_get_offset(&start);	
	file_service_update_file(self->file, text, length);

	g_free(text);

	// Apply tags.
	gint line_number = get_line_number(buffer);
	GtkTextIter iter;

	gtk_text_buffer_get_iter_at_line(buffer, &iter, line_number);
	gboolean last_key_newline = gtk_text_iter_backward_char(&iter) && gtk_text_iter_get_char(&iter);

	if(last_key_newline)
	{

		int heading_level;
		clear_tags(buffer, line_number - 1);
		heading_level = get_heading_level(buffer, line_number - 1);
		if(heading_level > 0)
		{
			apply_heading_tag(buffer, line_number - 1, heading_level);
		}
		else if(heading_level == 0)
		{
			//apply_paragraph_tag(buffer, line_number - 1);
		}
	}

	clear_tags(buffer, line_number);
	gint heading_level = get_heading_level(buffer, line_number);

	if(heading_level > 0)
	{
		apply_heading_tag(buffer, line_number, heading_level);
	}
	else if(heading_level == 0)
	{
		//apply_paragraph_tag(buffer, line_number);
	}
}

static void clear_tags(GtkTextBuffer *buffer, gint line_number)
{
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_start_iter(buffer, &end);
	gtk_text_iter_set_line(&start, line_number);
	gtk_text_iter_set_line(&end, line_number);
	if(!gtk_text_iter_ends_line(&end))
	{
		gtk_text_iter_forward_to_line_end(&end);
	}
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
static void mb_text_view_class_init(MbTextViewClass *klass) 
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = mb_text_view_dispose;

	gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

static void mb_text_view_dispose(GObject *object)
{
	MbTextView *self = MB_TEXT_VIEW(object);
	g_clear_pointer(&self->container, gtk_widget_unparent);
	G_OBJECT_CLASS(mb_text_view_parent_class)->dispose(object);
}

static void mb_text_view_init(MbTextView *self) 
{
	/* Instantiate widgets */
	self->container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	self->label = gtk_label_new("No File Open");
	self->scrolled_window = gtk_scrolled_window_new();
	self->text_view = gtk_text_view_new();
	
	/* Configure widgets */
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(self->scrolled_window);
	gtk_scrolled_window_set_child(scrolled_window, self->text_view);
	gtk_scrolled_window_set_policy(scrolled_window, GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(self->text_view), FALSE);
	gtk_text_view_set_monospace(GTK_TEXT_VIEW(self->text_view), TRUE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(self->text_view), GTK_WRAP_WORD);

	gtk_widget_set_hexpand(self->scrolled_window, TRUE);
	gtk_widget_set_vexpand(self->scrolled_window, TRUE);

	gtk_box_append(GTK_BOX(self->container), self->label);
	gtk_box_append(GTK_BOX(self->container), self->scrolled_window);

	gtk_widget_set_parent(self->container, GTK_WIDGET(self));

	/* Connect to signals */
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
	g_signal_connect(buffer, "changed", G_CALLBACK(changed), self);
}
