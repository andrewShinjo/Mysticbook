#include <stdio.h>
#include "./database.h"
#include "./block.h"

/* FORWARD DECLARATION */
static void 
block_increment_positions(gint64 parent_id, gint64 start);

static int
find_all_callback(
	void *data, 
	int  column_count,
	char **column_text,
	char **column_name
)
{
	GArray *blocks = (GArray*) data;
  Block b;
	for(gint i = 0; i < column_count; i++)
	{
    const char *cn = column_name[i];
    if(g_strcmp0(cn, "id") == 0)
    {
		  b.id = g_ascii_strtoull(column_text[i], NULL, 10);
    }
    else if(g_strcmp0(cn, "content") == 0)
    {
		  b.content = g_strdup(column_text[i]);
    }
	}
	g_array_append_val(blocks, b);
	return 0;
}

static int
find_all_ids_callback(
  void *data,
  int column_count,
  char **column_text,
  char **column_name
)
{
  GArray *ids = (GArray*) data;
  for(gint i=0; i < column_count; i++)
  {
    int compare = g_strcmp0(column_name[i], "id");
    if(compare != 0)
    {
      continue;
    }
    gint64 id = g_ascii_strtoull(column_text[i], NULL, 10);
    g_array_append_val(ids, id);
  }
  return 0;
}

