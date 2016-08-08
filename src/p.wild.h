
/*
   p.wild.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:15 CET 1999 from wild.c */

#ifndef _P_WILD_H
#define _P_WILD_H

/* wild.c */
int quick_wild(char *tstr, char *dstr);
int wild1(char *tstr, char *dstr, int arg);
int wild(char *tstr, char *dstr, char *args[], int nargs);
int wild_match(char *tstr, char *dstr);

#endif				/* _P_WILD_H */
