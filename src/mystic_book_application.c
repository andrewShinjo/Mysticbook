#include "mystic_book_application.h"
#include "mystic_book_app_window.h"

struct _MysticBookApplication
{
	GtkApplication parent;
	GtkWidget *app_window;
	GSettings *settings;
};

G_DEFINE_FINAL_TYPE(
	MysticBookApplication,
	mystic_book_application,
	GTK_TYPE_APPLICATION
)

/* Signal callbacks start */

/* 1. When an application runs for the first time,
 *    the instance is called primary.
 * 2. Primary instance registers itself to the system.
 *    If it succeeds, then it emits "startup" signal.
 * 3. When the instance is activated, an "activate" or
 *    "open" signal is emitted.
 * 4. Additional instances are called remote instances.
 * 5. Remote instances don't emit "startup" signal.
 * 6. If the remote instance tries to emit an "activate"
 *    or "open" signal, then the signal is emitted to the
 *    primary instance.
 */

static void
activate(GtkApplication *gtk_app, gpointer user_data)
{
	MysticBookApplication *self = MYSTIC_BOOK_APPLICATION(gtk_app);
	self->app_window = mystic_book_app_window_new(gtk_app);
	gtk_window_present(GTK_WINDOW(self->app_window));
}

/* Signal callbacks end */

static void
mystic_book_application_init(MysticBookApplication *app) /* Called everytime an instance of the class is created. */
{
	GtkApplication* gtk_app = GTK_APPLICATION(app);

	g_signal_connect(
		gtk_app,
		"activate",
		G_CALLBACK(activate),
		NULL
	);
}

// Called only the first time the class is being used.
static void
mystic_book_application_class_init(MysticBookApplicationClass *klass)
{}

MysticBookApplication *
mystic_book_application_new(
	const char *application_id,
	GApplicationFlags application_flags
)
{
	return MYSTIC_BOOK_APPLICATION(
		g_object_new(
			MYSTIC_BOOK_TYPE_APPLICATION,
			"application-id", application_id,
			"flags", application_flags,
			NULL
		)	
	);
}
