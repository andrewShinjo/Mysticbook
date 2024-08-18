#include "mysticbook_application_window.h"
#include "../pages/mysticbook_documents_page.h"

struct _MysticbookApplicationWindow
{
  GtkApplicationWindow parent;
  GtkWidget *documents_page;
};

G_DEFINE_TYPE(
	MysticbookApplicationWindow,
	mysticbook_application_window,
	GTK_TYPE_APPLICATION_WINDOW
)

static void
mysticbook_application_window_init(MysticbookApplicationWindow *self)
{
  GtkWindow *window = GTK_WINDOW(self);
  self->documents_page = mysticbook_documents_page_new();
  gtk_window_set_child(window, self->documents_page);
}

static void
mysticbook_application_window_class_init(
	MysticbookApplicationWindowClass *klass
) {}

GtkWidget *
mysticbook_application_window_new(GtkApplication *application)
{
	return GTK_WIDGET(
		g_object_new(
			MYSTICBOOK_TYPE_APPLICATION_WINDOW, 
			"application", application, 
			NULL
		)
	);
}
