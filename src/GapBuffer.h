#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

/**
 * GapBuffer is a dynamic array with a "gap" near the cursor, allowing
 * fast insertions and deletions by operating within the gap. When the
 * cursor moves, the gap is shifted by copying text, so that edits near
 * the cursor are efficient.
 */
typedef struct {
  char *buffer;
  int capacity;
  int gap_start;
  int gap_end;
} GapBuffer;

/**
 * Initializes a gap buffer with a buffer size of initial capacity.
 * Since the gap buffer is initialized as empty, gap_start is at the
 * beginning of the buffer and gap_end is at the end of the buffer.
 */
GapBuffer* gap_buffer_create(int initial_capacity);

#endif
