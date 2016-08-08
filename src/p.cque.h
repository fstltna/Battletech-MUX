
/*
   p.cque.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:43 CET 1999 from cque.c */

#ifndef _P_CQUE_H
#define _P_CQUE_H

/* cque.c */
int halt_que(dbref player, dbref object);
void do_halt(dbref player, dbref cause, int key, char *target);
int nfy_que(dbref sem, int attr, int key, int count);
void do_notify(dbref player, dbref cause, int key, char *what,
    char *count);
void wait_que(dbref player, dbref cause, int wait, dbref sem, int attr,
    char *command, char *args[], int nargs, char *sargs[]);
void do_wait(dbref player, dbref cause, int key, char *event, char *cmd,
    char *cargs[], int ncargs);
int que_next(void);
void do_second(void);
int do_top(int ncmds);
void do_ps(dbref player, dbref cause, int key, char *target);
void do_queue(dbref player, dbref cause, int key, char *arg);

#endif				/* _P_CQUE_H */
