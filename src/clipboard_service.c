#include "./clipboard_service.h"

/* Private definition */

static void image_data_received(GdkClipboard *self, GdkTexture *texture, gpointer user_data);

/* Public implementation */

gboolean clipboard_service_has_image()
{
	GdkClipboard *self = gdk_display_get_clipboard(gdk_display_get_default());
	GdkContentFormats *formats = gdk_clipboard_get_formats(self);
	gboolean has_image = gdk_content_formats_contain_mime_type(formats, "image/png") 
		|| gdk_content_formats_contain_mime_type(formats, "image/jpeg") 
		|| gdk_content_formats_contain_mime_type(formats, "image/bmp") 
		|| gdk_content_formats_contain_mime_type(formats, "image/x-icon");

	g_object_unref(formats);
	return has_image;
}

const gchar* clipboard_service_save_image()
{
	GdkClipboard *self = gdk_display_get_clipboard(gdk_display_get_default());
	gchar *filename = "./resources/images/clipboard_image.jpg";
	gdk_clipboard_read_texture_async(self, NULL, (GAsyncReadyCallback) image_data_received, filename);
	return filename;
}

/* Private implementation */

static void image_data_received(GdkClipboard *self, GdkTexture *texture, gpointer user_data)
{
	if(texture)
	{
		const char *filename = (char*) user_data;
		gdk_texture_save_to_png(texture, filename);
	}
}
