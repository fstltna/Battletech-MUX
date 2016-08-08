
/*
   p.game.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:48 CET 1999 from game.c */

#ifndef _P_GAME_H
#define _P_GAME_H

/* game.c */
__inline__ int __stat(__const char *__path, struct stat *__statbuf);
__inline__ int stat(__const char *__path, struct stat *__statbuf);
__inline__ int __lstat(__const char *__path, struct stat *__statbuf);
__inline__ int lstat(__const char *__path, struct stat *__statbuf);
__inline__ int __fstat(int __fd, struct stat *__statbuf);
__inline__ int fstat(int __fd, struct stat *__statbuf);
__inline__ int __mknod(__const char *__path, __mode_t __mode,
    __dev_t __dev);
__inline__ int mknod(__const char *__path, __mode_t __mode, __dev_t __dev);
void do_dump(dbref player, dbref cause, int key);
void do_dump_optimize(dbref player, dbref cause, int key);
void report(void);
int regexp_match(char *pattern, char *str, char *args[], int nargs);
int atr_match(dbref thing, dbref player, int type, char *str,
    int check_parents);
int check_filter(dbref object, dbref player, int filter, const char *msg);
int html_escape(const char *src, char *dest, char **destp);
void notify_check(dbref target, dbref sender, const char *msg, int key);
void notify_except(dbref loc, dbref player, dbref exception,
    const char *msg);
void notify_except2(dbref loc, dbref player, dbref exc1, dbref exc2,
    const char *msg);
void do_shutdown(dbref player, dbref cause, int key, char *message);
void dump_database_internal(int dump_type);
void dump_database(void);
void fork_and_dump(int key);
int list_check(dbref thing, dbref player, int type, char *str,
    int check_parent);
int Hearer(dbref thing);
void do_readcache(dbref player, dbref cause, int key);
void do_transponder(void);
void main(int argc, char *argv[]);

#endif				/* _P_GAME_H */
