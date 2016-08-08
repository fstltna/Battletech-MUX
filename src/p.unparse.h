
/*
   p.unparse.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:13 CET 1999 from unparse.c */

#ifndef _P_UNPARSE_H
#define _P_UNPARSE_H

/* unparse.c */
char *unparse_boolexp_quiet(dbref player, BOOLEXP * b);
char *unparse_boolexp(dbref player, BOOLEXP * b);
char *unparse_boolexp_decompile(dbref player, BOOLEXP * b);
char *unparse_boolexp_function(dbref player, BOOLEXP * b);

#endif				/* _P_UNPARSE_H */