/* PUBLIC IMPLEMENTATION */
int 
block_delete_by_id(sqlite3_int64 id)
{
  sqlite3 *db = db_get();
	sqlite3_stmt *stmt;
	const char *sql = "DELETE FROM blocks where id = ?";
	
	if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
	{
		fprintf(
			stderr, 
			"Failed to prepare statement: %s\n", 
			sqlite3_errmsg(db)
		);
		return 1;
	}
	if(sqlite3_bind_int(stmt, 1, id) != SQLITE_OK)
	{
		fprintf(stderr, "Failed to bind ID: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 1;
	}
	if(sqlite3_step(stmt) != SQLITE_DONE)
	{
		fprintf(
      stderr, 
      "Failed to execute statement: %s\n", 
      sqlite3_errmsg(db)
    );
		sqlite3_finalize (stmt);
		return 1;
	}
	sqlite3_finalize (stmt);
	return 0;
}
void
block_find_by_id(sqlite3_int64 id, Block *b)
{
  sqlite3 *db = db_get();
  sqlite3_stmt *stmt;
  const char* sql = "SELECT * FROM blocks WHERE id = ?;";
  sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  sqlite3_bind_int64(stmt, 1, id);
  int rc;
  if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
  {
    int column_count = sqlite3_column_count(stmt);
    for(int i=0; i < column_count; i++)
    {
      const char *cn = sqlite3_column_name(stmt, i);
      const unsigned char *ct = sqlite3_column_text(stmt, i);
      if(g_strcmp0(cn, "id") == 0) 
      {
		    b->id = g_ascii_strtoull(ct, NULL, 10);
      }
      else if(g_strcmp0(cn, "content") == 0) 
      {
		    b->content = g_strdup(ct);
      }
    }
  }
}
GArray*
block_get_all_children_ids(gint64 parent_id)
{
  GArray *ids = g_array_new(FALSE, FALSE, sizeof(gint64));
  sqlite3 *db = db_get();
  sqlite3_stmt *stmt;
  const char *sql = 
    "SELECT id FROM blocks WHERE parent_id = ? ORDER BY position;";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if(rc != SQLITE_OK)
  {
    fprintf(
      stderr, 
      "Failed to prepare statement: %s\n", 
      sqlite3_errmsg(db)
    );
    return ids;
  }
  sqlite3_bind_int64(stmt, 1, parent_id);
  while((rc = sqlite3_step(stmt)) == SQLITE_ROW)
  {
    int column_count = sqlite3_column_count(stmt);
    for(int i = 0; i < column_count; i++)
    {
      const char *column_value = sqlite3_column_text(stmt, i);
      gint64 id = g_ascii_strtoll(column_value, NULL, 10);
      g_array_append_val(ids, id);
    }
  }
  return ids;
}

void block_increment_all_position()
{
  sqlite3 *db = db_get();
  sqlite3_stmt *stmt;
  const char *sql = "UPDATE blocks SET position = position + 1;";
  char *error_message = 0;
  if(sqlite3_exec(db, sql, NULL, 0, &error_message) != SQLITE_OK)
  {
    fprintf(stderr, "Failed to execute: %s\n", error_message);
    sqlite3_free(error_message);
  }
}
static void 
block_increment_positions(gint64 parent_id, gint64 position)
{
  sqlite3 *db = db_get();
  sqlite3_stmt *stmt;
  const char *sql = "UPDATE blocks SET position = position + 1 " 
    "WHERE parent_id = ? AND position >= ?";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if(rc != SQLITE_OK)
  {
    fprintf(
      stderr, 
      "Failed to prepare statement: %s\n", 
      sqlite3_errmsg(db)
    );
    return;
  }
  sqlite3_bind_int64(stmt, 1, parent_id);
  sqlite3_bind_int64(stmt, 2, position);
  if(sqlite3_step(stmt) != SQLITE_DONE)
  {
    fprintf(
      stderr, 
      "Failed to execute statement: %s\n", 
      sqlite3_errmsg(db)
    );
  }
}
sqlite3_int64
block_new_all_fields(
  gint64 *creation_time,
  gint64 *is_document,
  gint64 *modification_time,
  gint64 *position,
  gint64 *parent_id,
  gchar  *content
)
{
  sqlite3 *db = db_get();
  sqlite3_stmt *stmt;
  const char *sql = 
    "INSERT INTO blocks("
      "creation_time,"
      "is_document,"
      "modification_time,"
      "position,"
      "parent_id,"
      "content"
    ")"
    "VALUES(?,?,?,?,?,?)";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if(rc != SQLITE_OK)
  {
    fprintf(
      stderr,
      "Failed to prepare statement: %s\n", 
      sqlite3_errmsg(db)
    );
    return -1;
  }
  if(creation_time != NULL) 
  {
    sqlite3_bind_int64(stmt, *creation_time, 1);
  }
  else
  {
    sqlite3_bind_null(stmt, 1);
  }
  if(is_document != NULL) 
  {
    sqlite3_bind_int64(stmt, 2, *is_document);
  }
  else
  {
    sqlite3_bind_null(stmt, 2);
  }
  if(modification_time != NULL) 
  {
    sqlite3_bind_int64(stmt, 3, *modification_time);
  }
  else
  {
    sqlite3_bind_null(stmt, 3);
  }
  if(position != NULL) 
  {
    sqlite3_bind_int64(stmt, 4, *position);
  }
  else
  {
    sqlite3_bind_null(stmt, 4);
  }
  if(parent_id != NULL) 
  {
    sqlite3_bind_int64(stmt, 5, *parent_id);
  }
  else
  {
    sqlite3_bind_null(stmt, 5);
  }
  if(content != NULL) 
  {
    sqlite3_bind_text(stmt, 6, content, -1, SQLITE_STATIC);
  }
  else
  {
    sqlite3_bind_text(stmt, 6, "", 0, SQLITE_STATIC);
  }
  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    fprintf(
      stderr,
      "Failed to execute statement: %s\n", sqlite3_errmsg(db)
    );
    return -1;
  }
  sqlite3_finalize(stmt);
	return sqlite3_last_insert_rowid(db);
}
sqlite3_int64
block_new_sibling(gint64 sibling_id)
{
  sqlite3 *db = db_get(); 
  sqlite3_stmt *stmt;
  const char *sql =
    "SELECT parent_id, position FROM blocks WHERE id = ?;";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if(rc != SQLITE_OK)
  {
    fprintf(
      stderr,
      "Failed to prepare statement: %s\n", 
      sqlite3_errmsg(db)
    );
    return -1;
  }
  sqlite3_bind_int64(stmt, 1, sibling_id);
  if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
  {
    gint64 parent_id = sqlite3_column_int64(stmt, 0);
    gint64 start = sqlite3_column_int64(stmt, 1);
    block_increment_positions(parent_id, start);
    gint64 creation_time = 0;
    gint64 is_document = 0;
    gint64 modification_time = 0;
    gchar *content = "";
    gint64 new_id = block_new_all_fields(
      &creation_time,
      &is_document,
      &modification_time,
      &start,
      &parent_id,
      content
    );
    return new_id;
  }
  return -1;
}
sqlite3_int64
block_new()
{
  sqlite3 *db = db_get();
	const char *sql = "INSERT INTO blocks(content) VALUES('Untitled');";
	int rc = sqlite3_exec(db, sql, 0, 0, 0);
	if(rc != 0)
	{
		return -1;
	}
	return sqlite3_last_insert_rowid(db);
}
GArray*
block_get_all()
{
  sqlite3 *db = db_get();
	const char *sql = "SELECT * FROM blocks;";
	GArray *blocks = g_array_new(FALSE, FALSE, sizeof(Block));
	int return_code = sqlite3_exec(
		db, 
		sql, 
		find_all_callback, 
		blocks,
		NULL
	);
	return blocks;
}
GArray*
block_get_all_ids()
{
  sqlite3 *db = db_get();
  const char *sql = "SELECT id FROM blocks;";
  GArray *ids = g_array_new(FALSE, FALSE, sizeof(sqlite3_int64));
  int rc = sqlite3_exec(
    db,
    sql,
    find_all_ids_callback,
    ids,
    NULL
  );
}
int
block_get_children_count(gint64 id)
{
  sqlite3 *db = db_get();
  sqlite3_stmt *stmt;
  const char *sql = "SELECT COUNT(*) FROM blocks WHERE parent_id = ?";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if(rc != SQLITE_OK)
  {
    fprintf(
      stderr,
      "Failed to prepare statement: %s\n", sqlite3_errmsg(db)
    );
  }
  sqlite3_bind_int64(stmt, id, 1);
  rc = sqlite3_step(stmt);
  if(rc == SQLITE_ROW)
  {
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return count;
  }
  sqlite3_finalize(stmt);
  return -1;
}
const
gchar* block_get_content(gint64 id)
{
  sqlite3 *db = db_get();
  sqlite3_stmt *stmt;
  const char *sql = "SELECT content FROM blocks WHERE id = ?;";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if(rc != SQLITE_OK)
  {
    fprintf(
      stderr,
      "Failed to prepare statement: %s\n",
      sqlite3_errmsg(db)
    );
    return NULL;
  }
  sqlite3_bind_int64(stmt, 1, id);
  if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
  {
    return sqlite3_column_text(stmt, 0);
  }
  return NULL;
}
int
block_update_content(sqlite3_int64 id, gchar *content)
{
  sqlite3 *db = db_get();
  sqlite3_stmt *stmt;
  const char *sql = "UPDATE blocks SET content = ? WHERE id = ?;";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

  if(rc != SQLITE_OK)
  {
    fprintf(
      stderr, 
      "Failed to prepare statement: %s\n", sqlite3_errmsg(db)
    );
    return rc;
  }
  sqlite3_bind_text(stmt, 1, content, -1, SQLITE_STATIC);
  sqlite3_bind_int64(stmt, 2, id);
  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    fprintf(stderr, "Failed to update row: %s\n", sqlite3_errmsg(db));
  }
  sqlite3_finalize(stmt);
  return 0;
}
/** CREATE **/
gint64
create_document()
{
  sqlite3 *db = db_get();
  const char *query = 
    "INSERT INTO blocks(is_document, content) "
    "VALUES(1, 'Untitled');";
  if(sqlite3_exec(db, query, 0, 0, 0) != SQLITE_OK)
  {
    fprintf(
      stderr,
      "create_document, insert failed: %s\n",
      sqlite3_errmsg(db)
    );
    return -1;
  }
  return sqlite3_last_insert_rowid(db);
}
/** READ **/
void
read_all_document_id_and_content(GArray *documents)
{
  sqlite3 *db = db_get();
  sqlite3_stmt *stmt;
  const char *query = 
    "SELECT id, content FROM blocks WHERE is_document = 1;";
  int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL); 
  if(rc != SQLITE_OK)
  {
    fprintf(
      stderr,
      "Failed to prepare statement: %s\n",
      sqlite3_errmsg(db)
    );
    return;
  }
  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    gint64 id = sqlite3_column_int64(stmt, 0);
    const gchar *content = sqlite3_column_text(stmt, 1);
    Block document;
    document.id = id;
    document.content = content;
    g_array_append_val(documents, document);
  }
}
/** UPDATE **/
/** DELETE **/
