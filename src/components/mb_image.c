#include "./mb_image.h"

/* Private definition */

static void dispose(GObject *object);
static void finalize(GObject *object);
static void get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

/* Widget definition */

struct _MbImage
{
	GtkWidget parent;
	/* Widgets */
	GtkWidget *image;
	/* Properties */
	gchar *image_path;
};

G_DEFINE_TYPE(MbImage, mb_image, GTK_TYPE_WIDGET)

/* Properties */

enum property_types
{
	PROP_IMAGE_PATH = 1,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

static void get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
	MbImage *self = MB_IMAGE(object);

	switch(property_id)
	{
		case PROP_IMAGE_PATH:
		{
			g_value_set_string(value, self->image_path);
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
	MbImage *self = MB_IMAGE(object);

	switch(property_id)
	{
		case PROP_IMAGE_PATH:
		{
			g_free(self->image_path);
			self->image_path = g_value_dup_string(value);
			gtk_image_clear(GTK_IMAGE(self->image));
			gtk_image_set_from_file(GTK_IMAGE(self->image), self->image_path);
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

GtkWidget* mb_image_new(gchar *image_path)
{
	return g_object_new(MB_TYPE_IMAGE, "image_path", image_path, NULL);
}

/* Private implementation */

static void dispose(GObject *object)
{
	MbImage *self = MB_IMAGE(object);
	g_clear_pointer(&self->image, gtk_widget_unparent);
	G_OBJECT_CLASS(mb_image_parent_class)->dispose(object);
}

static void finalize(GObject *object)
{
	G_OBJECT_CLASS(mb_image_parent_class)->finalize(object);
}

static void mb_image_class_init(MbImageClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->get_property = get_property;
	object_class->set_property = set_property;

	properties[PROP_IMAGE_PATH] = g_param_spec_string("image_path", NULL, NULL, NULL, G_PARAM_READWRITE);
	g_object_class_install_properties(object_class, N_PROPERTIES, properties);
	gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

static void mb_image_init(MbImage *self)
{
	self->image = gtk_image_new();
	gtk_widget_set_hexpand(self->image, TRUE);
	gtk_widget_set_vexpand(self->image, TRUE);
	gtk_widget_set_parent(self->image, GTK_WIDGET(self));
}
