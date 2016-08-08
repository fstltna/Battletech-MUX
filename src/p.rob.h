
/*
   p.rob.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:59 CET 1999 from rob.c */

#ifndef _P_ROB_H
#define _P_ROB_H

/* rob.c */
void do_kill(dbref player, dbref cause, int key, char *what,
    char *costchar);
void do_give(dbref player, dbref cause, int key, char *who, char *amnt);

#endif				/* _P_ROB_H */
