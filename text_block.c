#include "text_block.h"

gboolean
key_pressed (
  GtkEventControllerKey* self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
)
{
    GtkTextIter start, end;
    GtkWidget *text_view = (GtkWidget*) user_data;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextMark *mark = gtk_text_buffer_get_insert(buffer);

    gtk_text_buffer_get_iter_at_mark(buffer, &start, mark);
    int line_number = gtk_text_iter_get_line(&start);
    gtk_text_iter_set_line(&start, line_number);
    end = start;
    gtk_text_iter_forward_to_line_end(&end);
    char* text = gtk_text_buffer_get_text(
        buffer,
        &start,
        &end,
        false
    );
    g_print("Line: %s\n", text);
    return GDK_EVENT_PROPAGATE;
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

    g_signal_connect(self->key_event_controller, "key-pressed", G_CALLBACK(key_pressed), self->text_view);
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
