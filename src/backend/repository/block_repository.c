#include "./block_repository.h"
#include "../database/database.h"

/* CREATE */

void block_repository_save(
  gint64 creation_time,
  gint64 is_document,
  gint64 modification_time,
  gint64 position,
  gint64 parent_id,
  gchar *content  
)
{
  const char *sql = "INSERT INTO blocks (creation_time, is_document, modification_time, position, parent_id, content) "
    "VALUES(?, ?, ?, ?, ?, ?);";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_save: Failed to prepare statement.\n");
    return;
  }
  sqlite3_bind_int64(stmt, 1, creation_time);
  sqlite3_bind_int64(stmt, 2, is_document);
  sqlite3_bind_int64(stmt, 3, modification_time);
  sqlite3_bind_int64(stmt, 4, position);
  sqlite3_bind_int64(stmt, 5, parent_id);
  sqlite3_bind_text(stmt, 6, content, -1, SQLITE_STATIC);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if(rc != SQLITE_DONE)
  {
    g_print("block_repository_save: Failed to execute.\n");
  }
}

/* READ */

gint64 block_repository_find_last_child_position(gint64 id)
{
  const char *sql = 
  "SELECT IFNULL(MAX(position), 0) FROM blocks WHERE parent_id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_last_child_position: "
      "failed to prepare statement.\n");
    return -1;
  }
  sqlite3_bind_int64(stmt, 1, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if(rc != SQLITE_DONE)
  {
    g_print("block_repository_find_last_child_position: "
      "failed to execute statement.\n");
    return -1;
  }
  gint64 position = sqlite3_column_int64(stmt, 0);
  return position;
}

gint64 block_repository_find_parent_id_by_id(gint64 id)
{
  const char *sql = "SELECT parent_id FROM blocks WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);

  if(stmt == NULL)
  {
    g_print("block_repository_find_parent_id_by_id: "
      "failed to prepare statement.\n");
    return -1;
  }

  sqlite3_bind_int64(stmt, 1, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if(rc != SQLITE_DONE)
  {
    g_print("block_repository_find_parent_id_by_id: "
      "failed to execute statement.\n");
    return -1;
  }

  gint64 parent_id = sqlite3_column_int64(stmt, 0);
  return parent_id;
}

gint64 block_repository_find_position(gint64 id)
{
  const char *sql = "SELECT position FROM blocks WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  
  if(stmt == NULL)
  {
    g_print("block_repository_find_position: Failed to prepare statement.\n");
    return -1;
  }
  sqlite3_bind_int64(stmt, 1, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  
  if(rc != SQLITE_DONE)
  {
    g_print(
      "block_repository_find_position: Failed to execute statement.\n"
    );
    return -1;
  }

  gint64 position = sqlite3_column_int64(stmt, 0);
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

GArray* block_repository_find_ids_by_position_range_and_parent_id(gint64 start, gint64 end, gint64 parent_id)
{
  const char *sql = "SELECT id FROM blocks WHERE position >= ? AND position <= ? AND parent_id = ?;"; 
  sqlite3_stmt *stmt = prepare_statement(sql);
  if(stmt == NULL)
  {
    g_print("block_repository_find_ids_by_position_range_and_parent_id: Failed to prepare statement.\n");
    return NULL;
  }
  sqlite3_bind_int64(stmt, 1, start);
  sqlite3_bind_int64(stmt, 2, end);
  sqlite3_bind_int64(stmt, 3, parent_id);
  GArray *ids = g_array_new(FALSE, FALSE, sizeof(gint64));
  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    gint64 id = sqlite3_column_int64(stmt, 0);
    g_array_append_val(ids, id);
  }
  return ids;
}

/* UPDATE */

int block_repository_update_parent_id(gint64 id, gint64 parent_id)
{
  const char *sql = "UPDATE blocks SET parent_id = ? WHERE id = ?;"; 
  sqlite3_stmt *stmt = prepare_statement(sql);

  if(stmt == NULL)
  {
    return -1;
  }

  sqlite3_bind_int64(stmt, 1, parent_id);
  sqlite3_bind_int64(stmt, 2, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if(rc != SQLITE_DONE)
  {
    return -1;
  }
  if(rows_changed() == 1)
  {
    return 0;
  }
  return -1;
}

int block_repository_update_position_id(gint64 id, gint64 position)
{
  const char *sql = "UPDATE blocks SET position = ? WHERE id = ?;";
  sqlite3_stmt *stmt = prepare_statement(sql);

  if(stmt == NULL)
  {
    return -1;
  }

  sqlite3_bind_int64(stmt, 1, position);
  sqlite3_bind_int64(stmt, 2, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if(rc != SQLITE_DONE)
  {
    return -1;
  }
  if(rows_changed() == 1)
  {
    return 0;
  }
  return -1;
}

/* DELETE */
