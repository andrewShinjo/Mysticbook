#pragma once
#include <glib.h>

// block_repository is the data access layer.
// Only this file should contain functions
// that directly interact with tables in the database.
// Functions like create, read, update, and delete are here.

/* CREATE */
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
gint64 block_repository_find_parent_id_by_id(gint64 id);
gdouble block_repository_find_position(gint64 id);
GArray* block_repository_find_ids_by_is_document(gint64 is_document);
GArray* block_repository_find_ids_by_parent_id_order_by_position(gint64 id);
GArray* block_repository_find_ids_by_position_range_and_parent_id(gdouble start, gdouble end, gint64 parent_id);
GArray* block_repository_find_10_best_matching_blocks(gchar *text);
gboolean block_repository_is_document(gint64 id);
/* UPDATE */
void block_repository_update_expanded(gint64 id, int expanded);
void block_repository_update_parent_id(gint64 id, gint64 parent_id);
void block_repository_update_position(gint64 id, gdouble position);
void block_repository_update_content(gint64 id, const unsigned char *content);
/* DELETE */
void block_repository_delete(gint64 id);
