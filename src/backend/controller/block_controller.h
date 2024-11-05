#pragma once
#include <glib.h>
gint64 block_controller_append_sibling(gint64 id);
gint64 block_controller_create_document(gchar *content);
void block_controller_delete_block(gint64 id);
const unsigned char* block_controller_get_block_content(gint64 id);
GArray* block_controller_get_children_ids(gint64 id);
GArray* block_controller_get_document_ids();
int block_controller_indent_block(gint64 id);
gint64 block_controller_prepend_child(gint64 id);
void block_controller_update_content(gint64 id, const unsigned char* content);
