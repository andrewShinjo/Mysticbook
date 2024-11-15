#include "./mb_text_buffer.h"
#include "../../backend/service/block_service.h"

/* Widget definition */
struct _MbTextBuffer
{
  GtkTextBuffer parent;
  /* Widgets */
  /* Event listeners */
  /* Properties */
  gint64 id;
};
G_DEFINE_TYPE(MbTextBuffer, mb_text_buffer, GTK_TYPE_TEXT_BUFFER)
/* Forward declaration */
static void mb_text_buffer_dispose(GObject *object);
static void mb_text_buffer_finalize(GObject *object);
/* Callback */
static void changed(GtkTextBuffer *_self, gpointer user_data)
{

}
static void notify_id(GObject *object, GParamSpec *pspec, gpointer user_data)
{
  MbTextBuffer *_self = MB_TEXT_BUFFER(user_data);
  const gchar *content = block_service_get_block_content(_self->id);
  if(content != NULL)
  {
    GtkTextBuffer *self = GTK_TEXT_BUFFER(_self);
    gtk_text_buffer_set_text(self, content, -1);
  }
}
/* Properties */
enum property_types
{
  PROP_ID = 1,
  N_PROPERTIES
};
static GParamSpec *properties[N_PROPERTIES];
static void mb_text_buffer_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  MbTextBuffer *_self = MB_TEXT_BUFFER(object);
  switch(property_id)
  {
    case PROP_ID:
    {
      g_value_set_int64(value, _self->id);
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}
static void mb_text_buffer_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  MbTextBuffer *_self = MB_TEXT_BUFFER(object);
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
/* Signals */
/* Widget lifecycle */
static void mb_text_buffer_init(MbTextBuffer *_self)
{
  GtkTextBuffer *self = GTK_TEXT_BUFFER(_self);
  /* Instantiate widgets */
  /* Configure widgets */
  /* Connect to signals */
  g_signal_connect(self, "changed", G_CALLBACK(changed), self);
  g_signal_connect(self, "notify::id", G_CALLBACK(notify_id), self);
}
static void mb_text_buffer_class_init(MbTextBufferClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  /* Map virtual functions */
  object_class->dispose = mb_text_buffer_dispose;
  object_class->finalize = mb_text_buffer_finalize;
  object_class->get_property = mb_text_buffer_get_property;
  object_class->set_property = mb_text_buffer_set_property;
  /* Properties */
  properties[PROP_ID] = g_param_spec_int64("id", "id", "id", 0, G_MAXINT64, 0, G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, properties);
  /* Signals */
  /* Layout manager */
}
static void mb_text_buffer_dispose(GObject *object) 
{
  G_OBJECT_CLASS(mb_text_buffer_parent_class)->dispose(object);
}
static void mb_text_buffer_finalize(GObject *object) 
{
  G_OBJECT_CLASS(mb_text_buffer_parent_class)->finalize(object);
}
/* Public implementation */
GtkTextBuffer* mb_text_buffer_new(gint64 id)
{
  return g_object_new(MB_TYPE_TEXT_BUFFER, "id", id, NULL);
}
/* Private implementation */
