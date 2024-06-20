#include <string.h>
#include "text_block.h"
#include "util.h"

/* Editor private data */
int buffer_size_before_change = 1;

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
    int buffer_size_after_change = editor_get_line_count(self);
    int lines_added = buffer_size_after_change - buffer_size_before_change;
    int line_number = editor_get_cursor_position_line_number(self);
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(self);

    gchar *text = editor_get_text_at_line(self, line_number);

    if(editor_is_line_heading(text))
    {
        int heading_level = editor_get_heading_level(text);
        GtkTextTag *heading_tag;

        switch(heading_level % 5)
        {
            case 0:
            {
                heading_tag = gtk_text_tag_table_lookup(tag_table, "h5");
            } break;
            case 1:
            {
                heading_tag = gtk_text_tag_table_lookup(tag_table, "h1");
            } break;
            case 2:
            {
                heading_tag = gtk_text_tag_table_lookup(tag_table, "h2");
            } break;
            case 3:
            {
                heading_tag = gtk_text_tag_table_lookup(tag_table, "h3");
            } break;
            case 4:
            {
                heading_tag = gtk_text_tag_table_lookup(tag_table, "h4");
            } break;
        }

        editor_remove_all_tags_from_line(self, line_number);
        editor_apply_tag_to_line(self, heading_tag, line_number);
    }
    else
    {
        editor_remove_all_tags_from_line(self, line_number);
    }

    g_free(text);

    for(int i = 1; i <= lines_added; i++)
    {
        text = editor_get_text_at_line(self, line_number - i);

        if(editor_is_line_heading(text))
        {
            int heading_level = editor_get_heading_level(text);
            GtkTextTag *heading_tag;

            switch(heading_level % 5)
            {
                case 0:
                {
                    heading_tag = gtk_text_tag_table_lookup(tag_table, "h5");
                } break;
                case 1:
                {
                    heading_tag = gtk_text_tag_table_lookup(tag_table, "h1");
                } break;
                case 2:
                {
                    heading_tag = gtk_text_tag_table_lookup(tag_table, "h2");
                } break;
                case 3:
                {
                    heading_tag = gtk_text_tag_table_lookup(tag_table, "h3");
                } break;
                case 4:
                {
                    heading_tag = gtk_text_tag_table_lookup(tag_table, "h4");
                } break;
            }

            editor_remove_all_tags_from_line(self, line_number - 1);
            editor_apply_tag_to_line(self, heading_tag, line_number - 1);
        }
        else
        {
            editor_remove_all_tags_from_line(self, line_number - i);
        }

        g_free(text);
    }

    

    buffer_size_before_change = buffer_size_after_change;

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

        // GtkTextTag *fold_tag = gtk_text_buffer_create_tag(buffer, "fold", "invisible", true, NULL);
        GtkTextTag *heading_tag = gtk_text_buffer_create_tag(buffer, "heading", "foreground", "#00FFFF", NULL);
        GtkTextTag *h1_tag = gtk_text_buffer_create_tag(buffer, "h1", "foreground", "#00FFFF", NULL);
        GtkTextTag *h2_tag = gtk_text_buffer_create_tag(buffer, "h2", "foreground", "#32CD32", NULL);
        GtkTextTag *h3_tag = gtk_text_buffer_create_tag(buffer, "h3", "foreground", "#FF7F50", NULL);
        GtkTextTag *h4_tag = gtk_text_buffer_create_tag(buffer, "h4", "foreground", "#FF1493", NULL);
        GtkTextTag *h5_tag = gtk_text_buffer_create_tag(buffer, "h5", "foreground", "#DAA520", NULL);
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
