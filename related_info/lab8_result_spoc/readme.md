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


*************  files_destroy      Called when a proc exit 
*************  fs_cleanup
****************vfs_cleanup




read() -> sys_read() -> sysfile_read() -> file_read() -> vop_read()  -> sfs_read() -> sfs_io() -> sfs_io_nolock() -> sfs_rbuf() -> sfs_rwblock_nolock() -> dop_io() -> disk0_io()
