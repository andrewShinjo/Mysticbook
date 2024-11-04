#pragma once
#include <glib.h>

// block_controller is the interface the frontend 
// interacts with in order to work with the database.
// Directs requests from the frontend to the 
// appropriate service layer function.

GArray* block_controller_get_document_ids();
int block_controller_indent_block(gint64 id);
