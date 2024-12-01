#include "./mb_app_window.h"
#include "./gfile_list_view.h"
#include "../file_service.h"

/* Private definition */

static void apply_heading_tag(GtkTextBuffer *buffer, gint line_number, gint heading_level);

static void apply_paragraph_tag(GtkTextBuffer *buffer, gint line_number);

static void changed(GtkTextBuffer *buffer, gpointer user_data);

static void clear_tags(GtkTextBuffer *buffer, gint line_number);

static gint get_heading_level(GtkTextBuffer *buffer, int line_number);

static gint get_line_number(GtkTextBuffer *buffer);

/* Widget definition */
struct _MbAppWindow
{
  GtkApplicationWindow parent;
  /* WIDGETS */

	GtkWidget *container;
	GtkWidget *gfile_list_view;
	GtkWidget *text_view;
};
G_DEFINE_TYPE(MbAppWindow, mb_app_window, GTK_TYPE_APPLICATION_WINDOW)
/* PROPERTIES */
/* SIGNALS */

static void mb_app_window_init(MbAppWindow *self)
{
	self->container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	self->gfile_list_view = gfile_list_view_new();
	self->text_view = gtk_text_view_new();
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(self->text_view), 25);
	gtk_text_view_set_monospace(GTK_TEXT_VIEW(self->text_view), TRUE);
	
	gtk_box_append(GTK_BOX(self->container), self->gfile_list_view);
	gtk_box_append(GTK_BOX(self->container), self->text_view);

	gtk_widget_set_hexpand(self->text_view, TRUE);
	gtk_widget_set_vexpand(self->text_view, TRUE);

	gtk_window_set_child(GTK_WINDOW(self), self->container);
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));

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
