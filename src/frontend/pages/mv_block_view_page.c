#include "./mb_block_view_page.h"
#include "../components/mb_text_view.h"

static void mb_block_view_page_dispose(GObject *object);
static void mb_block_view_page_finalize(GObject *object);

struct _MbBlockViewPage
{
	GtkWidget parent;
  GtkWidget *vbox;
  GtkWidget *main_text_view;

  GtkWidget *hbox;
  GtkWidget *expander;
  GtkWidget *child_tv;
};

G_DEFINE_TYPE(MbBlockViewPage, mb_block_view_page, GTK_TYPE_WIDGET)

// *********************************************************************
// * Widget lifecycle
// *********************************************************************

static void mb_block_view_page_dispose(GObject *object) 
{
  MbBlockViewPage *self = MB_BLOCK_VIEW_PAGE(object);
  g_clear_pointer(&self->vbox, gtk_widget_unparent);
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->dispose(object);
}

static void mb_block_view_page_finalize(GObject *object)
{
  G_OBJECT_CLASS(mb_block_view_page_parent_class)->finalize(object);
}

static void mb_block_view_page_init(MbBlockViewPage *self)
{
  self->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);  
  self->main_text_view = gtk_text_view_new();

  self->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  self->expander = gtk_expander_new(NULL);
  self->child_tv = gtk_text_view_new();

  gtk_box_append(GTK_BOX(self->hbox), self->expander);
  gtk_box_append(GTK_BOX(self->hbox), self->child_tv);
  gtk_widget_set_hexpand(self->child_tv, TRUE);

  gtk_box_append(GTK_BOX(self->vbox), self->main_text_view);
  gtk_box_append(GTK_BOX(self->vbox), self->hbox);
  gtk_widget_set_hexpand(self->vbox, TRUE);
  gtk_widget_set_vexpand(self->vbox, TRUE);

  gtk_widget_set_parent(self->vbox, GTK_WIDGET(self));
}
static void mb_block_view_page_class_init(MbBlockViewPageClass *klass) 
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = mb_block_view_page_dispose;
  object_class->finalize = mb_block_view_page_finalize;
  gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(klass), GTK_TYPE_BOX_LAYOUT);
}

// *********************************************************************
// * Public
// *********************************************************************

GtkWidget *
mb_block_view_page_new()
{
	return g_object_new(MB_TYPE_BLOCK_VIEW_PAGE, NULL);
}
