
/*
   p.object.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:55 CET 1999 from object.c */

#ifndef _P_OBJECT_H
#define _P_OBJECT_H

/* object.c */
dbref start_home(void);
dbref default_home(void);
int can_set_home(dbref player, dbref thing, dbref home);
dbref new_home(dbref player);
dbref clone_home(dbref player, dbref thing);
dbref create_obj(dbref player, int objtype, char *name, int cost);
void destroy_obj(dbref player, dbref obj);
void divest_object(dbref thing);
void empty_obj(dbref obj);
void destroy_exit(dbref exit);
void destroy_thing(dbref thing);
void destroy_player(dbref victim);
void do_dbck(dbref player, dbref cause, int key);

#endif				/* _P_OBJECT_H */
