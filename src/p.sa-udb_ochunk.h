
/*
   p.sa-udb_ochunk.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:06 CET 1999 from sa-udb_ochunk.c */

#ifndef _P_SA_UDB_OCHUNK_H
#define _P_SA_UDB_OCHUNK_H

/* sa-udb_ochunk.c */
__inline__ int __stat(__const char *__path, struct stat *__statbuf);
__inline__ int stat(__const char *__path, struct stat *__statbuf);
__inline__ int __lstat(__const char *__path, struct stat *__statbuf);
__inline__ int lstat(__const char *__path, struct stat *__statbuf);
__inline__ int __fstat(int __fd, struct stat *__statbuf);
__inline__ int fstat(int __fd, struct stat *__statbuf);
__inline__ int __mknod(__const char *__path, __mode_t __mode,
    __dev_t __dev);
__inline__ int mknod(__const char *__path, __mode_t __mode, __dev_t __dev);
int dddb_optimize(void);
int dddb_init(void);
int dddb_setfile(char *fil);
int dddb_close(void);
Obj *dddb_get(Objname * nam);
int dddb_put(Obj * obj, Objname * nam);
int dddb_del(Objname * nam);

#endif				/* _P_SA_UDB_OCHUNK_H */
