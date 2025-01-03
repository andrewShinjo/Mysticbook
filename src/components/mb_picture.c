#include "./mb_picture.h"

/* Private definition */

static void drag_begin(GtkGestureDrag* self, gdouble start_x, gdouble start_y, gpointer user_data);
static void drag_update(GtkGestureDrag *drag, gdouble offset_x, gdouble offset_y, gpointer user_data);
static void pressed(GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer user_data);
static void notify_default_width(GObject *object, GParamSpec *pspec, gpointer user_data);
static void notify_path(GObject *object, GParamSpec *pspec, gpointer user_data);
static void dispose(GObject *object);
static void finalize(GObject *object);
static void get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void layout(GdkSurface* self, gint width, gint height, gpointer user_data);

/* Widget definition */

struct _MbPicture
{
	GtkWidget parent;
	/* Widgets */
	GtkWidget *picture;
	GtkTextView *text_view_reference;
	GtkWindow *window_reference;
	/* Event listeners */
	GtkGesture *click_listener;
	GtkGesture *drag_listener;
	/* Properties */
	gchar *path;
	/* Other fields */
	gdouble aspect_ratio;
	gdouble start_x;
	gdouble start_y;

	gdouble desired_width;
};

G_DEFINE_TYPE(MbPicture, mb_picture, GTK_TYPE_WIDGET)

/* Properties */

enum property_types
{
	PROP_PATH = 1,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

static void get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
	MbPicture *self = MB_PICTURE(object);

	switch(property_id)
	{
		case PROP_PATH:
		{
			g_value_set_string(value, self->path);
			break;
		}
		default:
		{
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
			break;
		}
	}
}

static void set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
	MbPicture *self = MB_PICTURE(object);

	switch(property_id)
	{
		case PROP_PATH:
		{
			g_free(self->path);
			self->path = g_value_dup_string(value);
			gtk_picture_set_filename(GTK_PICTURE(self->picture), self->path);
			break;
		}
		default:
		{
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
			break;
		}
	}
}

/* Signals */

/* Public implementation */

GtkWidget* mb_picture_new(const gchar *path)
{
	return g_object_new(MB_TYPE_PICTURE, "path", path, NULL);
}

void mb_picture_set_text_view(MbPicture *self, GtkTextView *text_view)
{
	self->text_view_reference = text_view;
	gint window_width = gtk_widget_get_width(GTK_WIDGET(self->window_reference));
	gint text_view_width = gtk_widget_get_width(GTK_WIDGET(self->text_view_reference));

	gint width = 0.80 * text_view_width;
	gint height = width / self->aspect_ratio;

	gtk_widget_set_size_request(self->picture, width, height);
}

void mb_picture_set_window(MbPicture *self, GtkWindow *window_reference)
{
	self->window_reference = window_reference;
	GdkSurface *surface = gtk_native_get_surface(GTK_NATIVE(window_reference));
	GdkToplevel *top_level = GDK_TOPLEVEL(surface);

	g_signal_connect(surface, "layout", G_CALLBACK(layout), self);
	g_signal_connect(self->window_reference, "notify::default-width", G_CALLBACK(notify_default_width), self);
}

/* Private implementation */

static void drag_begin(GtkGestureDrag* drag, gdouble start_x, gdouble start_y, gpointer user_data)
{
	GtkWidget *self = GTK_WIDGET(user_data);
	MB_PICTURE(self)->start_x = start_x;
	MB_PICTURE(self)->start_y = start_y;
}

static void drag_update(GtkGestureDrag *drag, gdouble offset_x, gdouble offset_y, gpointer user_data)
{
	MbPicture *self = MB_PICTURE(user_data);
	gdouble start_x = self->start_x;
	gdouble start_y = self->start_y;

	// Re-calculate image's desired width.
	self->desired_width = start_x + offset_x;

}

static void pressed(GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer user_data)
{
	// Do nothing.
}

static void notify_default_width(GObject *object, GParamSpec *pspec, gpointer user_data)
{
	g_print("notify_default_width\n");
	MbPicture *self = MB_PICTURE(user_data);

	// Calculate image's maximum width.
	GtkTextView *text_view = self->text_view_reference;
}

static void notify_path(GObject *object, GParamSpec *pspec, gpointer user_data)
{
	g_print("notify_path\n");
	MbPicture *self = MB_PICTURE(user_data);
	GdkPaintable *paintable = gtk_picture_get_paintable(GTK_PICTURE(self->picture));
	gint height = gdk_paintable_get_intrinsic_height(paintable);
	gint width = gdk_paintable_get_intrinsic_width(paintable);
	self->desired_width = width;
	self->aspect_ratio = (gdouble) width / height;
}

static void dispose(GObject *object)
{
	MbPicture *self = MB_PICTURE(object);
	g_clear_pointer(&self->picture, gtk_widget_unparent);
	G_OBJECT_CLASS(mb_picture_parent_class)->dispose(object);
}

static void finalize(GObject *object)
{
	G_OBJECT_CLASS(mb_picture_parent_class)->finalize(object);
}

static void mb_picture_class_init(MbPictureClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->get_property = get_property;
	object_class->set_property = set_property;

	properties[PROP_PATH] = g_param_spec_string("path", NULL, NULL, NULL, G_PARAM_READWRITE);
	g_object_class_install_properties(object_class, N_PROPERTIES, properties);
	gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

static void mb_picture_init(MbPicture *self)
{
	g_print("mb_picture_init\n");

	self->picture = gtk_picture_new();
	self->click_listener = gtk_gesture_click_new();
	self->drag_listener = gtk_gesture_drag_new();

	gtk_widget_set_parent(self->picture, GTK_WIDGET(self));

	gtk_widget_add_controller(self->picture, GTK_EVENT_CONTROLLER(self->click_listener));
	gtk_widget_add_controller(self->picture, GTK_EVENT_CONTROLLER(self->drag_listener));

	g_signal_connect(self->drag_listener, "drag_begin", G_CALLBACK(drag_begin), self);
	g_signal_connect(self->drag_listener, "drag_update", G_CALLBACK(drag_update), self);
	g_signal_connect(self->click_listener, "pressed", G_CALLBACK(pressed), self->picture);
	g_signal_connect(self, "notify::path", G_CALLBACK(notify_path), self);
}

static void layout(GdkSurface* surface, gint width, gint height, gpointer user_data)
{
	MbPicture *self = MB_PICTURE(user_data);
	g_print("GdkSurface::layout: width: %d, height: %d\n", width, height);
}
