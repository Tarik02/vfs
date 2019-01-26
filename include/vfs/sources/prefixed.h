#ifndef VFS_SOURCE_PREFIXED_H
#define VFS_SOURCE_PREFIXED_H

typedef struct vfs_source vfs_source_t;

vfs_source_t *vfs_source_prefixed_create(vfs_source_t *backend, const char *prefix, const char *root);

#endif // VFS_SOURCE_PREFIXED_H
