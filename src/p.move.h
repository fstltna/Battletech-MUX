
/*
   p.move.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:54 CET 1999 from move.c */

#ifndef _P_MOVE_H
#define _P_MOVE_H

/* move.c */
void move_object(dbref thing, dbref dest);
void move_via_generic(dbref thing, dbref dest, dbref cause, int hush);
void move_via_exit(dbref thing, dbref dest, dbref cause, dbref exit,
    int hush);
int move_via_teleport(dbref thing, dbref dest, dbref cause, int hush);
void move_exit(dbref player, dbref exit, int divest, const char *failmsg,
    int hush);
void do_move(dbref player, dbref cause, int key, char *direction);
void do_get(dbref player, dbref cause, int key, char *what);
void do_drop(dbref player, dbref cause, int key, char *name);
void do_enter_internal(dbref player, dbref thing, int quiet);
void do_enter(dbref player, dbref cause, int key, char *what);
void do_leave(dbref player, dbref cause, int key);

#endif				/* _P_MOVE_H */
