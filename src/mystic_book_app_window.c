#include "mystic_book_app_window.h"
#include "mystic_book_editor.h"

struct _MysticBookAppWindow
{
	GtkApplicationWindow parent;
	GtkWidget *paned;
	GtkWidget *left;
	GtkWidget *right;
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

	self->paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);

	self->left = gtk_list_box_new();
	self->right = mystic_book_editor_new();

	gtk_list_box_set_selection_mode(
		GTK_LIST_BOX(self->left), 
		GTK_SELECTION_NONE
	);

	GtkWidget *row1 = gtk_list_box_row_new();
    GtkWidget *label1 = gtk_label_new("Row 1");
    gtk_widget_set_hexpand(label1, TRUE);
	gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row1), label1);
	gtk_list_box_insert(GTK_LIST_BOX(self->left), row1, -1);

	GtkWidget *row2 = gtk_list_box_row_new();
    GtkWidget *label2 = gtk_label_new("Row 2");
    gtk_widget_set_hexpand(label2, TRUE);
	gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row2), label2);
	gtk_list_box_insert(GTK_LIST_BOX(self->left), row2, -1);


	gtk_paned_set_start_child(GTK_PANED(self->paned), self->left);
	gtk_paned_set_end_child(GTK_PANED(self->paned), self->right);

	gtk_window_set_child(window, self->paned);
	gtk_window_set_default_size(window, 900, 600);
	gtk_window_set_title(window, "Mysticbook");
}

static void
mystic_book_app_window_class_init(MysticBookAppWindowClass *klass) {}
