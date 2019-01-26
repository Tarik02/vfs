#include "vfs/source.h"
#include <stdlib.h>

struct vfs_source {
  void *data;

  vfs_source_open_fn open_fn;
  vfs_source_free_fn free_fn;
};

vfs_source_t *vfs_source_create(void *data, vfs_source_open_fn open_fn, vfs_source_free_fn free_fn) {
  vfs_source_t *source = calloc(1, sizeof(vfs_source_t));

  source->data = data;
  source->open_fn = open_fn;
  source->free_fn = free_fn;

  return source;
}

enum vfs_source_open_result vfs_source_open(vfs_source_t *source, const char *path, vfs_file_t **file) {
  return source->open_fn(source->data, path, file);
}

void vfs_source_destroy(vfs_source_t *source) {
  source->free_fn(source->data);
  free(source);
}

void *vfs_source_data(vfs_source_t *source) {
  return source->data;
}
