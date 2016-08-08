
/*
   p.file_c.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:46 CET 1999 from file_c.c */

#ifndef _P_FILE_C_H
#define _P_FILE_C_H

/* file_c.c */
void do_list_file(dbref player, dbref cause, int extra, char *arg);
void fcache_read_dir(char *dir, FCACHE foo[], int *cnt, int max);
void fcache_rawdump(int fd, int num);
void fcache_dumpbase(DESC * d, FCACHE fc[], int num);
void fcache_dump(DESC * d, int num);
void fcache_dump_conn(DESC * d, int num);
void fcache_send(dbref player, int num);
void fcache_load(dbref player);
void fcache_init(void);

#endif				/* _P_FILE_C_H */
