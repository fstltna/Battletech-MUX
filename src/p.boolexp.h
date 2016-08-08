
/*
   p.boolexp.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:39 CET 1999 from boolexp.c */

#ifndef _P_BOOLEXP_H
#define _P_BOOLEXP_H

/* boolexp.c */
int eval_boolexp(dbref player, dbref thing, dbref from, BOOLEXP * b);
int eval_boolexp_atr(dbref player, dbref thing, dbref from, char *key);
BOOLEXP *parse_boolexp(dbref player, const char *buf, int internal);

#endif				/* _P_BOOLEXP_H */
