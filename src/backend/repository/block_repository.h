#pragma once
#include <glib.h>

/**
 * @brief Creates a new block in the SQLite database.
 *
 * This function creates a new block, and returns the newly created block's ID.
 *
 * @param creation_time: The timestamp the block was created
 * @param is_document: 1 if the block is a document; otherwise, 0
 * @param modification_time: The timestamp the block was last modified
 * @param position: The position of the block in the parent's children list
 * @param parent_id: the block's parent ID
 * @param expanded: 1 if the block's children list are shown in the GUI; otherwise, 0
 * @param content: The text content of the block
 *
 * @return gint64: The ID of the newly created block
 */
gint64 block_repository_save(
  gint64 creation_time, 
  gint64 is_document, 
  gint64 modification_time, 
  gdouble position, 
  gint64 parent_id, 
  gint64 expanded,
  gchar *content
);

/* READ */
gint64 block_repository_count_by_parent_id(gint64 parent_id);
const unsigned char* block_repository_find_content(gint64 id);
gboolean block_repository_find_expanded(gint64 id);
gint64 block_repository_find_id_by_parent_id_and_position(gint64 parent_id, gdouble position);
gint64 block_repository_find_is_document(gint64 id);
gdouble block_repository_find_last_child_position(gint64 id);

/**
 * @brief Get the parent_id field that's associated with the input block ID from the SQLite database.
 * 
 * This function takes an existing block ID as input, and returns the parent_id field associated with it.
 *
 * @param id: The unique identifier of an existing block.
 *
 * @return gint64: Return the parent_id assocaited with the input block ID. If it doesn't exist, then return -1.
 *
 */
gint64 block_repository_find_parent_id_by_id(gint64 id);


/**
 * @brief Get the position field of the block with the input ID's sibling from the SQLite database.
 *
 * This function takes an existing block ID as input, and returns the position field of the block with the input ID's next 
 * sibling.
 *
 * @param id: The unique identifier of an existing block.
 * 
 * @return gdouble: Return the position value of the block with the input ID's sibling. If a sibling doesn't exist, then 
 * return -1.
 */
gdouble block_repository_find_next_closest_sibling_position(gint64 id);

/** 
 * @brief Get the position field of a block with the specified ID from the SQLite database.
 *
 * This function takes an existing block ID as input, and returns the position field of the block with the input ID.
 *
 * @param id: The unique identifier of an existing block.
 *
 * @return gdouble: Return the position value of the block with the input ID. 
 *
 */
gdouble block_repository_find_position(gint64 id);

GArray* block_repository_find_ids_by_is_document(gint64 is_document);
GArray* block_repository_find_ids_by_parent_id_order_by_position(gint64 id);
GArray* block_repository_find_ids_by_position_range_and_parent_id(gdouble start, gdouble end, gint64 parent_id);
GArray* block_repository_find_10_best_matching_blocks(const gchar *text);
gboolean block_repository_is_document(gint64 id);
/* UPDATE */
void block_repository_update_expanded(gint64 id, int expanded);
void block_repository_update_parent_id(gint64 id, gint64 parent_id);
void block_repository_update_position(gint64 id, gdouble position);
void block_repository_update_content(gint64 id, const unsigned char *content);
/* DELETE */
void block_repository_delete(gint64 id);
