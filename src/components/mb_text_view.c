#include "./mb_text_view.h"
#include "../file_service.h"

/* Private definition */

typedef enum
{
	SEARCH_AUTHOR,
	SEARCH_PROPERTY,
	SEARCH_TEXT,
	SEARCH_TITLE
} ParseState;

static void apply_heading_tag(GtkTextBuffer *buffer, gint line_number, gint heading_level);

static void apply_simple_text_formatting(GtkTextBuffer *buffer, gint line_number);

static void changed(GtkTextBuffer *buffer, gpointer user_data);

static void clear_tags(GtkTextBuffer *buffer, gint line_number);

static gboolean find_indent_range(GtkTextBuffer *buffer, GtkTextIter *start, GtkTextIter *end);

static gint get_heading_level(GtkTextBuffer *buffer, int line_number);

static gint get_line_number(GtkTextBuffer *buffer);

static void mb_text_view_class_init(MbTextViewClass *klass);

static void mb_text_view_dispose(GObject *object);

static void mb_text_view_init(MbTextView *self);

static void update_tags(GtkTextBuffer *buffer);

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

	gtk_text_view_set_editable(GTK_TEXT_VIEW(self->text_view), TRUE);
	self->file = file;

	gchar *contents;
	gsize length;

	// Set filename label.
	gchar *basename = g_file_get_basename(file);
	gtk_label_set_text(GTK_LABEL(self->label), basename);

	// Read file contents into text buffer.

	contents = file_service_read_file(file, &length);

	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));

	if(contents == NULL)
	{
		gtk_text_buffer_set_text(buffer, "", -1);
	}
	else if(contents != NULL)
	{
		gtk_text_buffer_set_text(buffer, contents, length);
	}

	// Apply tags to newly added text
	gint line_count = gtk_text_buffer_get_line_count(buffer);

	for(gint line_number = 0; line_number < line_count; line_number++)
	{
		gint heading_level = get_heading_level(buffer, line_number);
		if(heading_level > 0)
		{
			apply_heading_tag(buffer, line_number, heading_level);	
		}	
	}

	g_free(contents);
}

/* Private implementation */

static void apply_heading_tag(GtkTextBuffer *buffer, gint line_number, gint heading_level)
{
	// Apply heading tag.
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_start_iter(buffer, &end);
	gtk_text_iter_set_line(&start, line_number);
	gtk_text_iter_set_line(&end, line_number);
	gtk_text_iter_forward_to_line_end(&end);
	GtkTextTag *heading_tag = gtk_text_buffer_create_tag(buffer, NULL, "font", "Monospace 20", NULL);
	gtk_text_buffer_apply_tag(buffer, heading_tag, &start, &end);

	// Apply TODO/DONE tag.
	gtk_text_iter_forward_chars(&start, heading_level + 1);
	end = start;
	gtk_text_iter_forward_chars(&end, 5);

	gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
	gboolean is_todo = g_strcmp0(text, "TODO ") == 0;
	gboolean is_done = g_strcmp0(text, "DONE ") == 0;

	if(is_todo)
	{
		GtkTextTag *todo_tag = 
			gtk_text_buffer_create_tag(buffer, NULL, "foreground", "blue", "weight", PANGO_WEIGHT_BOLD, NULL);
		gtk_text_buffer_apply_tag(buffer, todo_tag, &start, &end);
	}
	else if(is_done)
	{
		GtkTextTag *done_tag = 
			gtk_text_buffer_create_tag(buffer, NULL, "foreground", "red", "weight", PANGO_WEIGHT_BOLD, NULL);
		gtk_text_buffer_apply_tag(buffer, done_tag, &start, &end);
	}

	// Look for PROPERTIES below heading.
	gtk_text_iter_forward_line(&start);
	end = start;
	gtk_text_iter_forward_to_line_end(&end);
	text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);

	if(g_strcmp0(text, ":PROPERTIES:") == 0)
	{
		GtkTextIter temp = start;
		gtk_text_iter_forward_line(&temp);
		gtk_text_iter_forward_line(&end);
		gtk_text_iter_forward_to_line_end(&end);
		gchar *text2 = gtk_text_buffer_get_text(buffer, &temp, &end, TRUE);
		if(g_strcmp0(text2, ":END:") == 0)
		{
			GtkTextTag *properties_tag = 
				gtk_text_buffer_create_tag(buffer, NULL, "paragraph-background", "purple", NULL);
			gtk_text_buffer_apply_tag(buffer, properties_tag, &start, &end);
		}
		g_free(text2);
	}

	g_free(text);
}

