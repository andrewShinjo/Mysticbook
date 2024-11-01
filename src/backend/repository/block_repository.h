#pragma once
#include <glib.h>

// block_repository is the data access layer.
// Only this file should contain functions
// that directly interact with tables in the database.
// Functions like create, read, update, and delete are here.

/* CREATE */

/* READ */
gint64 block_repository_find_last_child_position(gint64 id);
gint64 block_repository_find_parent_id_by_id(gint64 id);
gint64 block_repository_find_position(gint64 id);

GArray* block_repository_find_ids_by_position_range_and_parent_id(
  gint64 start,
  gint64 end,
  gint64 parent_id
);

/* UPDATE */

int block_repository_update_parent_id(gint64 id, gint64 parent_id);
int block_repository_update_position(gint64 id, gint64 position);

/* DELETE */
