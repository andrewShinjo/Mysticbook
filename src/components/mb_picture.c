#include "./mb_picture.h"

/* Private definition */

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
	GtkWidget *overlay;
	GtkWidget *button;
	GtkWidget *picture;
	/* Event listeners */
	GtkGesture *click_listener;
	/* Properties */
	gchar *path;
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
			g_print("PROP_path\n");
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

/* Private implementation */

static void pressed(GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer user_data)
{
	g_print("Pressed\n");
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
	g_object_class_install_properties(object_class, N_PROPERTIES, properties);
	gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

static void mb_picture_init(MbPicture *self)
{
	self->overlay = gtk_overlay_new();
	self->button = gtk_button_new_with_label("Button");
	self->picture = gtk_picture_new();
	self->click_listener = gtk_gesture_click_new();

	gtk_overlay_set_child(GTK_OVERLAY(self->overlay), self->picture);
	gtk_overlay_add_overlay(GTK_OVERLAY(self->overlay), self->button);
	gtk_widget_set_halign(self->button, GTK_ALIGN_END);
	gtk_widget_set_valign(self->button, GTK_ALIGN_START);
	gtk_widget_set_size_request(self->button, 50, 50);

	gtk_widget_add_controller(self->picture, GTK_EVENT_CONTROLLER(self->click_listener));
	gtk_widget_set_parent(self->overlay, GTK_WIDGET(self));
	g_signal_connect(self->click_listener, "pressed", G_CALLBACK(pressed), NULL);
	g_signal_connect(self, "notify::path", G_CALLBACK(notify_path), self);
}
