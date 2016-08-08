
/*
   p.regexp.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:59 CET 1999 from regexp.c */

#ifndef _P_REGEXP_H
#define _P_REGEXP_H

/* regexp.c */
void regerror(char *msg);
regexp *regcomp(char *exp);
int regexec(regexp * prog, char *string);

#endif				/* _P_REGEXP_H */
