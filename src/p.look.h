
/*
   p.look.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:51 CET 1999 from look.c */

#ifndef _P_LOOK_H
#define _P_LOOK_H

/* look.c */
void look_in(dbref player, dbref loc, int key);
void do_look(dbref player, dbref cause, int key, char *name);
void do_examine(dbref player, dbref cause, int key, char *name);
void do_score(dbref player, dbref cause, int key);
void do_inventory(dbref player, dbref cause, int key);
void do_entrances(dbref player, dbref cause, int key, char *name);
void do_sweep(dbref player, dbref cause, int key, char *where);
void do_decomp(dbref player, dbref cause, int key, char *name, char *qual);
void show_vrml_url(dbref thing, dbref loc);

#endif				/* _P_LOOK_H */
