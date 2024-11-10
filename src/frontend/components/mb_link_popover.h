#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define MB_TYPE_LINK_POPOVER mb_link_popover_get_type()
G_DECLARE_FINAL_TYPE(MbLinkPopover, mb_link_popover, MB, LINK_POPOVER, GtkPopover)
GtkWidget *mb_link_popover_new();
void mb_link_popover_popup(MbLinkPopover *_self);
void mb_link_popover_popdown(MbLinkPopover *_self);
G_END_DECLS
