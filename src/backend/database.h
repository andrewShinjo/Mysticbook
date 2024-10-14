#pragma once
#include <sqlite3.h>

int db_open(const char *db_path);
int db_close();
sqlite3 *database_get();
