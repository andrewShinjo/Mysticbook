#include <stdio.h>
#include "./database.h"
#include "./block.h"

static int
find_all_callback (
	void *data, 
	int  column_count,
	char **column_text,
	char **column_name
)
{
	GArray *blocks = (GArray*) data;
	for(gint i = 0; i < column_count; i++)
	{
		int compare = g_strcmp0 (column_name[i], "id");
		if(compare != 0)
		{
			continue;
		}
		Block b;
		b.id = g_ascii_strtoull(column_text[i], NULL, 10);
		b.content = g_strdup(column_text[i+3]);
		g_array_append_val(blocks, b);
	}
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

sqlite3_int64 block_new()
{
  sqlite3 *db = db_get();
	const char *sql = "INSERT INTO blocks("
    "creation_time, is_document, modification_time, content)" 
	  " VALUES(0, 0, 'Untitled', 1);";
	int rc = sqlite3_exec(db, sql, 0, 0, 0);
	if(rc != 0)
	{
		return -1;
	}
	return sqlite3_last_insert_rowid(db);
}

GArray* block_get_all()
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

GArray* block_get_all_ids()
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

int block_delete_by_id(sqlite3_int64 id)
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

int block_update_content(sqlite3_int64 id, gchar *content)
{
  return 0;
}
