#include <littlefs.h>



littlefs_t *littlefs;
struct lfs_config *cfg = 0;
lfs_t lfs;
lfs_file_t lfs_file;
lfs_dir_t lfs_dir;
struct lfs_config config;
struct lfs_info inf;


lfs_file_t* file_open(const char *path, int flags)
{
    int ret;
    ret = lfs_file_open(&lfs, &lfs_file, path, flags);
    if(ret != LFS_ERR_OK) return 0;
    return &lfs_file;
}


int file_write(void *pbuf, int size, lfs_file_t *file)
{
    return lfs_file_write(&lfs, file, pbuf, size);
}

int file_read(void *pbuf, int size, lfs_file_t *file)
{
    return lfs_file_read(&lfs, file, pbuf, size);
}

int file_close(lfs_file_t *file)
{
    return lfs_file_close(&lfs, file);
}

int file_seek(lfs_file_t *file, int offset, int whence)
{
    return lfs_file_seek(&lfs, file, offset, whence);
}

int file_tell(lfs_file_t *file)
{
    return lfs_file_tell(&lfs, file);
}
int file_size(lfs_file_t *file)
{
    return lfs_file_size(&lfs, file);
}

int file_available(lfs_file_t *file)
{
    if (!file) return 0;
    lfs_soff_t pos = lfs_file_tell(&lfs, file);
    if (pos < 0) return 0;
    lfs_soff_t size = lfs_file_size(&lfs, file);
    if (size < 0) return 0;
    return size - pos;
}

int file_rewind(lfs_file_t *file)
{
    return lfs_file_rewind(&lfs, file);
}

int file_truncate(int size, lfs_file_t *file)
{
    return lfs_file_truncate(&lfs, file, size);
}

int remove(const char *path)
{
    return lfs_remove(&lfs, path);
}

int rename(const char *oldpath, const char *newpath)
{
    return lfs_rename(&lfs, oldpath, newpath);
}

int dir_mkdir(const char *path)
{
    return lfs_mkdir(&lfs, path);
}

int exist(const char *path)
{
    return lfs_stat(&lfs, path,&inf)==0?1:0;
}


lfs_dir_t *dir_open(const char *path)
{
    int ret;
    ret = lfs_dir_open(&lfs, &lfs_dir, path);
    if(ret != LFS_ERR_OK) return 0;
    return &lfs_dir;
}

int dir_close(lfs_dir_t *dir)
{
    return lfs_dir_close(&lfs, dir);
}

int dir_read(lfs_dir_t *dir, struct lfs_info *info)
{
    return lfs_dir_read(&lfs, dir, info);
}

int dir_rewind(lfs_dir_t *dir)
{
    return lfs_dir_rewind(&lfs, dir);
}

int dir_tell(lfs_dir_t *dir)
{
    return lfs_dir_tell(&lfs, dir);
}

int dir_seek(lfs_dir_t *dir, int offset)
{
    return lfs_dir_seek(&lfs, dir, offset);
}

int format(lfs_t *_lfs, const struct lfs_config *_cfg)
{
    return lfs_format(_lfs, _cfg);
}

int mount(lfs_t *_lfs, const struct lfs_config *_cfg)
{
    return lfs_mount(_lfs, _cfg);
}

int unmount(lfs_t *_lfs)
{
    return lfs_unmount(_lfs);
}


static int _traverse_df_cb(void *p, lfs_block_t block)
{
    int *nb = p;
    *nb += 1;
    return 0;
}

int disk_free(void)
{
    int err;
    int available;
    int _df_nballocatedblock = 0;
    err = lfs_fs_traverse(&lfs, _traverse_df_cb, &_df_nballocatedblock);
    if(err < 0) return err;
    available = cfg->block_count*cfg->block_size - _df_nballocatedblock*cfg->block_size;
    return available;
}


/*
    .open = file_open,
    .write = file_write,
    .read = file_read,
    .close = file_close,
    .seek = file_seek,
    .tell = file_tell,
    .rewind = file_rewind,
    .truncate = file_truncate

    .mkdir = dir_mkdir,
    .open = dir_open,
    .close = dir_close,
    .read = dir_read,
    .rewind = dir_rewind,
    .tell = dir_tell,
    .seek = dir_seek

*/


 
#define LFS_CACHE_SIZE      64
static unsigned char read_buf[LFS_CACHE_SIZE];
static unsigned char prog_buf[LFS_CACHE_SIZE];
static unsigned char lookahead_buf[4];

int littlefs_init(littlefs_t *lfs_obj, struct lfs_config *config,int size){
    littlefs = lfs_obj;
    cfg = config;
    littlefs->lfs = &lfs;
    
    littlefs->open = file_open;
    littlefs->write = file_write;
    littlefs->read = file_read;
    littlefs->close = file_close;
    littlefs->seek = file_seek;
    littlefs->tell = file_tell;
    littlefs->rewind = file_rewind;
    littlefs->truncate = file_truncate;
    littlefs->size = file_size;
    littlefs->available = file_available;

    littlefs->mkdir = dir_mkdir;
    littlefs->openDir = dir_open;
    littlefs->closeDir = dir_close;
    littlefs->readDir = dir_read;
    littlefs->rewindDir = dir_rewind;
    littlefs->tellDir = dir_tell;
    littlefs->seekDir = dir_seek;

    littlefs->format = format;
    littlefs->mount = mount;
    littlefs->unmount = unmount;
    littlefs->remove = remove;
    littlefs->rename = rename;
    littlefs->free = disk_free;

    littlefs->exist = exist;
    //char *ptr = malloc(size);
    //if (!ptr) return 1;
    char *ptr =(char *)&fs;
    //memset(ptr, 0xFF, size); // always start with blank slate
    //size = size & 0xFFFFFF00;
    cfg->context = ptr;
    cfg->read = &static_read;
    cfg->prog = &static_prog;
    cfg->erase = &static_erase;
    cfg->sync = &static_sync;

    cfg->read_size = LFS_CACHE_SIZE;
    cfg->prog_size = LFS_CACHE_SIZE;
    cfg->block_size = LFS_CACHE_SIZE;
    cfg->block_count = size / LFS_CACHE_SIZE;
    cfg->block_cycles = 100;
    cfg->cache_size = LFS_CACHE_SIZE;
    cfg->lookahead_size = 4;
    cfg->name_max = LFS_NAME_MAX;
    cfg->read_buffer = (void*)read_buf;
    cfg->prog_buffer = (void*)prog_buf;
  
    //cfg->file_max = 0;
    //cfg->attr_max = 0;
    return 0;
}

void upload_fs(uint32_t x)
{
  asm volatile(".option arch, +zicsr"); 
  asm volatile("csrw 0x401, %0"
               :
               : "r"(x));
}
 

void download_fs(uint32_t x)
{
  asm volatile(".option arch, +zicsr"); 
  asm volatile("csrw 0x400, %0"
               :
               : "r"(x));
}