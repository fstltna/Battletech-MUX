
/*
   p.commac.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:40 CET 1999 from commac.c */

#ifndef _P_COMMAC_H
#define _P_COMMAC_H

/* commac.c */
void load_comsys_and_macros(char *filename);
void save_comsys_and_macros(char *filename);
void load_commac(FILE * fp);
void purge_commac(void);
void save_commac(FILE * fp);
struct commac *create_new_commac(void);
struct commac *get_commac(dbref which);
void add_commac(struct commac *c);
void del_commac(dbref who);
void destroy_commac(struct commac *c);
void sort_com_aliases(struct commac *c);

#endif				/* _P_COMMAC_H */
