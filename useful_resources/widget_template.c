#include "./mb_xxx.h"

/* WIDGET DEFINITION */
struct _MbXXX
{
  GtkWidget parent;
  /* WIDGETS */
  /* EVENT LISTENERS */
  /* PROPERTIES */
};

G_DEFINE_TYPE(MbXXX, mb_xxx, GTK_TYPE_WIDGET)

/* FORWARD DECLARATION */
/* CALLBACK */

/* PROPERTIES */
enum property_types
{
  FIRST_PROPERTY = 1,
  N_PROPERTIES
};
static GParamSpec *properties[N_PROPERTIES];
static void
mb_xxx_get_property(
  GObject *object,
  guint property_id,
  GValue *value,
  GParamSpec *pspec
)
{
  switch(property_id)
  {
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}
static void
mb_xxx_set_property(
  GObject *object,
  guint property_id,
  const GValue *value,
  GParamSpec *pspec
)
{
  switch(property_id)
  {
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}

/* SIGNALS */

enum signal_types
{
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

static gboolean
signal_name_source_func(gpointer user_data)
{
  return G_SOURCE_CONTINUE;
}


/* WIDGET LIFECYCLE */

static void
mb_xxx_init(MbXxx *_self) 
{
  /* INSTANTIATE WIDGETS */
  /* CONFIGURE WIDGETS */
  /* CONNECT TO SIGNALS */
}

static void
mb_xxx_class_init(MbXxxClass *klass) 
{
  /* MAP VFUNC */
  /* PROPERTY */
  /* SIGNAL */
  /* LAYOUT MANAGER */
}

static void 
mb_xxx_dispose(GObject *object) 
{

}

static void
mb_xxx_finalize(GObject *object)
{

}
/* PUBLIC IMPLEMENTATION */

/* PRIVATE IMPLEMENTATION */
