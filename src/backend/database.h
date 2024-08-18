#pragma once
#include <sqlite3.h>

int open_database (const char *database_path);
int close_database ();
