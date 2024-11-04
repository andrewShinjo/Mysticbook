#include "./block_service.h"
#include "../repository/block_repository.h"

GArray* block_service_get_document_ids()
{
  GArray *document_ids = block_repository_find_ids_by_is_document(1);
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
