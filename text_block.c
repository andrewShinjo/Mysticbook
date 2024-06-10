#include <string.h>
#include "text_block.h"

/* Editor private functions */

static int
editor_get_heading_level(const char *heading)
{
    int level = 0;
    while(heading[level] == '*') 
    {
        level++;
    }
    return level;
}

static const char*
editor_get_current_line(GtkTextView *text_view) 
{
    GtkTextIter start, end;
    GtkTextBuffer *buffer;
    GtkTextMark *mark;
    int line_number;
    
    buffer = gtk_text_view_get_buffer(text_view);
    mark = gtk_text_buffer_get_insert(buffer);
    
    gtk_text_buffer_get_iter_at_mark(buffer, &start, mark);
    gtk_text_buffer_get_iter_at_mark(buffer, &end, mark);
    gtk_text_iter_set_line_offset(&start, 0);
    gtk_text_iter_forward_to_line_end(&end);

    char* text = gtk_text_buffer_get_text(
        buffer,
        &start,
        &end,
        false
    );

    return text;
}

static gboolean
editor_is_line_a_heading(const char* line)
{
    size_t length = strlen(line);

    if(length < 2 || line[0] != '*')
    {
        return false;
    }

    for(int i = 1; i < length; i++)
    {
        char c = line[i];

        if(c == '*')
        {
            continue;
        }
        else if(c == ' ')
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

static void
editor_fold_heading(GtkTextView *text_view)
{
    // This function assumes the text cursor is on a heading line.

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    GtkTextMark *text_cursor = gtk_text_buffer_get_insert(buffer);
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);
    GtkTextIter start_iter, end_iter;
    GtkTextIter content_start, content_end;

    gtk_text_buffer_get_iter_at_mark(
        buffer,
        &start_iter,
        text_cursor
    );

    end_iter = start_iter;
    gtk_text_iter_set_line_offset(&start_iter, 0);
    gtk_text_iter_forward_to_line_end(&end_iter);
    gchar *line = gtk_text_iter_get_text(&start_iter, &end_iter);
    int heading_level = editor_get_heading_level(line);
    g_free(line);

    gtk_text_iter_forward_line(&start_iter);

    if(gtk_text_iter_is_end(&start_iter))
    {
        return;
    }

    end_iter = start_iter;
    gtk_text_iter_set_line_offset(&start_iter, 0);
    gtk_text_iter_forward_to_line_end(&end_iter);
    line = gtk_text_iter_get_text(&start_iter, &end_iter);

    if(editor_is_line_a_heading(line))
    {
        int heading_level2 = editor_get_heading_level(line);
        
        if(heading_level >= heading_level2)
        {
            g_free(line);
            return;
        }
    }

    g_free(line);

    content_start = start_iter;
    content_end = start_iter;
    gboolean next_line_is_same_level_heading = false;

    while(
        !gtk_text_iter_is_end(&content_end) && 
        !next_line_is_same_level_heading
    )
    {
        gtk_text_iter_forward_char(&content_end);

        if(gtk_text_iter_starts_line(&content_end))
        {
            GtkTextIter s, e;
            s = content_end;
            e = content_end;
            gtk_text_iter_forward_to_line_end(&e);
            gchar *line2 = gtk_text_iter_get_text(&s, &e);
            if(!editor_is_line_a_heading(line2))
            {
                continue;
            }
            int heading_level2 = editor_get_heading_level(line2);
            if(heading_level >= heading_level2)
            {
                gtk_text_iter_backward_char(&content_end);
                next_line_is_same_level_heading = true;
            }
        }
    }

    line = gtk_text_iter_get_text(&content_start, &content_end);
    g_print("Line:\n%s\n", line);
    g_free(line);

    GtkTextTag *fold_tag = gtk_text_tag_table_lookup(
        tag_table,
        "fold"
    );

    gtk_text_buffer_apply_tag(
        buffer, 
        fold_tag, 
        &content_start, 
        &content_end
    );

}

gboolean
key_pressed (
    GtkEventControllerKey* self,
    guint keyval,
    guint keycode,
    GdkModifierType state,
    gpointer user_data
)
{
    GtkWidget *widget = (GtkWidget*) user_data;
    GtkTextView *text_view = GTK_TEXT_VIEW(widget);
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(text_buffer);

     gunichar input = gdk_keyval_to_unicode(keyval);

    if(g_unichar_isprint(input))
    {
        GtkTextMark *insert_mark = gtk_text_buffer_get_insert(text_buffer);  
        GtkTextIter insert_iter;
        gtk_text_buffer_get_iter_at_mark(text_buffer, &insert_iter, insert_mark);
        GtkTextIter start_iter = insert_iter;
        GtkTextIter end_iter = insert_iter;
        gtk_text_iter_set_line_offset(&start_iter, 0);
        gtk_text_iter_forward_to_line_end(&end_iter);
        gchar *line = gtk_text_iter_get_text(&start_iter, &end_iter);
        GtkTextTag *h1_tag = gtk_text_tag_table_lookup(tag_table, "h1");
        if(editor_is_line_a_heading(line))
        {
            gtk_text_buffer_insert_with_tags(text_buffer, &insert_iter, (char*) &input, 1, h1_tag, NULL);
        }
        else
        {
            gtk_text_buffer_insert(text_buffer, &insert_iter, (char*) &input, 1);
        }
        return GDK_EVENT_STOP;
    }

    return GDK_EVENT_PROPAGATE;
}

void
key_released (
  GtkEventControllerKey* self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
)
{
    GtkWidget *widget = (GtkWidget*) user_data;
    GtkTextView *text_view = GTK_TEXT_VIEW(widget);
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(text_buffer);
    

    // Check if current line is a heading or not after modification.
    {
        GtkTextMark *insert_mark = gtk_text_buffer_get_insert(text_buffer);
        GtkTextIter insert_iter;
        gtk_text_buffer_get_iter_at_mark(text_buffer, &insert_iter, insert_mark);
        GtkTextIter start_iter = insert_iter;
        GtkTextIter end_iter = insert_iter;
        gtk_text_iter_set_line_offset(&start_iter, 0);
        gtk_text_iter_forward_to_line_end(&end_iter);
        gchar *line = gtk_text_iter_get_text(&start_iter, &end_iter);
        GtkTextTag *h1_tag = gtk_text_tag_table_lookup(tag_table, "h1");
        if(editor_is_line_a_heading(line))
        {
            gtk_text_buffer_apply_tag(text_buffer, h1_tag, &start_iter, &end_iter);
        }
        else
        {
            gtk_text_buffer_remove_tag(text_buffer, h1_tag, &start_iter, &end_iter);
        }
        g_free(line);
    }
}

static void text_block_dispose(GObject *object);
static void text_block_finalize(GObject *object);

struct _TextBlock
{
    GtkWidget parent_instance;
    GtkWidget *text_view;
    GtkEventController *key_event_controller;
};

G_DEFINE_TYPE(
    TextBlock,
    text_block,
    GTK_TYPE_WIDGET
)

static void
text_block_init(TextBlock *self)
{
    GtkWidget *widget = GTK_WIDGET(self);
    self->text_view = gtk_text_view_new();
    self->key_event_controller = gtk_event_controller_key_new();

    GtkTextView *text_view = GTK_TEXT_VIEW(self->text_view);

    gtk_text_view_set_bottom_margin(text_view, 10);
    gtk_text_view_set_left_margin(text_view, 10);
    gtk_text_view_set_right_margin(text_view, 10);
    gtk_text_view_set_top_margin(text_view, 10);
    gtk_widget_add_controller(self->text_view, self->key_event_controller);
    gtk_widget_set_hexpand(self->text_view, true);
    gtk_widget_set_parent(self->text_view, widget);

    // Initialize tag table.
    {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
        GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);

        GtkTextTag *fold_tag = gtk_text_buffer_create_tag(
            buffer, 
            "fold", 
            "invisible", 
            true, 
            NULL
        );

        GtkTextTag *h1_tag = gtk_text_buffer_create_tag(
            buffer,
            "h1",
            "foreground",
            "purple",
            NULL
        );

    }

    // Initialize signals
    {
        g_signal_connect(
            self->key_event_controller, 
            "key-pressed", 
            G_CALLBACK(key_pressed), 
            self->text_view
        );

        g_signal_connect(
            self->key_event_controller,
            "key-released",
            G_CALLBACK(key_released),
            self->text_view
        );
    }
    
}

static void
text_block_class_init(TextBlockClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->dispose = text_block_dispose;
    object_class->finalize = text_block_finalize;

    gtk_widget_class_set_layout_manager_type(
        GTK_WIDGET_CLASS(klass),
        GTK_TYPE_BOX_LAYOUT
    );
}

GtkWidget *
text_block_new(void)
{
    return g_object_new(TEXT_TYPE_BLOCK, NULL);
}

static void
text_block_dispose(GObject *object)
{
    TextBlock *self = TEXT_BLOCK(object);
    g_clear_pointer(&self->text_view, gtk_widget_unparent);
    G_OBJECT_CLASS(text_block_parent_class)->dispose(object);
}

static void
text_block_finalize(GObject *object)
{
    G_OBJECT_CLASS(text_block_parent_class)->finalize(object);
}
