###部分输出如下所示###
```
0 ******************sysfile_read  read file  
1 ******************* file_read
*****************iobuf_init           init io buffer struct
2 *****************************vop_read
3 **************** sfs_read
4  *********************sfs_rbuf
*****************iobuf_init           init io buffer struct
5 ******************read in disk 
6 ***********************ide_read_secs
*****************iobuf_move        move dat
*****************iobuf_skip        change the current position of io buffer
*****************iobuf_skip        change the current position of io buffer
******************sysfile_seek  seek file  
```
###在一个进程结束时，会关闭相应的文件，当程序退出时，会将fs和vfs都退出###
```
*************  files_destroy      Called when a proc exit 
*************  fs_cleanup
****************vfs_cleanup
```

###当然，因为上面是输出需要主要的几个步骤，并没有完全展示所有的函数，函数调用全过程如下所示###
```
   read()                                                    file.c
-> sys_read()                                                syscall.c
-> sysfile_read()                                            sysfile.c
-> file_read()                                               file.c
-> vop_read()                                                inode.c
-> sfs_read() -> sfs_io() -> sfs_io_nolock() -> sfs_rbuf()   sfs_inode.c
-> sfs_rwblock_nolock()                                      sfs_io.c
-> dop_io()                                                  dev.h 
-> disk0_io()                                                dev_disk.c
->ide_read_secs()                                            ide.c
```
