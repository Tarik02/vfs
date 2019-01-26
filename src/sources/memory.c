#include "vfs/sources/prefixed.h"
#include "vfs/source.h"
#include "vfs/file.h"
#include <stdlib.h>
#include <string.h>
#include <hashmap.h>

typedef struct vfs_source_memory_entry {
  enum vfs_source_open_result mode;

  size_t size;
  unsigned char data[0];
} vfs_source_memory_entry_t;

typedef struct vfs_source_memory_file {
  size_t offset;
  vfs_source_memory_entry_t *entry;
} vfs_source_memory_file_t;

HASHMAP_FUNCS_CREATE(vfs_source_memory_entry, char, struct vfs_source_memory_entry);

typedef struct vfs_source_memory {
  struct hashmap map;
} vfs_source_memory_t;

size_t vfs_source_memory_file_read(void *data, unsigned char *buffer, size_t size) {
  vfs_source_memory_file_t *file = (vfs_source_memory_file_t *) data;
  vfs_source_memory_entry_t *entry = file->entry;

  size_t rem = entry->size - file->offset;
  size_t to_read = rem < size ? rem : size;
  memcpy(buffer, entry->data + file->offset, to_read);
  file->offset += to_read;
  return to_read;
}

void vfs_source_memory_file_close(void *data) {
  vfs_source_memory_file_t *file = (vfs_source_memory_file_t *) data;

  free(file);
}

enum vfs_source_open_result vfs_source_memory_open(void *data, const char *path, vfs_file_t **file) {
  vfs_source_memory_t *source = (vfs_source_memory_t *) data;

  vfs_source_memory_entry_t *entry = vfs_source_memory_entry_hashmap_get(&source->map, path);

  if (entry == NULL) {
    return VFS_SOURCE_OPEN_RESULT_NOTEXISTS;
  }

  if (entry->mode == VFS_SOURCE_OPEN_RESULT_EXISTS) {
    vfs_source_memory_file_t *memory_file = calloc(1, sizeof(vfs_source_memory_file_t));
    memory_file->offset = 0;
    memory_file->entry = entry;
    *file = vfs_file_create(memory_file, vfs_source_memory_file_read, vfs_source_memory_file_close);
  }

  return entry->mode;
}

void vfs_source_memory_free(void *data) {
  vfs_source_memory_t *source = (vfs_source_memory_t *) data;

  for (struct hashmap_iter *iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
    free(vfs_source_memory_entry_hashmap_iter_get_data(iter));
  }

  hashmap_destroy(&source->map);
}

vfs_source_t *vfs_source_memory_create(void) {
  vfs_source_memory_t *source = calloc(1, sizeof(vfs_source_memory_t));

  hashmap_init(&source->map, hashmap_hash_string, hashmap_compare_string, 64);

  return vfs_source_create(source, vfs_source_memory_open, vfs_source_memory_free);
}

void vfs_source_memory_create_file(vfs_source_t *source, const char *path, const unsigned char *data, size_t size) {
  vfs_source_memory_t *memory_source = (vfs_source_memory_t *) vfs_source_data(source);

  vfs_source_memory_entry_t *entry = malloc(sizeof(vfs_source_memory_entry_t) + size);
  entry->size = size;
  entry->mode = VFS_SOURCE_OPEN_RESULT_EXISTS;
  memcpy(entry->data, data, size);

  free(vfs_source_memory_entry_hashmap_remove(&memory_source->map, path));
  vfs_source_memory_entry_hashmap_put(&memory_source->map, path, entry);
}

void vfs_source_memory_remove_file(vfs_source_t *source, const char *path) {
  vfs_source_memory_t *memory_source = (vfs_source_memory_t *) vfs_source_data(source);

  vfs_source_memory_entry_t *entry = malloc(sizeof(vfs_source_memory_entry_t));
  entry->size = 0;
  entry->mode = VFS_SOURCE_OPEN_RESULT_REMOVE;
  
  free(vfs_source_memory_entry_hashmap_remove(&memory_source->map, path));
  vfs_source_memory_entry_hashmap_put(&memory_source->map, path, entry);
}
