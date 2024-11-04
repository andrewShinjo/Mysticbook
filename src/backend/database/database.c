#include <glib.h>
#include "database.h"

sqlite3 *db = NULL;

int open_database(const char *path)
{
	if(db != NULL)
	{
		return -1;	
	}
	sqlite3_open(path, &db);
  create_blocks_table();
  return 0;
}

int close_database(const char *path)
{
	if(db != NULL)
	{
    sqlite3_close(db);
    db = NULL;
    return 0;
	}
  return -1;
}

void create_blocks_table()
{
  const char *sql = "CREATE TABLE IF NOT EXISTS blocks("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "creation_time INTEGER, "
    "is_document INTEGER, "
    "modification_time INTEGER, "
    "position INTEGER, "
    "parent_id INTEGER, "
    "content TEXT "
    ");"
    ;
  sqlite3_stmt *stmt = prepare_statement(sql);
  sqlite3_exec(db, sql, 0, 0, NULL);
}

void clear_blocks_table()
{
  const char *sql = "DELETE FROM blocks;";
  sqlite3_stmt *stmt = prepare_statement(sql);
  sqlite3_exec(db, sql, 0, 0, NULL);
}

sqlite3_stmt* prepare_statement(const char *sql)
{
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if(rc != SQLITE_OK)
  {
    g_print("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return NULL;
  }
  return stmt;
}

void begin_transaction()
{
  sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);
}

void commit_transaction()
{
  sqlite3_exec(db, "COMMIT TRANSACTION;", 0, 0, 0);
}

void rollback_transaction()
{
  sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
}

int rows_changed()
{
  return sqlite3_changes(db);
}

gint64 last_inserted_id()
{
  return sqlite3_last_insert_rowid(db);
}
