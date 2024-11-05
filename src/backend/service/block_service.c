#include "./block_service.h"
#include "../repository/block_repository.h"

gint64 block_service_append_sibling(gint64 id)
{
  g_print("block_service_append_sibling\n");
  // Reposition siblings.
  gint64 parent_id = block_repository_find_parent_id_by_id(id);
  gint64 start = block_repository_find_position(id) + 1;
  gint64 end = block_repository_find_last_child_position(parent_id);
  g_print("start=%ld, end=%ld\n", start, end);
  GArray *sibling_ids = block_repository_find_ids_by_position_range_and_parent_id(start, end, parent_id);
  guint count = sibling_ids->len;
  for(guint i = 0; i < count; i++)
  {
    gint64 sibling_id = g_array_index(sibling_ids, gint64, i);
    block_service_increment_position(sibling_id);
  }
  g_free(sibling_ids);
  // Create new siblings.
  gint64 new_id = block_repository_save(0, 0, 0, start, parent_id, "");
  return new_id;
}

gint64 block_service_create_block(gchar *content)
{
  return block_repository_save(0, 0, 0, 0, 0, content);
}

gint64 block_service_create_document(gchar *content)
{
  return block_repository_save(0, 1, 0, 0, 0, content);
}

const unsigned char* block_service_get_block_content(gint64 id)
{
  return block_repository_find_content(id);
}

GArray* block_service_get_children_ids(gint64 id)
{
  return block_repository_find_ids_by_parent_id_order_by_position(id);
}

GArray* block_service_get_document_ids()
{
  GArray *document_ids = block_repository_find_ids_by_is_document(1);
}

void block_service_increment_position(gint64 id)
{
  gint64 position = block_repository_find_position(id);
  block_repository_update_position(id, position + 1);
}

int block_service_indent_block(gint64 id)
{
  // Get parent_id
  gint64 parent_id = block_repository_find_parent_id_by_id(id);
  if(parent_id == 0)
  {
    // No parent_id. Can't indent.
    return -1;
  }
  // Get my position
  gint64 start_position = block_repository_find_position(id);
  // Get position of parent's last child
  gint64 last_position = block_repository_find_last_child_position(id);
  // Get id of childrens positioned [pos+1, last_pos]
  // Decrement all of their positions by 1
  // Get id of previous_sibling
  // Update my parent to be previous_sibling
  // Get last position of new parent
  // Set my position to be new last position of new parent
}

gint64 block_service_prepend_child(gint64 id)
{
  gint64 start = 1;
  gint64 end = block_repository_find_last_child_position(id);
  GArray *children_ids = block_repository_find_ids_by_position_range_and_parent_id(start, end, id);
  guint count = children_ids->len;
  for(guint i = 0; i < count; i++)
  {
    gint64 child_id = g_array_index(children_ids, gint64, i);
    block_service_increment_position(child_id);
  }
  gint64 child_id = block_repository_save(0, 0, 0, 1, id, "");
  g_free(children_ids);
  return child_id;
}

void block_service_update_content(gint64 id, const unsigned char *content)
{
  block_repository_update_content(id, content);
}
