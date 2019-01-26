#include "vfs/sources/prefixed.h"
#include "vfs/source.h"
#include "vfs/file.h"
#include <stdlib.h>
#include <string.h>

typedef struct vfs_source_prefixed {
  vfs_source_t *backend;

  char *prefix;
  char *root;
} vfs_source_prefixed_t;

enum vfs_source_open_result vfs_source_prefixed_open(void *data, const char *path, vfs_file_t **file) {
  vfs_source_prefixed_t *source = (vfs_source_prefixed_t *) data;

  size_t lenpre = strlen(source->prefix);
  size_t lenstr = strlen(path);
  
  if (lenstr > lenpre && strncmp(source->prefix, path, lenpre) == 0) {
    size_t lenroot = strlen(source->root);
    size_t len = lenstr + lenroot - lenpre;
    char *new_path = malloc(len + 1);
    strcpy(new_path, source->root);
    strcpy(new_path + lenroot, path + lenpre);
    new_path[len] = '\0';

    enum vfs_source_open_result res = vfs_source_open(source->backend, new_path, file);
    free(new_path);
    return res;
  }

  return VFS_SOURCE_OPEN_RESULT_NOTEXISTS;
}

void vfs_source_prefixed_free(void *data) {
  vfs_source_prefixed_t *source = (vfs_source_prefixed_t *) data;

  vfs_source_destroy(source->backend);
  free(source->prefix);
  free(source->root);
}

vfs_source_t *vfs_source_prefixed_create(vfs_source_t *backend, const char *prefix, const char *root) {
  vfs_source_prefixed_t *source = calloc(1, sizeof(vfs_source_prefixed_t));

  source->backend = backend;
  source->prefix = strdup(prefix);
  source->root = strdup(root);

  return vfs_source_create(source, vfs_source_prefixed_open, vfs_source_prefixed_free);
}
