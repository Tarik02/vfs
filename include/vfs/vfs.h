#ifndef VFS_VFS_H
#define VFS_VFS_H

#ifndef VFS_SOURCES_MAX
#define VFS_SOURCES_MAX 32
#endif

typedef struct vfs vfs_t;
typedef struct vfs_source vfs_source_t;
typedef struct vfs_file vfs_file_t;

vfs_t *vfs_create(void);
void vfs_destroy(vfs_t *fs);

void vfs_preload(vfs_t *fs, const char *path);
vfs_file_t *vfs_open(vfs_t *fs, const char *path);

void vfs_add_source(vfs_t *fs, vfs_source_t *source);

#endif // VFS_VFS_H
