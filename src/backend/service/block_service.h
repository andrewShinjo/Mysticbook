#pragma once
#include <glib.h>

// block_service is responsible for implementing the
// services that the frontend requests.
// It processes data, enforce rules, and perform transactions.

gint64 block_service_append_sibling(gint64 id);
gint64 block_service_create_block(gchar *content);
gint64 block_service_create_document(gchar *content);
void block_service_delete_block(gint64 id);
const unsigned char* block_service_get_block_content(gint64 id);
GArray* block_service_get_children_ids(gint64 id);
GArray* block_service_get_document_ids();
gboolean block_service_get_expanded(gint64 id);
void block_service_increment_position(gint64 id);
void block_service_indent_block(gint64 id);
gint64 block_service_prepend_child(gint64 id);
void block_service_set_expanded(gint64 id, gboolean expanded);
void block_service_unindent_block(gint64 id);
void block_service_update_content(gint64 id, const unsigned char *content);
