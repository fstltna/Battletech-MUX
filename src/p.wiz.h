
/*
   p.wiz.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:15 CET 1999 from wiz.c */

#ifndef _P_WIZ_H
#define _P_WIZ_H

/* wiz.c */
int recursive_check_contents(dbref victim, dbref destination);
void do_teleport(dbref player, dbref cause, int key, char *arg1,
    char *arg2);
void do_force_prefixed(dbref player, dbref cause, int key, char *command,
    char *args[], int nargs);
void do_force(dbref player, dbref cause, int key, char *what,
    char *command, char *args[], int nargs);
void do_toad(dbref player, dbref cause, int key, char *toad,
    char *newowner);
void do_newpassword(dbref player, dbref cause, int key, char *name,
    char *password);
void do_boot(dbref player, dbref cause, int key, char *name);
void do_poor(dbref player, dbref cause, int key, char *arg1);
void do_cut(dbref player, dbref cause, int key, char *thing);
void do_quota(dbref player, dbref cause, int key, char *arg1, char *arg2);
void do_motd(dbref player, dbref cause, int key, char *message);
void do_global(dbref player, dbref cause, int key, char *flag);

#endif				/* _P_WIZ_H */
