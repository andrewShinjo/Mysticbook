#pragma once
#include <glib.h>
#include <sqlite3.h>

typedef struct
{
	sqlite3_int64 id;	
	sqlite3_int64 creation_time;
	sqlite3_int64 modification_time;
	const unsigned char *content;
	int is_document;
} Block;

/* PUBLIC INTERFACE */
int 
block_delete_by_id(sqlite3_int64 id);
void 
block_find_by_id(sqlite3_int64 id, Block *b);
GArray* 
block_get_all();
GArray* 
block_get_all_ids();
GArray* 
block_get_all_children_ids(gint64 parent_id);
int 
block_get_children_count(gint64 id);
void 
block_increment_all_position();
sqlite3_int64 
block_new();
sqlite3_int64 
block_new_all_fields(
  gint64 *creation_time,
  gint64 *is_document,
  gint64 *modification_time,
  gint64 *position,
  gint64 *parent_id,
  gchar *content
);
sqlite3_int64 
block_new_sibling(gint64 sibling_id);
int 
block_update_content(sqlite3_int64 id, gchar *content);

/** CREATE **/
gint64
create_document();
/** READ **/
void
read_all_document_id_and_content(GArray *documents);
void
read_all_document_ids(GArray *document_ids);
const gchar*
read_block_content(gint64 id);
/** UPDATE **/
/** DELETE **/
