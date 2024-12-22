#include "./mb_picture.h"
#include "./mb_text_view.h"
#include "../clipboard_service.h"
#include "../file_service.h"

/* Private definition */

typedef enum
{
	SEARCH_AUTHOR,
	SEARCH_PROPERTY,
	SEARCH_TEXT,
	SEARCH_TITLE
} ParseState;

static void apply_simple_text_formatting(GtkTextBuffer *buffer, gint line_number);

static void changed(GtkTextBuffer *buffer, gpointer user_data);

static void clear_tags(GtkTextBuffer *buffer, gint line_number);

static gint get_heading_level(GtkTextBuffer *buffer, int line_number);

static gint get_line_number(GtkTextBuffer *buffer);

static void insert_picture_at_insert(MbTextView *self, const gchar* picture_path);

static gboolean key_pressed(
	GtkEventControllerKey* key_event, guint keyval, guint keycode, GdkModifierType state, gpointer user_data);

static void mb_text_view_class_init(MbTextViewClass *klass);

static void mb_text_view_dispose(GObject *object);

static void mb_text_view_init(MbTextView *self);

static void update_tags(MbTextView *self, GtkTextBuffer *buffer);

/* Widget definition */

struct _MbTextView
{
	GtkWidget parent;
	/* Child widgets */
	GtkWidget *container;
	GtkWidget *label;
	GtkWidget *scrolled_window;
	GtkWidget *text_view;
	GtkWidget *picture;
	/* Event listeners */
	GtkEventController *key_event;
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

	gchar *basename = g_file_get_basename(file);
	gtk_label_set_text(GTK_LABEL(self->label), basename);

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

	update_tags(self, buffer);
	g_free(contents);
}

/* Private implementation */

static void changed(GtkTextBuffer *buffer, gpointer user_data)
{
	MbTextView *self = MB_TEXT_VIEW(user_data);

	// Save text to disk.
	/*
	{	
		GtkTextIter start, end;
		gtk_text_buffer_get_start_iter(buffer, &start);
		gtk_text_buffer_get_end_iter(buffer, &end);
		gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
		gsize length = gtk_text_iter_get_offset(&end) - gtk_text_iter_get_offset(&start);	
		file_service_update_file(self->file, text, length);
		g_free(text);
	}
	*/

	update_tags(self, buffer);
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

static void insert_picture_at_insert(MbTextView *self, const gchar* picture_path)
{
	g_print("insert_picture_at_insert\n");
	GtkWidget *picture = mb_picture_new(picture_path);
	GtkTextView *text_view = GTK_TEXT_VIEW(self->text_view);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
	GtkTextMark *insert_mark = gtk_text_buffer_get_insert(buffer);
	GtkTextIter iter;
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, insert_mark);
	GtkTextChildAnchor *anchor = gtk_text_buffer_create_child_anchor(buffer, &iter);
	gtk_text_view_add_child_at_anchor(text_view, picture, anchor);

}

