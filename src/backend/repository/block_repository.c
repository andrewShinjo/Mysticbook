#include "./block_repository.h"
#include "../database/database.h"

/* CREATE */

gint64 block_repository_save(
  gint64 creation_time,
  gint64 is_document,
  gint64 modification_time,
  gdouble position,
  gint64 parent_id,
  gint64 expanded,
  gchar *content  
)
{
  /**
   * Algorithm:
   * 1. Bind all of the function parameters into the query.
   * 2. Execute the query.
   * 3. Return the newly created block's ID.
   */
  const char *sql = 
    "INSERT INTO blocks (creation_time, is_document, modification_time, position, parent_id, expanded, content) "
    "VALUES(?, ?, ?, ?, ?, ?, ?);";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_save: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, creation_time);
  sqlite3_bind_int64(stmt, 2, is_document);
  sqlite3_bind_int64(stmt, 3, modification_time);
  sqlite3_bind_double(stmt, 4, position);
  sqlite3_bind_int64(stmt, 5, parent_id);
  sqlite3_bind_int64(stmt, 6, expanded);
  sqlite3_bind_text(stmt, 7, content, -1, SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return last_inserted_id();
}

/* READ */

gint64 block_repository_count_by_parent_id(gint64 parent_id)
{
  const char *sql = "SELECT COUNT(id) FROM blocks WHERE parent_id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_count_by_parent_id: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, parent_id);
  sqlite3_step(stmt);
  gint64 count = sqlite3_column_int64(stmt, 0);
  return count; 
}

const unsigned char* block_repository_find_content(gint64 id)
{
  const char *sql = "SELECT content FROM blocks WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_content: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, id);
  sqlite3_step(stmt);
  const unsigned char *content = g_strdup(sqlite3_column_text(stmt, 0));
  sqlite3_finalize(stmt);
  return content;
}

gboolean block_repository_find_expanded(gint64 id)
{
  const char *sql = "SELECT expanded FROM blocks where id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_expanded: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, id);
  sqlite3_step(stmt);
  gint64 expanded = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);
  return expanded == 1;
}

gint64 block_repository_find_id_by_parent_id_and_position(gint64 parent_id, gdouble position)
{
  const char *sql = "SELECT id FROM blocks WHERE parent_id = ? AND position = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_id_by_parent_id_and_position: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, parent_id);
  sqlite3_bind_double(stmt, 2, position);
  sqlite3_step(stmt);
  gint64 id = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);
  return id;
}

GArray* block_repository_find_10_best_matching_blocks(gchar *text)
{
  const char *sql = 
    "SELECT *, bm25(blocks_fts5) AS RANK "
    "FROM blocks_fts5 "
    "WHERE blocks_fts5 MATCH ? "
    "ORDER BY RANK LIMIT 10;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_10_best_matching_blocks: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_text(stmt, 1, text, -1, SQLITE_STATIC);
  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    gint64 id = sqlite3_column_int64(stmt, 0);
    const gchar* content = sqlite3_column_text(stmt, 1);
  }
  return NULL;
}

gint64 block_repository_find_is_document(gint64 id)
{
  const char *sql = "SELECT is_document FROM blocks WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_is_document: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, id);
  sqlite3_step(stmt);
  gint64 is_document = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);
  return is_document;
}

gdouble block_repository_find_last_child_position(gint64 id)
{
  g_print("block_repository_find_last_child_position: %ld\n", id);
  const char *sql = "SELECT COUNT(*) FROM blocks WHERE parent_id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_last_child_position: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, id);
  sqlite3_step(stmt);
  gdouble position = sqlite3_column_double(stmt, 0);
  sqlite3_finalize(stmt);
  return position;
}

gint64 block_repository_find_parent_id_by_id(gint64 id)
{
  const char *sql = "SELECT parent_id FROM blocks WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_parent_id_by_id: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, id);
  sqlite3_step(stmt);
  gint64 parent_id = sqlite3_column_int64(stmt, 0);
  sqlite3_finalize(stmt);
  return parent_id;
}

/**
 * Algorithm:
 *
 * 1. Bind the input ID.
 * 2. Execute the SQL query.
 * 3. Return the next cloest position, if exists. Otherwise, return -1.
 */
