#include "vfs/file.h"
#include <stdlib.h>

struct vfs_file {
  void *data;

  vfs_file_read_fn read_fn;
  vfs_file_close_fn close_fn;
};

vfs_file_t *vfs_file_create(void *data, vfs_file_read_fn read_fn, vfs_file_close_fn close_fn) {
  vfs_file_t *file = calloc(1, sizeof(vfs_file_t));

  file->data = data;
  file->read_fn = read_fn;
  file->close_fn = close_fn;

  return file;
}

size_t vfs_file_read(vfs_file_t *file, unsigned char *buffer, size_t size) {
  return file->read_fn(file->data, buffer, size);
}

void vfs_file_close(vfs_file_t *file) {
  if (file == NULL) return;

  file->close_fn(file->data);
}