static gboolean key_pressed(
	GtkEventControllerKey* key_event, guint keyval, guint keycode, GdkModifierType state, gpointer user_data)
{
	MbTextView *self = MB_TEXT_VIEW(user_data);
	const gboolean CONTROL_V_PRESSED = (state & GDK_CONTROL_MASK) && keyval == GDK_KEY_v;

	if(CONTROL_V_PRESSED && clipboard_service_has_picture())
	{
		const gchar *filename = clipboard_service_save_picture();
		insert_picture_at_insert(self, filename);
		return TRUE;
	}

	return FALSE;
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
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
	/* Instantiate event listeners */
	self->key_event = gtk_event_controller_key_new();

	/* Configure widgets */
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(self->scrolled_window);
	gtk_scrolled_window_set_child(scrolled_window, self->text_view);
	gtk_scrolled_window_set_policy(scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(self->text_view), FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(self->text_view), GTK_WRAP_CHAR);

	gtk_widget_set_hexpand(self->scrolled_window, TRUE);
	gtk_widget_set_vexpand(self->scrolled_window, TRUE);

	gtk_box_append(GTK_BOX(self->container), self->label);
	gtk_box_append(GTK_BOX(self->container), self->scrolled_window);

	gtk_widget_set_parent(self->container, GTK_WIDGET(self));
	
	gtk_widget_add_controller(self->text_view, self->key_event);

	/* Connect to signals */
	gtk_text_buffer_create_tag(buffer, "author", "background", "blue", NULL);
	gtk_text_buffer_create_tag(buffer, "properties", "background", "green", NULL);
	gtk_text_buffer_create_tag(buffer, "title", "background", "purple", NULL);
	gtk_text_buffer_create_tag(buffer, "heading", "font", "Open Sans 18", NULL);
	gtk_text_buffer_create_tag(buffer, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);
	gtk_text_buffer_create_tag(buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
	g_signal_connect(self->key_event, "key-pressed", G_CALLBACK(key_pressed), self);
	g_signal_connect(buffer, "changed", G_CALLBACK(changed), self);
}

static void update_tags(MbTextView *self, GtkTextBuffer *buffer)
{
	// Clear all tags in the buffer
	{
		GtkTextIter start, end;
		gtk_text_buffer_get_start_iter(buffer, &start);
		gtk_text_buffer_get_end_iter(buffer, &end);
		gtk_text_buffer_remove_all_tags(buffer, &start, &end);
	}

	// Apply tags
	ParseState state = SEARCH_TITLE;
	GtkTextIter buffer_end;
	gtk_text_buffer_get_end_iter(buffer, &buffer_end);

	gint line_count = gtk_text_iter_get_line(&buffer_end) + 1;
	gint heading_level = 0;
	gint parent_level = 0;
	gint line = 0;

	while(line < line_count)
	{
		GtkTextIter start, end;
		gtk_text_buffer_get_start_iter(buffer, &start);
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
				gtk_text_buffer_apply_tag_by_name(buffer, "heading", &start, &end);
				state = SEARCH_PROPERTY;
			}

			GQueue *stack = g_queue_new();
			int start_pos = gtk_text_iter_get_offset(&start);
			GtkTextIter pointer = start;

			while(!gtk_text_iter_is_end(&pointer))
			{
				gunichar c = gtk_text_iter_get_char(&pointer);

				if(c == '/')
				{
					gboolean is_ending = FALSE;

					if(!gtk_text_iter_starts_line(&pointer))
					{
						GtkTextIter backward_iter = pointer;
						gtk_text_iter_backward_char(&backward_iter);
						gunichar previous_char = gtk_text_iter_get_char(&backward_iter);

						if(previous_char != ' ' && previous_char != '\n' && previous_char != '/'
							&& !g_queue_is_empty(stack))
						{
							gint italic_start_offset = GPOINTER_TO_INT(g_queue_peek_tail(stack));
							gint italic_end_offset = gtk_text_iter_get_offset(&pointer) + 1;
							GtkTextIter italic_start_iter = start;
							GtkTextIter italic_end_iter = start;
							gtk_text_iter_forward_chars(&italic_start_iter, italic_start_offset);
							gtk_text_iter_forward_chars(&italic_end_iter, italic_end_offset);
							gunichar italic_start_char = gtk_text_iter_get_char(&italic_start_iter);

							if(italic_start_char == '/')
							{
								g_queue_pop_tail(stack);
								gtk_text_buffer_apply_tag_by_name(buffer, "italic", &italic_start_iter, &italic_end_iter);
								is_ending = TRUE;
							}
						}
					}
					if(!is_ending && !gtk_text_iter_ends_line(&pointer))
					{
						GtkTextIter forward_iter = pointer;
						gtk_text_iter_forward_char(&forward_iter);
						gunichar forward_char = gtk_text_iter_get_char(&forward_iter);
						if(forward_char != ' ' && forward_char != '\n' && forward_char != '/' && forward_char != 0)
						{
							gint pos = gtk_text_iter_get_offset(&pointer);
							g_queue_push_tail(stack, GINT_TO_POINTER(pos));
						}
					}
				}
				else if(c == '*')
				{
					gboolean is_ending = FALSE;

					if(!gtk_text_iter_starts_line(&pointer))
					{
						GtkTextIter backward_iter = pointer;
						gtk_text_iter_backward_char(&backward_iter);
						gunichar backward_char = gtk_text_iter_get_char(&backward_iter);
						if(backward_char != ' ' && backward_char != '\n' && backward_char != '*'
							&& !g_queue_is_empty(stack))
						{
							gint bold_start_offset = GPOINTER_TO_INT(g_queue_peek_tail(stack));
							gint bold_end_offset = gtk_text_iter_get_offset(&pointer) + 1;
							GtkTextIter bold_start_iter = start;
							GtkTextIter bold_end_iter = start;
							gtk_text_iter_forward_chars(&bold_start_iter, bold_start_offset);
							gtk_text_iter_forward_chars(&bold_end_iter, bold_end_offset);
							gunichar bold_start_char = gtk_text_iter_get_char(&bold_start_iter);

							if(bold_start_char == '*')
							{
								g_queue_pop_tail(stack);
								gtk_text_buffer_apply_tag_by_name(buffer, "bold", &bold_start_iter, &bold_end_iter);
								is_ending = TRUE;
							}
						}
					}	
					if(!is_ending && !gtk_text_iter_ends_line(&pointer))
					{
						GtkTextIter forward_iter = pointer;
						gtk_text_iter_forward_char(&forward_iter);
						gunichar forward_char = gtk_text_iter_get_char(&forward_iter);
						if(forward_char != ' ' && forward_char != '\n' && forward_char != '*' && forward_char != 0)
						{
							gint pos = gtk_text_iter_get_offset(&pointer);
							g_queue_push_tail(stack, GINT_TO_POINTER(pos));
						}
					}
				}

				gtk_text_iter_forward_char(&pointer);
			}
			line++;
		}
		else if(state == SEARCH_PROPERTY)
		{
			if(g_strcmp0(text, ":PROPERTIES:") == 0)
			{
				gint next_line = line + 1;
				GtkTextIter next_start, next_end;
				gtk_text_iter_set_line(&next_start, next_line);
				next_end = next_start;

				if(!gtk_text_iter_ends_line(&next_end))
				{
					gtk_text_iter_forward_to_line_end(&next_end);
				}

				gchar *next_text = gtk_text_buffer_get_text(buffer, &next_start, &next_end, TRUE);

				while(g_strcmp0(next_text, ":END:") != 0 && g_str_has_prefix(next_text, ":"))
				{
					g_free(next_text);
					next_line++;
					gtk_text_iter_set_line(&next_start, next_line);
					next_end = next_start;
					if(!gtk_text_iter_ends_line(&next_end))
					{
						gtk_text_iter_forward_to_line_end(&next_end);
					}
					next_text = gtk_text_buffer_get_text(buffer, &next_start, &next_end, TRUE);
				}
				
				if(g_strcmp0(next_text, ":END:") == 0)
				{
					gtk_text_buffer_apply_tag_by_name(buffer, "properties", &start, &next_end);
					line++;
				}

				state = SEARCH_TEXT;
				g_free(next_text);
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
