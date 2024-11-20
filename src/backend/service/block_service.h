#pragma once
#include <glib.h>

/** 
 * @brief Creates a sibling block positioned following a specified block ID.
 * 
 * This function takes an existing block ID as input and creates a new sibling block immediately following the block with the 
 * input ID. The new block has the same parent_id as the input block and has a position adjacent after the input block.
 * 
 * @param id: The unique identifier of the existing block after which the new sibling block will be appended.
 *
 * @return gint64: Return the id of the newly created sibling block.
 */
gint64 block_service_append_sibling(gint64 id);
gint64 block_service_create_block(gchar *content);
gint64 block_service_create_document(gchar *content);
void block_service_delete_block(gint64 id);
const unsigned char* block_service_get_block_content(gint64 id);
GArray* block_service_get_children_ids(gint64 id);
GArray* block_service_get_document_ids();
gboolean block_service_get_expanded(gint64 id);
GArray* block_service_get_10_best_matching_blocks(gchar *text);
void block_service_increment_position(gint64 id);
void block_service_indent_block(gint64 id);
gint64 block_service_prepend_child(gint64 id);
void block_service_set_expanded(gint64 id, gboolean expanded);
void block_service_unindent_block(gint64 id);
void block_service_update_content(gint64 id, const unsigned char *content);
