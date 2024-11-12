#include "./block_controller.h"
#include "../service/block_service.h"

gint64 block_controller_append_sibling(gint64 id)
{
  return block_service_append_sibling(id);
}

gint64 block_controller_create_document(gchar *content)
{
  return block_service_create_document(content);
}

void block_controller_delete_block(gint64 id)
{
  return block_service_delete_block(id);
}

const unsigned char* block_controller_get_block_content(gint64 id)
{
  return block_service_get_block_content(id);
}

GArray* block_controller_get_children_ids(gint64 id)
{
  return block_service_get_children_ids(id);
}

GArray* block_controller_get_document_ids()
{
  return block_service_get_document_ids();
}

gboolean block_controller_get_expanded(gint64 id)
{
  return block_service_get_expanded(id);  
}

GArray* block_controller_get_10_best_matching_blocks(gchar *text)
{
  return block_service_get_10_best_matching_blocks(text);
}

void block_controller_indent_block(gint64 id)
{
  return block_service_indent_block(id);  
}

gint64 block_controller_prepend_child(gint64 id)
{
  return block_service_prepend_child(id);
}

void block_controller_unindent_block(gint64 id)
{
  block_service_unindent_block(id);
}

void block_controller_update_content(gint64 id, const unsigned char* content)
{
  block_service_update_content(id, content);
}

void block_controller_update_expanded(gint64 id, gboolean expanded)
{
  block_service_set_expanded(id, expanded);
}

