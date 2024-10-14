#pragma once
#include <sqlite3.h>

extern sqlite3 *db;

int db_open(const char *db_path);
int db_close();
