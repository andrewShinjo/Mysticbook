#include "mystic_book_app_window.h"
#include "mystic_book_doc_list.h"
#include "mystic_book_editor.h"

struct _MysticBookAppWindow
{
	GtkApplicationWindow parent;
	GtkWidget *paned;
	GtkWidget *left_pane;
	GtkWidget *right_pane;
};

/* Public start */

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

void
mystic_book_app_window_show(MysticBookAppWindow *w)
{
	gtk_window_present(GTK_WINDOW(w));
}

/* Public end */

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
mystic_book_app_window_init(MysticBookAppWindow *self) 
{
	GtkWindow *window = GTK_WINDOW(self);
	gtk_window_set_default_size(window, 900, 600);
	gtk_window_set_title(window, "Mysticbook");

	self->paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	self->left_pane = mystic_book_doc_list_new();
	self->right_pane = mystic_book_editor_new();

	gtk_paned_set_start_child(GTK_PANED(self->paned), self->left_pane);
	gtk_paned_set_end_child(GTK_PANED(self->paned), self->right_pane);
	//gtk_paned_set_position(GTK_PANED(self->paned), 200);

	gtk_window_set_child(window, self->paned);
	
}

static void
mystic_book_app_window_class_init(MysticBookAppWindowClass *klass) {}
