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

void block_service_delete_block(gint64 id)
{
  gint64 parent_id = block_repository_find_parent_id_by_id(id);
  gint64 position = block_repository_find_position(id);
  gint64 children_count = block_repository_count_by_parent_id(id);
  gint64 start = position + 1;
  gint64 end = block_repository_find_last_child_position(parent_id);
  // Reposition siblings.
  GArray *sibling_ids = block_repository_find_ids_by_position_range_and_parent_id(start, end, parent_id);
  guint sibling_count = sibling_ids->len;
  gint64 position_change = children_count - 1;
  for(guint i = 0; i < sibling_count; i++)
  {
    gint64 sibling_id = g_array_index(sibling_ids, gint64, i);
    gint64 original_position = block_repository_find_position(sibling_id);
    gint64 new_position = original_position + position_change;
    block_repository_update_position(sibling_id, new_position);
  }
  g_free(sibling_ids);
  // Reposition children.
  GArray *children_ids = block_repository_find_ids_by_parent_id_order_by_position(id);
  for(guint i = 0; i < children_ids->len; i++)
  {
    gint64 child_id = g_array_index(children_ids, gint64, i);
    block_repository_update_parent_id(child_id, parent_id);
    block_repository_update_position(child_id, position + i);
  }
  g_free(children_ids);
  // Delete self.
  block_repository_delete(id); 
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

void block_service_indent_block(gint64 id)
{
  // Get parent_id
  gint64 parent_id = block_repository_find_parent_id_by_id(id);
  if(parent_id == 0)
  {
    // No parent_id. Can't indent.
    return;
  }
  // Reposition siblings.
  gint64 my_pos = block_repository_find_position(id);
  gint64 start = my_pos + 1;
  gint64 end = block_repository_find_last_child_position(parent_id);
  GArray *sibling_ids = block_repository_find_ids_by_position_range_and_parent_id(start, end, parent_id);
  guint sibling_count = sibling_ids->len;
  for(guint i = 0; i < sibling_count; i++)
  {
    gint64 sibling_id = g_array_index(sibling_ids, gint64, i);
    gint64 original_position = block_repository_find_position(sibling_id);
    gint64 new_position = original_position - 1;
    block_repository_update_position(sibling_id, new_position);
  }
  g_free(sibling_ids);
  // Update self's parent_id and position.
  gint64 prev_sibling_pos = my_pos - 1;
  g_print("previous_sibling_position=%ld\n", prev_sibling_pos);
  gint64 prev_sibling_id = block_repository_find_id_by_parent_id_and_position(parent_id, prev_sibling_pos);
  g_print("previous_sibling_id=%ld\n", prev_sibling_id);
  block_repository_update_parent_id(id, prev_sibling_id); // move this afterwards
  gint64 new_pos = block_repository_find_last_child_position(prev_sibling_id) + 1;
  block_repository_update_position(id, new_pos);
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
