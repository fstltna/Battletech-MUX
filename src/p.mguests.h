
/*
   p.mguests.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:53 CET 1999 from mguests.c */

#ifndef _P_MGUESTS_H
#define _P_MGUESTS_H

/* mguests.c */
dbref create_guest(char *name, char *password);
void destroy_guest(dbref guest);
char *make_guest(DESC * d);

#endif				/* _P_MGUESTS_H */
