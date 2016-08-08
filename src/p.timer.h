
/*
   p.timer.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:10 CET 1999 from timer.c */

#ifndef _P_TIMER_H
#define _P_TIMER_H

/* timer.c */
void init_timer(void);
void dispatch(void);
void do_timewarp(dbref player, dbref cause, int key, char *arg);

#endif				/* _P_TIMER_H */
