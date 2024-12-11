#include "./mb_picture.h"

/* Private definition */

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
	/* Properties */
	gchar *picture_path;
};

G_DEFINE_TYPE(MbPicture, mb_picture, GTK_TYPE_WIDGET)

/* Properties */

enum property_types
{
	PROP_PICTURE_PATH = 1,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

static void get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
	MbPicture *self = MB_PICTURE(object);

	switch(property_id)
	{
		case PROP_PICTURE_PATH:
		{
			g_value_set_string(value, self->picture_path);
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
		case PROP_PICTURE_PATH:
		{
			g_free(self->picture_path);
			self->picture_path = g_value_dup_string(value);
			self->picture = gtk_picture_new_for_filename(self->picture_path);
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

GtkWidget* mb_picture_new(const gchar *picture_path)
{
	return g_object_new(MB_TYPE_PICTURE, "picture_path", picture_path, NULL);
}

/* Private implementation */

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

	properties[PROP_PICTURE_PATH] = g_param_spec_string("picture_path", NULL, NULL, NULL, G_PARAM_READWRITE);
	g_object_class_install_properties(object_class, N_PROPERTIES, properties);
	gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

static void mb_picture_init(MbPicture *self)
{
	self->picture = gtk_picture_new();
	gtk_widget_set_hexpand(self->picture, TRUE);
	gtk_widget_set_vexpand(self->picture, TRUE);
	gtk_widget_set_parent(self->picture, GTK_WIDGET(self));
}
