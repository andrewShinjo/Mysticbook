#include <string.h>
#include "text_block.h"

gulong handler_id;

/* Editor private functions */

static int
heading_level(const char *heading)
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
is_heading(const char* line)
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

/*
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
    int heading_level = heading_level(line);
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

    if(is_heading(line))
    {
        int heading_level2 = heading_level(line);
        
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
            if(!is_heading(line2))
            {
                continue;
            }
            int heading_level2 = heading_level(line2);
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
*/

/* Signal callbacks */

void
changed (
  GtkTextBuffer* self,
  gpointer user_data
)
{
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(self);
    GtkTextTag *heading_tag = gtk_text_tag_table_lookup(tag_table, "heading");
    GtkTextIter start, end;

    // Remove all tags
    {
        GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(self);
        GtkTextTag *heading_tag = gtk_text_tag_table_lookup(tag_table, "heading");
        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(self, &start);
        gtk_text_buffer_get_end_iter(self, &end);
        gtk_text_buffer_remove_all_tags (self, &start, &end);
    }

    // Re-apply header tags
    {
        end = start;

        while(!gtk_text_iter_is_end(&end))
        {
            if(gtk_text_iter_ends_line(&end))
            {
                gtk_text_iter_forward_line(&start);
                end = start;
                continue;
            }

            gtk_text_iter_forward_to_line_end(&end);

            gchar *line = gtk_text_buffer_get_text(self, &start, &end, FALSE);
            if(is_heading(line))
            {
                gtk_text_buffer_apply_tag(self, heading_tag, &start, &end);
            }
    
            g_free(line);
            
            gtk_text_iter_forward_line(&start);
            gtk_text_iter_forward_line(&end);
        }
    }
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
    GtkTextBuffer *text_buffer = (GtkTextBuffer *) user_data;
    GtkTextMark *insert_mark = gtk_text_buffer_get_insert(text_buffer);
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(text_buffer);
    GtkTextTag *fold_tag = gtk_text_tag_table_lookup(tag_table, "fold");

    GtkTextIter insert_iter, left_iter, right_iter, fold_start, fold_end;
    gtk_text_buffer_get_iter_at_mark(text_buffer, &insert_iter, insert_mark);
    left_iter = insert_iter;
    gtk_text_iter_set_line_offset(&left_iter, 0);
    right_iter = left_iter;

    if(!gtk_text_iter_ends_line(&right_iter))
    {
        gtk_text_iter_forward_to_line_end(&right_iter);
    }

    gchar *line = gtk_text_iter_get_text(&left_iter, &right_iter);

    if(keyval == GDK_KEY_Tab)
    {
        if(is_heading(line))
        {

            gtk_text_iter_forward_line(&left_iter);

            if(gtk_text_iter_is_end(&left_iter)) {
                return GDK_EVENT_STOP;
            }

            right_iter = left_iter;

            gtk_text_iter_forward_to_line_end(&right_iter);
            gchar *line2 = gtk_text_iter_get_text(&left_iter, &right_iter);

            if(is_heading(line2) && heading_level(line) >= heading_level(line2)) 
            {
                g_free(line2);
                return GDK_EVENT_STOP;
            }

            g_free(line2);

            fold_start = left_iter;
            fold_end = right_iter;

            while(true)
            {
                gtk_text_iter_forward_line(&left_iter);

                if(gtk_text_iter_is_end(&left_iter))
                {
                    break;
                }
                right_iter = left_iter;
                gtk_text_iter_forward_to_line_end(&right_iter);
                line2 = gtk_text_iter_get_text(&left_iter, &right_iter);
                if(is_heading(line2) && heading_level(line) >= heading_level(line2)) 
                {
                    g_free(line2);
                    break;
                }
                g_free(line2);
                fold_end = right_iter;
            }
            
            if(gtk_text_iter_has_tag(&fold_start, fold_tag) || gtk_text_iter_has_tag(&fold_end, fold_tag))
            {
                gtk_text_buffer_remove_tag(text_buffer, fold_tag, &fold_start, &fold_end);
            }
            else
            {
                gtk_text_buffer_apply_tag(text_buffer, fold_tag, &fold_start, &fold_end);
            }
            
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
{}


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
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);

    gtk_text_view_set_bottom_margin(text_view, 10);
    gtk_text_view_set_left_margin(text_view, 10);
    gtk_text_view_set_right_margin(text_view, 10);
    gtk_text_view_set_top_margin(text_view, 10);
    gtk_widget_add_controller(self->text_view, self->key_event_controller);
    gtk_widget_set_hexpand(self->text_view, true);
    gtk_widget_set_parent(self->text_view, widget);

    // Initialize tag table.
    {

        GtkTextTag *fold_tag = gtk_text_buffer_create_tag(
            buffer, 
            "fold", 
            "invisible", 
            true, 
            NULL
        );

        GtkTextTag *heading_tag = gtk_text_buffer_create_tag(
            buffer,
            "heading",
            "foreground",
            "purple",
            NULL
        );
    }

    // Initialize signals
    {
        GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);
        
        g_signal_connect(
            self->key_event_controller, 
            "key-pressed", 
            G_CALLBACK(key_pressed), 
            text_buffer
        );

        // g_signal_connect(
        //     self->key_event_controller,
        //     "key-released",
        //     G_CALLBACK(key_released),
        //     self->text_view
        // ); 

        handler_id = g_signal_connect(
            buffer,
            "changed",
            G_CALLBACK(changed),
            NULL
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
