#include "mystic_book_app_window.h"

struct _MysticBookAppWindow
{
	GtkApplicationWindow parent;
};


GtkWidget *
mystic_book_app_window_new(GtkApplication* gtk_app)
{
	return GTK_WIDGET(
		g_object_new(
			MYSTIC_BOOK_TYPE_APP_WINDOW,
			"application",
			gtk_app,
			NULL
		)
	);
}

G_DEFINE_TYPE(
	MysticBookAppWindow, 
	mystic_book_app_window, 
	GTK_TYPE_WINDOW
)

/* Private */

static void 
mystic_book_app_window_dispose(GObject *object) {}

static void 
mystic_book_app_window_finalize(GObject *object) {}

static void
mystic_book_app_window_init(MysticBookAppWindow *self) {}

static void
mystic_book_app_window_class_init(MysticBookAppWindowClass *klass) {}
