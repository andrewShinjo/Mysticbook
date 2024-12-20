#include "./mb_picture.h"

/* Private definition */

static void drag_begin(GtkGestureDrag* self, gdouble start_x, gdouble start_y, gpointer user_data);
static void pressed(GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer user_data);
static void notify_path(GObject *object, GParamSpec *pspec, gpointer user_data);
static void dispose(GObject *object);
static void finalize(GObject *object);
static void get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

/* Widget definition */

struct _MbPicture
{
	GtkWidget parent;
	/* Widgets */
	GtkWidget *picture;

	/* Event listeners */
	GtkGesture *click_listener;
	GtkGesture *drag_listener;
	/* Properties */
	gchar *path;
	gboolean resizing;
	gdouble last_x;
	gdouble last_y;
};

G_DEFINE_TYPE(MbPicture, mb_picture, GTK_TYPE_WIDGET)

/* Properties */

enum property_types
{
	PROP_PATH = 1,
	PROP_RESIZING,
	PROP_LASTX,
	PROP_LASTY,
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
		case PROP_RESIZING:
		{
			g_value_set_boolean(value, self->resizing);
			break;
		}
		case PROP_LASTX:
		{
			g_value_set_double(value, self->last_x);
			break;
		}
		case PROP_LASTY:
		{
			g_value_set_double(value, self->last_y);
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
		case PROP_RESIZING:
		{
			self->resizing = g_value_get_boolean(value);
			break;
		}
		case PROP_LASTX:
		{
			self->last_x = g_value_get_double(value);
			break;
		}
		case PROP_LASTY:
		{
			self->last_y = g_value_get_double(value);
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

/* Private implementation */

static void drag_begin(GtkGestureDrag* self, gdouble start_x, gdouble start_y, gpointer user_data)
{
	GtkWidget *picture = GTK_WIDGET(user_data);
	gint height = gtk_widget_get_height(picture);
	gint width = gtk_widget_get_width(picture);

	gboolean IS_TOP_LEFT_CORNER = (start_x <= 10 && start_y <= 10);
	gboolean IS_TOP_RIGHT_CORNER = width - start_x <= 10 && start_y <= 10;
	gboolean IS_BOTTOM_LEFT_CORNER = start_x <= 10 && height - start_y <= 10;
	gboolean IS_BOTTOM_RIGHT_CORNER = width - start_x <= 10 && height - start_y <= 10;

	if(IS_TOP_LEFT_CORNER) g_print("Drag TOP LEFT CORNER\n");
	else if(IS_TOP_RIGHT_CORNER) g_print("Drag TOP RIGHT CORNER\n");
	else if(IS_BOTTOM_LEFT_CORNER) g_print("Drag BOTTOM LEFT CORNER\n");
	else if(IS_BOTTOM_RIGHT_CORNER) 
	{
		g_print("Drag BOTTOM RIGHT CORNER\n");
		gtk_widget_set_size_request(picture, width + 10, height + 10);
	}
}

static void pressed(GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer user_data)
{
	// Do nothing.
}

static void notify_path(GObject *object, GParamSpec *pspec, gpointer user_data)
{
	MbPicture *self = MB_PICTURE(user_data);
	GdkPaintable *paintable = gtk_picture_get_paintable(GTK_PICTURE(self->picture));
	gint height = gdk_paintable_get_intrinsic_height(paintable);
	gint width = gdk_paintable_get_intrinsic_width(paintable);
	gtk_widget_set_size_request(self->picture, width, height);
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
	properties[PROP_RESIZING] = g_param_spec_boolean("resizing", NULL, NULL, FALSE, G_PARAM_READWRITE);
	properties[PROP_LASTX] = 
		g_param_spec_double("last-x", NULL, NULL, -G_MAXDOUBLE, G_MAXDOUBLE, 0, G_PARAM_READWRITE);
	properties[PROP_LASTY] =
		g_param_spec_double("last-y", NULL, NULL, -G_MAXDOUBLE, G_MAXDOUBLE, 0, G_PARAM_READWRITE);
	g_object_class_install_properties(object_class, N_PROPERTIES, properties);
	gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

static void mb_picture_init(MbPicture *self)
{
	self->picture = gtk_picture_new();
	self->click_listener = gtk_gesture_click_new();
	self->drag_listener = gtk_gesture_drag_new();

	gtk_widget_add_controller(self->picture, GTK_EVENT_CONTROLLER(self->click_listener));
	gtk_widget_add_controller(self->picture, GTK_EVENT_CONTROLLER(self->drag_listener));

	gtk_widget_set_parent(self->picture, GTK_WIDGET(self));

	g_signal_connect(self->drag_listener, "drag_begin", G_CALLBACK(drag_begin), self->picture);
	g_signal_connect(self->click_listener, "pressed", G_CALLBACK(pressed), self->picture);
	g_signal_connect(self, "notify::path", G_CALLBACK(notify_path), self);
}
