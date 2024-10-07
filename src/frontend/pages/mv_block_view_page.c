#include "./mb_block_view_page.h"
#include "../components/mb_text_block.h"
#include "../components/mb_root_text_block.h"

// idea: override snapshot method to draw rectangle

static void mb_block_view_page_dispose(GObject *object);
static void mb_block_view_page_finalize(GObject *object);
static void mb_block_view_page_snapshot(GtkWidget *widget, GtkSnapshot *snapshot);

struct _MbBlockViewPage
{
	GtkWidget parent;
  GtkWidget *overlay;
  GtkWidget *scrolled_window;
  GtkWidget *layout;
  GtkWidget *root_block;

  // Drag
  GtkGesture *gesture_drag;
  double x0;
  double x1;
  double y0;
  double y1;
  gboolean dragging;
};

G_DEFINE_TYPE(MbBlockViewPage, mb_block_view_page, GTK_TYPE_WIDGET)

// Private

static void
drag_begin(
  GtkGestureDrag *gesture_drag,
  gdouble start_x,
  gdouble start_y,
  gpointer user_data
)
{
  GtkWidget *self = GTK_WIDGET(user_data);
  MbBlockViewPage *_self = MB_BLOCK_VIEW_PAGE(user_data);
  g_print("drag_begin\n");
  _self->x0 = start_x;
  _self->y0 = start_y;
}

static void
drag_update(
  GtkGestureDrag *gesture_drag,
  gdouble offset_x,
  gdouble offset_y,
  gpointer user_data
)
{
  GtkWidget *self = GTK_WIDGET(user_data);
  MbBlockViewPage *_self = MB_BLOCK_VIEW_PAGE(user_data);
  g_print("drag_update\n");
  _self->dragging = TRUE;
  _self->x1 = offset_x;
  _self->y1 = offset_y;
  gtk_widget_queue_draw(self);
}

static void
drag_end(
  GtkGestureDrag *gesture_drag,
  gdouble offset_x,
  gdouble offset_y,
  gpointer user_data
)
{
  GtkWidget *self = GTK_WIDGET(user_data);
  MbBlockViewPage *_self = MB_BLOCK_VIEW_PAGE(user_data);
  g_print("drag_end\n");
  _self->x0 = 0;
  _self->x1 = 0;
  _self->y0 = 0;
  _self->y1 = 0;
  _self->dragging = FALSE;
  gtk_widget_queue_draw(self);
}

gboolean 
prepend_block(MbBlockViewPage *self, GtkWidget *block)
{
  return TRUE;
}

gboolean 
insert_block_after(MbBlockViewPage *self, GtkWidget *sibling, GtkWidget *insert)
{
  return TRUE;
}

/* Virtual functions */

static void mb_block_view_page_dispose(GObject *object) 
{
  MbBlockViewPage *self = MB_BLOCK_VIEW_PAGE(object);
  g_clear_pointer(&self->scrolled_window, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->dispose(object);
}

static void 
mb_block_view_page_snapshot(GtkWidget *widget, GtkSnapshot *snapshot)
{
  MbBlockViewPage *_self = MB_BLOCK_VIEW_PAGE(widget);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(mb_block_view_page_parent_class);

  widget_class->snapshot(widget, snapshot);

  if(_self->dragging)
  {
    g_print("Drawing rectangle.\n");
    g_print("x0=%f, y0=%f, x1=%f, y1=%f\n", _self->x0, _self->y0, _self->x1, _self->y1);
    graphene_rect_t graphene_rect = GRAPHENE_RECT_INIT(
        _self->x0, _self->y0, _self->x1, _self->y1
    );
    GdkRGBA color;
    gdk_rgba_parse(&color, "red");
    gtk_snapshot_append_color(snapshot, &color, &graphene_rect);
  }
}

static void 
mb_block_view_page_finalize(GObject *object)
{
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->finalize(object);
}

static void 
mb_block_view_page_init(MbBlockViewPage *self)
{
  self->scrolled_window = gtk_scrolled_window_new();
  self->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);  
  self->root_block = mb_root_text_block_new();
  gtk_widget_set_hexpand(self->scrolled_window, TRUE);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(self->scrolled_window), self->layout);
  gtk_box_append(GTK_BOX(self->layout), self->root_block);
  gtk_widget_set_parent(self->scrolled_window, GTK_WIDGET(self));

  self->gesture_drag = gtk_gesture_drag_new();
  gtk_widget_add_controller(GTK_WIDGET(self), GTK_EVENT_CONTROLLER(self->gesture_drag));
  
  self->x0 = 0;
  self->x1 = 0;
  self->y0 = 0;
  self->y1 = 0;
  self->dragging = FALSE;

  g_signal_connect(self->gesture_drag, "drag-begin", G_CALLBACK(drag_begin), self);
  g_signal_connect(self->gesture_drag, "drag-update", G_CALLBACK(drag_update), self);
  g_signal_connect(self->gesture_drag, "drag-end", G_CALLBACK(drag_end), self);
}
static void mb_block_view_page_class_init(MbBlockViewPageClass *klass) 
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  
  object_class->dispose = mb_block_view_page_dispose;
  object_class->finalize = mb_block_view_page_finalize;
  widget_class->snapshot = mb_block_view_page_snapshot;
  gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

// Public

GtkWidget *mb_block_view_page_new()
{
	return g_object_new(MB_TYPE_BLOCK_VIEW_PAGE, NULL);
}
