#include "./mb_block_view_page.h"
#include "../components/mb_text_block.h"
#include "../components/mb_root_text_block.h"
#include "../../backend/block.h"

enum property_types
{
  PROP_ID = 1,
  N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

static void 
dispose(GObject *object);

static void 
finalize(GObject *object);

static void 
set_property(
  GObject *object,
  guint property_id,
  const GValue *value,
  GParamSpec *pspec
);

static void 
snapshot(GtkWidget *widget, GtkSnapshot *snapshot);

struct _MbBlockViewPage
{
	GtkWidget parent;
  GtkWidget *overlay;
  GtkWidget *scrolled_window;
  GtkWidget *layout;
  GtkWidget *root_block;

  gint64 id;

  // Drag
  GtkGesture *gesture_drag;
  double x0;
  double x1;
  double y0;
  double y1;
  gboolean dragging;
};

G_DEFINE_TYPE(MbBlockViewPage, mb_block_view_page, GTK_TYPE_WIDGET)

/* Callback */

static void 
notify_id(
  GObject *object, 
  GParamSpec *pspec, 
  gpointer user_data
)
{
  MbBlockViewPage *_self = MB_BLOCK_VIEW_PAGE(object);
  g_print("MbBlockViewPage notify_id: id=%ld\n", _self->id);
  g_object_set(_self->root_block, "id", _self->id, NULL);
  // Block b;
  // block_find_by_id(_self->id, &b);
  // mb_root_text_block_set_content(
  //   MB_ROOT_TEXT_BLOCK(_self->root_block),
  //   b.content
  // );
}

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
  _self->x0 = start_x;
  _self->y0 = start_y;
}

static void drag_update(
  GtkGestureDrag *gesture_drag,
  gdouble offset_x,
  gdouble offset_y,
  gpointer user_data
)
{
  GtkWidget *self = GTK_WIDGET(user_data);
  MbBlockViewPage *_self = MB_BLOCK_VIEW_PAGE(user_data);
  _self->dragging = TRUE;
  _self->x1 = offset_x;
  _self->y1 = offset_y;
  gtk_widget_queue_draw(self);
}

static void drag_end(
  GtkGestureDrag *gesture_drag,
  gdouble offset_x,
  gdouble offset_y,
  gpointer user_data
)
{
  GtkWidget *self = GTK_WIDGET(user_data);
  MbBlockViewPage *_self = MB_BLOCK_VIEW_PAGE(user_data);
  _self->x0 = 0.0;
  _self->x1 = 0.0;
  _self->y0 = 0.0;
  _self->y1 = 0.0;
  _self->dragging = FALSE;
  gtk_widget_queue_draw(self);
}

gboolean prepend_block(MbBlockViewPage *self, GtkWidget *block)
{
  return TRUE;
}

gboolean insert_block_after(
  MbBlockViewPage *self, 
  GtkWidget *sibling, 
  GtkWidget *insert
)
{
  return TRUE;
}

/* Virtual functions */

static void dispose(GObject *object) 
{
  MbBlockViewPage *self = MB_BLOCK_VIEW_PAGE(object);
  g_clear_pointer(&self->scrolled_window, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->dispose(object);
}

static void snapshot(GtkWidget *widget, GtkSnapshot *snapshot)
{
  MbBlockViewPage *_self = MB_BLOCK_VIEW_PAGE(widget);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(
    mb_block_view_page_parent_class
  );

  widget_class->snapshot(widget, snapshot);

  if(_self->dragging)
  {
    graphene_rect_t graphene_rect = GRAPHENE_RECT_INIT(
      _self->x0, 
      _self->y0, 
      _self->x1, 
      _self->y1
    );
    GdkRGBA color;
    gdk_rgba_parse(&color, "rgba(255, 0, 0, 0.25)");
    gtk_snapshot_append_color(snapshot, &color, &graphene_rect);
  }
}

static void finalize(GObject *object)
{
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->finalize(object);
}

static void set_property(
  GObject *object,
  guint property_id,
  const GValue *value,
  GParamSpec *pspec
)
{
  g_print("block_view_page, set_property\n");
  MbBlockViewPage *_self = MB_BLOCK_VIEW_PAGE(object);
  switch(property_id)
  {
    case PROP_ID:
    {
      _self->id = g_value_get_int64(value); 
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}

static void get_property(
  GObject *object,
  guint property_id,
  GValue *value,
  GParamSpec *pspec
)
{
  g_print("mb_block_view_page, get_property\n");
  MbBlockViewPage *_self = MB_BLOCK_VIEW_PAGE(object);
  switch(property_id)
  {
    case PROP_ID:
    {
      g_value_set_int64(value, _self->id);
      g_print("block_view_page, get_property=%ld\n", _self->id);
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}

static void mb_block_view_page_init(MbBlockViewPage *self)
{
  g_print("mb_block_view_page_init\n");
  /* INSTANTIATE WIDGETS */
  self->scrolled_window = gtk_scrolled_window_new();
  self->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);  
  self->root_block = mb_root_text_block_new();
  self->gesture_drag = gtk_gesture_drag_new();
  /* Get block data */
  /* CONFIGURE WIDGETS */
  gtk_widget_set_hexpand(self->scrolled_window, TRUE);
  gtk_scrolled_window_set_child(
    GTK_SCROLLED_WINDOW(self->scrolled_window), 
    self->layout
  );
  gtk_box_append(GTK_BOX(self->layout), self->root_block);
  gtk_widget_set_parent(self->scrolled_window, GTK_WIDGET(self));

  gtk_widget_add_controller(
    GTK_WIDGET(self), 
    GTK_EVENT_CONTROLLER(self->gesture_drag)
  );
  
  self->x0 = 0;
  self->x1 = 0;
  self->y0 = 0;
  self->y1 = 0;
  self->dragging = FALSE;
  /* CONNECT TO SIGNALS */
  g_signal_connect(
    GTK_WIDGET(self),
    "notify::id",
    G_CALLBACK(notify_id),
    NULL
  );
  g_signal_connect(
    self->gesture_drag, 
    "drag-begin", 
    G_CALLBACK(drag_begin), 
    self
  );
  g_signal_connect(
    self->gesture_drag, 
    "drag-update", 
    G_CALLBACK(drag_update), 
    self
  );
  g_signal_connect(
    self->gesture_drag, 
    "drag-end", 
    G_CALLBACK(drag_end), 
    self
  );
}
static void mb_block_view_page_class_init(MbBlockViewPageClass *klass) 
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GParamFlags default_flags = 
    G_PARAM_READWRITE | 
    G_PARAM_EXPLICIT_NOTIFY;

  object_class->dispose      = dispose;
  object_class->finalize     = finalize;
  object_class->get_property = get_property;
  object_class->set_property = set_property;
  widget_class->snapshot     = snapshot;

  properties[PROP_ID] = g_param_spec_int64(
    "id", 
    "id nickname", 
    "id blurb",
    G_MININT64,
    G_MAXINT64, 
    0, 
    G_PARAM_READWRITE
  );

  g_object_class_install_properties(
    object_class, 
    N_PROPERTIES, 
    properties
  );
  
    gtk_widget_class_set_layout_manager_type(
    GTK_WIDGET_CLASS(klass), 
    GTK_TYPE_BOX_LAYOUT
  );
}

// Public

GtkWidget *mb_block_view_page_new(gint64 id)
{
  g_print("mb_block_view_page_new, id=%ld\n", id);
	return g_object_new(
    MB_TYPE_BLOCK_VIEW_PAGE,
    "id", id,
    NULL
  );
}
