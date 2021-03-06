
/*
 * $Id: event.h 1.1 02/01/03 01:03:28-00:00 twouters@ $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1996 Markus Stenberg
 *       All rights reserved
 *
 * Created: Tue Aug 27 19:02:00 1996 fingon
 * Last modified: Sat Jun  6 22:20:36 1998 fingon
 *
 */

#ifndef EVENT_H
#define EVENT_H

/* EVENT_DEBUG adds some useful debugging information to the structure
   / allows more diverse set of error messages to be shown. However,
   for a run-time version it's practically useless. */

/* #undef EVENT_DEBUG */

#define LOOKAHEAD_STACK_SIZE 70

/* Larger the stack, better ; this one's right now used to
   prevent general silliness [ if event's time is >= L_A_S_S, things
   turn _nasty_ */
#define FLAG_FREE_DATA      1	/* Free the 1st data segment after execution */
#define FLAG_FREE_DATA2     2	/* Free the 2nd data segment after execution */
#define FLAG_ZOMBIE         4	/* Exists there just because we're too
				   lazy to search for it everywhere - dud */

/* ZOMBIE events aren't moved during reschedule, they instead die then.
   Killing them outside event_run is kinda unhealthy, therefore we set things
   just ZOMBIE and delete if it's convenient for us. */
#define FLAG_IMPORTANT      16	/* Too important to be moved during
				   load balancing */

/* Main idea: Events are arranged as follows:
   - next 1-60sec (depending on present timing) each their own
     linked list
   - next hour with each min in the own linked list
   - next 60 hours with each hour in the own linked list
   - the rest in one huge 'stack', ordered according to time
     */

/* typedef unsigned char byte; */

typedef struct my_event_type {
    char flags;
    void (*function) (struct my_event_type *);
    void *data;
    void *data2;
    int tick;			/* The tick this baby was first scheduled to go off */
    char type;
#ifdef EVENT_DEBUG
    int tick_scheduled;
    int count_0_scheduled_at;
    int count_1_scheduled_at;
    int count_0_scheduled_to;
    int count_1_scheduled_to;
#endif
    struct my_event_type *next;
    struct my_event_type *next_in_main;
    struct my_event_type *prev_in_main;
    struct my_event_type *prev_in_type;
    struct my_event_type *next_in_type;
} EVENT;

/* Some external things _do_ use this one */
extern int event_tick;
extern int events_scheduled;
extern int events_executed;
extern int events_zombies;

#include "p.event.h"

/* Simplified event adding is more or less irrelevant, most programs
   tend to make their own macros for it. This is an example,
   though. */
#define event_add_simple_arg(time,func,data) event_add(time,0,0,func,data, NULL)
#define event_add_simple_noarg(time,func) event_add(time,0,0,func,NULL, NULL)

/* Macros for handling simple lists 
   Where it applies: a = main list, b = thing to be added, c = prev
   field, d = next field (c = next field in case of single-linked
   model */

#define REMOVE_FROM_LIST(a,c,b) if (a == b ) a = b->c; else { EVENT *t; \
for (t=a;t->c != b;t=t->c); t->c = b->c; }
#define REMOVE_FROM_BIDIR_LIST(a,c,d,b) if (b->c) b->c->d = b->d; \
 if (b->d) b->d->c = b->c; if (a==b) { a=b->d; if (a) a->c=NULL; }

#define ADD_TO_LIST_HEAD(a,c,b) b->c = a ; a = b
#define ADD_TO_BIDIR_LIST_HEAD(a,c,d,b) b->d=a ; if (a) a->c = b ; a=b ; \
                                        b->c=NULL


/* The standard debugging macro.. Should be more or less useless
 because this is mostly used in daemon type of programs.  */

#ifdef DEBUG
#define debug(a...) printf(##a)
#else
#define debug(a...)
#endif

void event_add(int time, int flags, int type, void (*func) (EVENT *),
    void *data, void *data2);
void event_gothru_type_data(int type, void *data, void (*func) (EVENT *));
void event_gothru_type(int type, void (*func) (EVENT *));



/* Did I mention cproto is braindead? */

#endif				/* EVENT_H */
