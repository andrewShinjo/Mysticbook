#include <stdio.h>
#include "database.h"

sqlite3 *db;

int open_database(const char *path)
{
	if(db != NULL)
	{
		return -1;	
	}
	return sqlite3_open(db_path, &db);
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
