#ifndef VFS_SOURCE_MEMORY_H
#define VFS_SOURCE_MEMORY_H

typedef struct vfs_source vfs_source_t;

vfs_source_t *vfs_source_memory_create(void);

void vfs_source_memory_create_file(vfs_source_t *source, const char *path, const unsigned char *data, size_t size);
void vfs_source_memory_remove_file(vfs_source_t *source, const char *path);

#endif // VFS_SOURCE_MEMORY_H
