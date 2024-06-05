#include <string.h>
#include "text_block.h"

/* Editor private functions */

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
}

static void
editor_hide_content_under_current_heading(GtkTextView *text_view)
{
    GtkTextIter start, end;
    GtkTextBuffer *buffer;
    GtkTextMark *mark;
    int line_number;

    buffer = gtk_text_view_get_buffer(text_view);
    mark = gtk_text_buffer_get_insert(buffer);

    gtk_text_buffer_get_iter_at_mark(buffer, &start, mark);
    // gtk_text_buffer_get_iter_at_mark(buffer, &end, mark);
    // gtk_text_iter_set_line_offset(&start, 0);
    // gtk_text_iter_forward_to_line_end(&end);

    while(!gtk_text_iter_is_end(&start))
    {
        // gtk_text_iter_assign(&start, &end);
        // gtk_text_iter_forward_to_line_end(&end);
        // gchar *text = gtk_text_iter_get_text(&start, &end);
        // g_print("Text: %s\n", text);
        // g_free(text);
        gtk_text_iter_forward_line(&start);
        // TODO(andy): print text from buffer line-by-line under the current heading.
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
    GtkWidget *text_view = (GtkWidget*) user_data;

    switch(keyval)
    {
        case GDK_KEY_Tab:
        {
            // Is current line a heading?
            const char *current_line = editor_get_current_line(GTK_TEXT_VIEW(text_view));
            gboolean is_heading = editor_is_line_a_heading(current_line);
            if(is_heading)
            {
                editor_hide_content_under_current_heading(GTK_TEXT_VIEW(text_view));
            }
            else {
                // Do nothing.
            }
            return GDK_EVENT_STOP;
        } break;

        default:
        {
            return GDK_EVENT_PROPAGATE;
        } break;
    }
    
    // GtkWidget *text_view = (GtkWidget*) user_data;
    // char* text = editor_get_current_line(GTK_TEXT_VIEW(text_view));
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
    // GtkWidget *text_view = (GtkWidget*) user_data;
    // const char* text = editor_get_current_line(GTK_TEXT_VIEW(text_view));
    // gboolean is_editor = editor_is_line_a_heading(text);
    // g_print("Line: %s, is_heading: %s\n", text, is_editor ? "TRUE":"FALSE");
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

    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(self->text_view), 10);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(self->text_view), 10);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(self->text_view), 10);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(self->text_view), 10);
    gtk_widget_add_controller(self->text_view,self->key_event_controller);
    gtk_widget_set_hexpand(self->text_view, true);
    gtk_widget_set_parent(self->text_view, widget);

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
