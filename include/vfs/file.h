#ifndef VFS_FILE_H
#define VFS_FILE_H

#include <stddef.h>

typedef struct vfs_file vfs_file_t;

typedef size_t (*vfs_file_read_fn)(void *data, unsigned char *buffer, size_t size);
typedef void (*vfs_file_close_fn)(void *data);

vfs_file_t *vfs_file_create(void *data, vfs_file_read_fn read_fn, vfs_file_close_fn close_fn);
size_t vfs_file_read(vfs_file_t *file, unsigned char *buffer, size_t size);
void vfs_file_close(vfs_file_t *file);

#endif // VFS_FILE_H