static void changed(GtkTextBuffer *buffer, gpointer user_data)
{
	MbTextView *self = MB_TEXT_VIEW(user_data);

	// Save text to disk.
	{	
		GtkTextIter start, end;
		gtk_text_buffer_get_start_iter(buffer, &start);
		gtk_text_buffer_get_end_iter(buffer, &end);
		gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
		gsize length = gtk_text_iter_get_offset(&end) - gtk_text_iter_get_offset(&start);	
		file_service_update_file(self->file, text, length);
		g_free(text);
	}

	update_tags(buffer);
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

static gboolean find_indent_range(GtkTextBuffer *buffer, GtkTextIter *start, GtkTextIter *end)
{
	GtkTextIter pointer = *start;
	gtk_text_iter_forward_char(&pointer);
	gunichar c = gtk_text_iter_get_char(&pointer);

	if(c == ' ' || c == '*')
	{
		return FALSE;
	}

	return FALSE;
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
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(self->text_view), GTK_WRAP_CHAR);

	gtk_widget_set_hexpand(self->scrolled_window, TRUE);
	gtk_widget_set_vexpand(self->scrolled_window, TRUE);

	gtk_box_append(GTK_BOX(self->container), self->label);
	gtk_box_append(GTK_BOX(self->container), self->scrolled_window);

	gtk_widget_set_parent(self->container, GTK_WIDGET(self));

	/* Connect to signals */
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
	gtk_text_buffer_create_tag(buffer, "author", "background", "blue", NULL);
	gtk_text_buffer_create_tag(buffer, "properties", "background", "green", NULL);
	gtk_text_buffer_create_tag(buffer, "title", "background", "purple", NULL);
	g_signal_connect(buffer, "changed", G_CALLBACK(changed), self);
}

static void update_tags(GtkTextBuffer *buffer)
{
	// Clear all tags in the buffer
	{
		GtkTextIter start, end;
		gtk_text_buffer_get_start_iter(buffer, &start);
		gtk_text_buffer_get_end_iter(buffer, &end);
		gtk_text_buffer_remove_all_tags(buffer, &start, &end);
	}

	ParseState state = SEARCH_TITLE;
	GtkTextIter end;
	gtk_text_buffer_get_end_iter(buffer, &end);

	gint line_count = gtk_text_iter_get_line(&end) + 1;
	gint heading_level = 0;
	gint parent_level = 0;
	gint line = 0;

	while(line < line_count)
	{
		GtkTextIter start, end;
		gtk_text_iter_set_line(&start, line);
		end = start;

		if(!gtk_text_iter_ends_line(&end))
		{
			gtk_text_iter_forward_to_line_end(&end);
		}

		gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);

		if(state == SEARCH_TITLE)
		{
			if(g_str_has_prefix(text, "#+title: "))
			{
				gtk_text_buffer_apply_tag_by_name(buffer, "title", &start, &end);
				line++;
			}
			state = SEARCH_AUTHOR;
		}		
		else if(state == SEARCH_AUTHOR)
		{
			if(g_str_has_prefix(text, "#+author: "))
			{
				gtk_text_buffer_apply_tag_by_name(buffer, "author", &start, &end);
				line++;
			}
			state = SEARCH_TEXT;
		}
		else if(state == SEARCH_TEXT)
		{
			heading_level = get_heading_level(buffer, line);	
			gboolean is_heading = (heading_level > 0);

			if(is_heading)
			{
				apply_heading_tag(buffer, line, heading_level);
				state = SEARCH_PROPERTY;
			}
			else if(!is_heading)
			{

			}
			line++;
		}
		else if(state == SEARCH_PROPERTY)
		{
			if(g_strcmp0(text, ":PROPERTIES:") == 0)
			{
				gtk_text_buffer_apply_tag_by_name(buffer, "properties", &start, &end);
				state = SEARCH_TEXT;
				line++;
			}
			else
			{
				state = SEARCH_TEXT;
			}
		}
		else
		{
			line++;
		}
		
		g_free(text);
	}
}
