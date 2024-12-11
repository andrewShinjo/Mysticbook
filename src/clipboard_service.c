#include "./clipboard_service.h"

/* Private definition */

static void image_data_received(GdkClipboard *self, GAsyncResult *result, gpointer user_data);

/* Public implementation */

gboolean clipboard_service_has_image()
{
	GdkClipboard *self = gdk_display_get_clipboard(gdk_display_get_default());
	GdkContentFormats *formats = gdk_clipboard_get_formats(self);
	gboolean has_image = gdk_content_formats_contain_mime_type(formats, "image/png");

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

static void image_data_received(GdkClipboard *clipboard, GAsyncResult *result, gpointer user_data)
{
	GdkTexture *texture = gdk_clipboard_read_texture_finish(clipboard, result, NULL);
	if(texture && GDK_IS_TEXTURE(texture))
	{
		const char *filename = (char*) user_data;
		gdk_texture_save_to_png(texture, filename);
	}
}
