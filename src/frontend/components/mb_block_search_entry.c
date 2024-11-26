#include "./mb_block_search_entry.h"

/* Widget definition */
struct _MbBlockSearchEntry
{
	GtkWidget parent;
	/* Widgets */
	GtkWidget *label;
	/* Event listeners */
	/* Properties */
	gchar *content;
	gint64 id;
};

G_DEFINE_TYPE(MbBlockSearchEntry, mb_block_search_entry, GTK_TYPE_WIDGET)

/* Forward declaration */
static void dispose(GObject *object);
static void finalize(GObject *object);
/* Callback */
/* Properties */
enum property_types
{
	PROP_ID = 1,
	PROP_CONTENT,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

static void get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
	MbBlockSearchEntry *self = MB_BLOCK_SEARCH_ENTRY(object);
	switch(property_id)
	{
		case PROP_CONTENT:
		{
			g_value_set_string(value, self->content);
			break;
		}
		case PROP_ID:
		{
			g_value_set_int64(value, self->id);
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
	MbBlockSearchEntry *self = MB_BLOCK_SEARCH_ENTRY(object);	
	switch(property_id)
	{
		case PROP_CONTENT:
		{
			g_free(self->content);
			self->content = g_value_dup_string(value);
			gtk_label_set_text(GTK_LABEL(self->label), self->content);
			break;
		}
		case PROP_ID:
		{
			self->id = g_value_get_int64(value);
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
/* Widget lifecycle */
static void mb_block_search_entry_init(MbBlockSearchEntry *self)
{
	/* Instantiate widgets */
	self->label = gtk_label_new(NULL);
	/* Configure widgets */
	gtk_widget_set_parent(self->label, GTK_WIDGET(self));
	/* Connect to signals */
}

static void mb_block_search_entry_class_init(MbBlockSearchEntryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	/* Map virtual functions */
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->get_property = get_property;
	object_class->set_property = set_property;
	/* Properties */
	properties[PROP_CONTENT] = g_param_spec_string("content", NULL, NULL, NULL, G_PARAM_READWRITE);
	properties[PROP_ID] = g_param_spec_int64("id", NULL, NULL, G_MININT64, G_MAXINT64, 0, G_PARAM_READWRITE);
	g_object_class_install_properties(object_class, N_PROPERTIES, properties);
	/* Signals */
	/* Layout manager */
	gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);	
}

static void dispose(GObject *object)
{
	G_OBJECT_CLASS(mb_block_search_entry_parent_class)->dispose(object);
}

static void finalize(GObject *object)
{
	G_OBJECT_CLASS(mb_block_search_entry_parent_class)->finalize(object);
}

/* Public implementation */
GtkWidget* mb_block_search_entry_new(const gchar *content, gint64 id)
{
	return g_object_new(MB_TYPE_BLOCK_SEARCH_ENTRY, "content", content, "id", id, NULL);
}
/* Private implementation */
