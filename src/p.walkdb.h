
/*
   p.walkdb.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:14 CET 1999 from walkdb.c */

#ifndef _P_WALKDB_H
#define _P_WALKDB_H

/* walkdb.c */
void do_dolist(dbref player, dbref cause, int key, char *list,
    char *command, char *cargs[], int ncargs);
void do_find(dbref player, dbref cause, int key, char *name);
int get_stats(dbref player, dbref who, STATS * info);
void do_stats(dbref player, dbref cause, int key, char *name);
int chown_all(dbref from_player, dbref to_player);
void do_chownall(dbref player, dbref cause, int key, char *from, char *to);
void er_mark_disabled(dbref player);
int search_setup(dbref player, char *searchfor, SEARCH * parm);
void search_perform(dbref player, dbref cause, SEARCH * parm);
void do_search(dbref player, dbref cause, int key, char *arg);
void do_markall(dbref player, dbref cause, int key);
void do_apply_marked(dbref player, dbref cause, int key, char *command,
    char *cargs[], int ncargs);
void olist_init(void);
void olist_add(dbref item);
dbref olist_first(void);
dbref olist_next(void);

#endif				/* _P_WALKDB_H */
