#pragma once
#include <sqlite3.h>

int open_database(const char *path);
int close_database(const char *path);

void create_blocks_table();
void clear_blocks_table();

sqlite3_stmt* prepare_statement(const char *sql);

void begin_transaction();
void commit_transaction();
void rollback_transaction();

int rows_changed();
