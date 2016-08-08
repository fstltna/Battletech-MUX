
/*
   p.help.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:49 CET 1999 from help.c */

#ifndef _P_HELP_H
#define _P_HELP_H

/* help.c */
int helpindex_read(HASHTAB * htab, char *filename);
void helpindex_load(dbref player);
void helpindex_init(void);
void help_write(dbref player, char *topic, HASHTAB * htab, char *filename,
    int eval);
void do_help(dbref player, dbref cause, int key, char *message);

#endif				/* _P_HELP_H */
