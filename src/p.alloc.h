
/*
   p.alloc.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:37 CET 1999 from alloc.c */

#ifndef _P_ALLOC_H
#define _P_ALLOC_H

/* alloc.c */
void pool_init(int poolnum, int poolsize);
void pool_check(const char *tag);
char *pool_alloc(int poolnum, const char *tag);
void pool_free(int poolnum, char **buf);
void list_bufstats(dbref player);
void list_buftrace(dbref player);
void pool_reset(void);

#endif				/* _P_ALLOC_H */
