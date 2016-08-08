
/*
   p.eval.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:45 CET 1999 from eval.c */

#ifndef _P_EVAL_H
#define _P_EVAL_H

/* eval.c */
char *parse_to(char **dstr, int delim, int eval);
char *parse_arglist(dbref player, dbref cause, char *dstr, int delim,
    dbref eval, char *fargs[], dbref nfargs, char *cargs[], dbref ncargs);
int get_gender(dbref player);
void tcache_init(void);
int tcache_empty(void);
void exec(char *buff, char **bufc, int tflags, dbref player, dbref cause,
    int eval, char **dstr, char *cargs[], int ncargs);

#endif				/* _P_EVAL_H */