gdouble block_repository_find_next_closest_sibling_position(gint64 id)
{
  const char *sql = "SELECT position FROM blocks "
    "WHERE parent_id = (SELECT parent_id FROM blocks WHERE id = ?) "
    "AND position > (SELECT position FROM blocks WHERE id = ?) "
    "ORDER BY position LIMIT 1;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_next_closest_sibling_position: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, id);
  sqlite3_bind_int64(stmt, 2, id);
  gboolean result_found = sqlite3_step(stmt) == SQLITE_ROW;
  if(result_found)
  {
    gdouble position = sqlite3_column_double(stmt, 0);
    return position;
  }
  else
  {
    return -1;
  }
}

/**
 * Algorithm:
 *
 * 1. Bind the input ID to the SQL query.
 * 2. Execute the SQL query.
 * 3. Return block's position.
 */
gdouble block_repository_find_position(gint64 id)
{
  const char *sql = "SELECT position FROM blocks WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_position: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, id);
  sqlite3_step(stmt);
  gdouble position = sqlite3_column_double(stmt, 0);
  sqlite3_finalize(stmt);
  return position;
}

GArray* block_repository_find_ids_by_is_document(gint64 is_document)
{
  const char *sql = "SELECT id FROM blocks WHERE is_document = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_finds_ids_by_is_document: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, is_document);
  GArray *ids = g_array_new(FALSE, FALSE, sizeof(gint64));
  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    gint64 id = sqlite3_column_int64(stmt, 0);
    g_array_append_val(ids, id);
  }
  return ids;
}

GArray* block_repository_find_ids_by_parent_id_order_by_position(gint64 parent_id)
{
  const char *sql = "SELECT id FROM blocks WHERE parent_id = ? ORDER BY position;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_ids_by_parent_id_order_by_position: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, parent_id);
  GArray *ids = g_array_new(FALSE, FALSE, sizeof(gint64));
  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    gint64 id = sqlite3_column_int64(stmt, 0);
    g_array_append_val(ids, id);
  }
  return ids;
}

GArray* block_repository_find_ids_by_position_range_and_parent_id(gdouble start, gdouble end, gint64 parent_id)
{
  GArray *ids = g_array_new(FALSE, FALSE, sizeof(gint64));
  if(start > end)
  {
    return ids;
  }
  const char *sql = "SELECT id FROM blocks WHERE position >= ? AND position <= ? AND parent_id = ?;"; 
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_ids_by_position_range_and_parent_id: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_double(stmt, 1, start);
  sqlite3_bind_double(stmt, 2, end);
  sqlite3_bind_int64(stmt, 3, parent_id);
  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    gint64 id = sqlite3_column_int64(stmt, 0);
    g_array_append_val(ids, id);
  }
  return ids;
}

gboolean block_repository_is_document(gint64 id)
{
  return block_repository_find_is_document(id) == 1;
}

/* UPDATE */

void block_repository_update_expanded(gint64 id, int expanded)
{
  const char *sql = "UPDATE blocks SET expanded = ? WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_update_expanded: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, expanded);
  sqlite3_bind_int64(stmt, 2, id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void block_repository_update_parent_id(gint64 id, gint64 parent_id)
{
  const char *sql = "UPDATE blocks SET parent_id = ? WHERE id = ?;"; 
  sqlite3_stmt *stmt = prepare_statement(sql);

  if(stmt == NULL)
  {
    return;
  }

  sqlite3_bind_int64(stmt, 1, parent_id);
  sqlite3_bind_int64(stmt, 2, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void block_repository_update_position(gint64 id, gdouble position)
{
  const char *sql = "UPDATE blocks SET position = ? WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    return;
  }
  sqlite3_bind_double(stmt, 1, position);
  sqlite3_bind_int64(stmt, 2, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void block_repository_update_content(gint64 id, const unsigned char *content)
{
  const char *sql = "UPDATE blocks SET content = ? WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_update_content: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_text(stmt, 1, content, -1, SQLITE_STATIC);
  sqlite3_bind_int64(stmt, 2, id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

/* DELETE */
void block_repository_delete(gint64 id)
{
  const char *sql = "DELETE FROM blocks WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_delete: Failed to prepare statement.\n");
    exit(EXIT_FAILURE);
  }
  sqlite3_bind_int64(stmt, 1, id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}
