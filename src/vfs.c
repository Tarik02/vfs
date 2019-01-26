#include "vfs/vfs.h"
#include "vfs/source.h"

#include <stdlib.h>
#include <string.h>

typedef struct vfs {
  size_t sources_count;
  vfs_source_t *sources[VFS_SOURCES_MAX];
} vfs_t;

vfs_t *vfs_create(void) {
  vfs_t *fs = calloc(1, sizeof(vfs_t));
  
  fs->sources_count = 0;
  for (size_t i = 0; i < VFS_SOURCES_MAX; ++i) {
    fs->sources[i] = NULL;
  }

  return fs;
}

void vfs_destroy(vfs_t *fs) {
  for (size_t i = 0; i < VFS_SOURCES_MAX; ++i) {
    if (fs->sources[i] == NULL) continue;

    vfs_source_destroy(fs->sources[i]);
  }

  free(fs);
}

void vfs_preload(vfs_t *fs, const char *path) {
  // TODO:
}

vfs_file_t *vfs_open(vfs_t *fs, const char *path) {
  for (size_t i = VFS_SOURCES_MAX - 1;; --i) {
    vfs_source_t *source = fs->sources[i];
    if (source == NULL) continue;

    vfs_file_t *file;

    switch (vfs_source_open(source, path, &file)) {
    case VFS_SOURCE_OPEN_RESULT_NOTEXISTS:
      break;

    case VFS_SOURCE_OPEN_RESULT_EXISTS:
      return file;

    case VFS_SOURCE_OPEN_RESULT_REMOVE:
      return NULL;
    }

    if (i == 0) {
      break;
    }
  }

  return NULL;
}

void vfs_add_source(vfs_t *fs, vfs_source_t *source) {
  fs->sources[fs->sources_count] = source;
  ++fs->sources_count;
}
