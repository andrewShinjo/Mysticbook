#include <string.h>
#include "text_block.h"
#include "util.h"

/* Editor private data */
guint last_keyval = 0;

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

/* Signal callbacks */

void
notify_on_cursor_position_change (
  GObject* self,
  GParamSpec* pspec,
  gpointer user_data
)
{
    GtkTextBuffer* buffer = GTK_TEXT_BUFFER(self);
    editor_print_buffer(buffer);
}

void
changed (
  GtkTextBuffer* self,
  gpointer user_data
)
{
    int line_number = editor_get_cursor_position_line_number(self);
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(self);
    GtkTextTag *heading_tag = gtk_text_tag_table_lookup(tag_table, "heading");
    
    gchar *text = editor_get_text_at_line(self, line_number);

    if(editor_is_line_heading(text))
    {
        editor_remove_tag_from_line(self, heading_tag, line_number);
        editor_apply_tag_to_line(self, heading_tag, line_number);
    }
    else
    {
        editor_remove_tag_from_line(self, heading_tag, line_number);
    }

    g_free(text);

    text = editor_get_text_at_line(self, line_number-1);

    if(editor_is_line_heading(text))
    {
        editor_remove_tag_from_line(self, heading_tag, line_number-1);
        editor_apply_tag_to_line(self, heading_tag, line_number-1);
    }
    else
    {
        editor_remove_tag_from_line(self, heading_tag, line_number-1);
    }

    g_free(text);

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
    last_keyval = keyval;
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
            NULL
        );

        g_signal_connect(
            text_buffer,
            "notify::cursor-position",
            G_CALLBACK(notify_on_cursor_position_change),
            NULL
        ); 

        g_signal_connect(
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
