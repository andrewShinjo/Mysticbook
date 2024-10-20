#include <stdio.h>
#include "./database.h"
#include "./block.h"

static int find_all_callback(
	void *data, 
	int  column_count,
	char **column_text,
	char **column_name
)
{
  g_print("find_all_callback\n");
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

void block_find_by_id(sqlite3_int64 id, Block *b)
{
  g_print("block_find_by_id\n");
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

      //g_print("column_name=%s, column_text=%s\n", cn, ct);

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

sqlite3_int64 block_new()
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
