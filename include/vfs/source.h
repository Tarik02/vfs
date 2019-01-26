#ifndef VFS_SOURCE_H
#define VFS_SOURCE_H

typedef struct vfs_file vfs_file_t;
typedef struct vfs_source vfs_source_t;

enum vfs_source_open_result {
  VFS_SOURCE_OPEN_RESULT_NOTEXISTS,
  VFS_SOURCE_OPEN_RESULT_EXISTS,
  VFS_SOURCE_OPEN_RESULT_REMOVE,
};

typedef enum vfs_source_open_result (*vfs_source_open_fn)(void *data, const char *path, vfs_file_t **file);
typedef void (*vfs_source_free_fn)(void *data);

vfs_source_t *vfs_source_create(void *data, vfs_source_open_fn open_fn, vfs_source_free_fn free_fn);
enum vfs_source_open_result vfs_source_open(vfs_source_t *source, const char *path, vfs_file_t **file);
void vfs_source_destroy(vfs_source_t *source);

void *vfs_source_data(vfs_source_t *source);

#endif // VFS_SOURCE_H
