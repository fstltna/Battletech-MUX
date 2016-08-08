
/*
   p.player_c.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:57 CET 1999 from player_c.c */

#ifndef _P_PLAYER_C_H
#define _P_PLAYER_C_H

/* player_c.c */
void pcache_init(void);
PCACHE *pcache_find(dbref player);
void pcache_reload(dbref player);
void pcache_trim(void);
void pcache_sync(void);
void pcache_purge(dbref player);
int a_Queue(dbref player, int adj);
void s_Queue(dbref player, int val);
int QueueMax(dbref player);
int Pennies(dbref obj);
void s_Pennies(dbref obj, int howfew);

#endif				/* _P_PLAYER_C_H */
