
/*
   p.sa-db_rw.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:01 CET 1999 from sa-db_rw.c */

#ifndef _P_SA_DB_RW_H
#define _P_SA_DB_RW_H

/* sa-db_rw.c */
void efo_convert(void);
void unscraw_foreign(int db_format, int db_version, int db_flags);
dbref db_read(FILE * f, int *db_format, int *db_version, int *db_flags);
dbref db_write(FILE * f, int format, int version);

#endif				/* _P_SA_DB_RW_H */
