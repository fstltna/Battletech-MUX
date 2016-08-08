
/*
   p.create.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:43 CET 1999 from create.c */

#ifndef _P_CREATE_H
#define _P_CREATE_H

/* create.c */
void do_open(dbref player, dbref cause, int key, char *direction,
    char *links[], int nlinks);
void do_link(dbref player, dbref cause, int key, char *what, char *where);
void do_parent(dbref player, dbref cause, int key, char *tname,
    char *pname);
void do_dig(dbref player, dbref cause, int key, char *name, char *args[],
    int nargs);
void do_create(dbref player, dbref cause, int key, char *name,
    char *coststr);
void do_clone(dbref player, dbref cause, int key, char *name, char *arg2);
void do_pcreate(dbref player, dbref cause, int key, char *name,
    char *pass);
void do_destroy(dbref player, dbref cause, int key, char *what);

#endif				/* _P_CREATE_H */
