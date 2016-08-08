
/*
   p.sa-vattr.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:07 CET 1999 from sa-vattr.c */

#ifndef _P_SA_VATTR_H
#define _P_SA_VATTR_H

/* sa-vattr.c */
void vattr_init(void);
VATTR *vattr_find(char *name);
VATTR *vattr_alloc(char *name, int flags);
VATTR *vattr_define(char *name, int number, int flags);
void do_dbclean(dbref player, dbref cause, int key);
void vattr_delete(char *name);
VATTR *vattr_rename(char *name, char *newname);
VATTR *vattr_first(void);
VATTR *vattr_next(VATTR * vp);

#endif				/* _P_SA_VATTR_H */
