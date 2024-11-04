#pragma once
#include <glib.h>

// block_service is responsible for implementing the
// services that the frontend requests.
// It processes data, enforce rules, and perform transactions.

gint64 block_service_create_document(gchar *content);
const unsigned char* block_service_get_block_content(gint64 id);
GArray* block_service_get_document_ids();
int block_service_indent_block(gint64 id);
