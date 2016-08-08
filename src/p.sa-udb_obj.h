
/*
   p.sa-udb_obj.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:05 CET 1999 from sa-udb_obj.c */

#ifndef _P_SA_UDB_OBJ_H
#define _P_SA_UDB_OBJ_H

/* sa-udb_obj.c */
Obj *objfromFILE(char *buff);
int objtoFILE(Obj * o, char *buff);
int obj_siz(Obj * o);

#endif				/* _P_SA_UDB_OBJ_H */
