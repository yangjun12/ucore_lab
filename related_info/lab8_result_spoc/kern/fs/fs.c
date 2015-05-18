#include <defs.h>
#include <kmalloc.h>
#include <sem.h>
#include <vfs.h>
#include <dev.h>
#include <file.h>
#include <sfs.h>
#include <inode.h>
#include <assert.h>
//called when init_main proc start
void
fs_init(void) {
	cprintf("*************  fs init \n");
	cprintf("************* begin vfs  init \n");
    vfs_init();
	cprintf("************* end vfs  init \n");
    cprintf("*************   begin  dev_init \n");
    dev_init();
    cprintf("*************   end  dev_init \n");
    cprintf("*************   begin   sfs_init \n");
    sfs_init();
    cprintf("*************   end   sfs_init \n");
}

void
fs_cleanup(void) {
	cprintf("*************  fs_cleanup\n");
    vfs_cleanup();
}

void
lock_files(struct files_struct *filesp) {
	cprintf("*************  lock_files\n");
    down(&(filesp->files_sem));
}

void
unlock_files(struct files_struct *filesp) {
	cprintf("*************  unlock_files\n");
    up(&(filesp->files_sem));
}
//Called when a new proc init
struct files_struct *
files_create(void) {
	cprintf("*************  files_create      Called when a new proc init \n");
    //cprintf("[files_create]\n");
    static_assert((int)FILES_STRUCT_NENTRY > 128);
    struct files_struct *filesp;
    if ((filesp = kmalloc(sizeof(struct files_struct) + FILES_STRUCT_BUFSIZE)) != NULL) {
        filesp->pwd = NULL;
        filesp->fd_array = (void *)(filesp + 1);
        filesp->files_count = 0;
        sem_init(&(filesp->files_sem), 1);
        fd_array_init(filesp->fd_array);
    }
    return filesp;
}
//Called when a proc exit
void
files_destroy(struct files_struct *filesp) {
	cprintf("*************  files_destroy      Called when a proc exit \n");
//    cprintf("[files_destroy]\n");
    assert(filesp != NULL && files_count(filesp) == 0);
    if (filesp->pwd != NULL) {
        vop_ref_dec(filesp->pwd);
    }
    int i;
    struct file *file = filesp->fd_array;
    for (i = 0; i < FILES_STRUCT_NENTRY; i ++, file ++) {
        if (file->status == FD_OPENED) {
            fd_array_close(file);
        }
        assert(file->status == FD_NONE);
    }
    kfree(filesp);
}

void
files_closeall(struct files_struct *filesp) {
	cprintf("*************  files_closeall     closeall \n");
//    cprintf("[files_closeall]\n");
    assert(filesp != NULL && files_count(filesp) > 0);
    int i;
    struct file *file = filesp->fd_array;
    //skip the stdin & stdout
    for (i = 2, file += 2; i < FILES_STRUCT_NENTRY; i ++, file ++) {
        if (file->status == FD_OPENED) {
            fd_array_close(file);
        }
    }
}

int
dup_fs(struct files_struct *to, struct files_struct *from) {
	cprintf("*************  dup_fs     copy \n");
//    cprintf("[dup_fs]\n");
    assert(to != NULL && from != NULL);
    assert(files_count(to) == 0 && files_count(from) > 0);
    if ((to->pwd = from->pwd) != NULL) {
        vop_ref_inc(to->pwd);
    }
    int i;
    struct file *to_file = to->fd_array, *from_file = from->fd_array;
    for (i = 0; i < FILES_STRUCT_NENTRY; i ++, to_file ++, from_file ++) {
        if (from_file->status == FD_OPENED) {
            /* alloc_fd first */
            to_file->status = FD_INIT;
            fd_array_dup(to_file, from_file);
        }
    }
    return 0;
}

