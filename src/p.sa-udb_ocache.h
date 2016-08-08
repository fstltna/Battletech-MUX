
/*
   p.sa-udb_ocache.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:06 CET 1999 from sa-udb_ocache.c */

#ifndef _P_SA_UDB_OCACHE_H
#define _P_SA_UDB_OCACHE_H

/* sa-udb_ocache.c */
void cache_repl(Cache * cp, Obj * new);
int cache_init(int width, int depth);
void cache_reset(int trim);
Attr *cache_get(Aname * nam);
int cache_put(Aname * nam, Attr * obj);
int cache_sync(void);
void cache_del(Aname * nam);

#endif				/* _P_SA_UDB_OCACHE_H */
