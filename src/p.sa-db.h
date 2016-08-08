
/*
   p.sa-db.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:00 CET 1999 from sa-db.c */

#ifndef _P_SA_DB_H
#define _P_SA_DB_H

/* sa-db.c */
int fwdlist_load(FWDLIST * fp, dbref player, char *atext);
int fwdlist_rewrite(FWDLIST * fp, char *atext);
int fwdlist_ck(int key, dbref player, dbref thing, int anum, char *atext);
FWDLIST *fwdlist_get(dbref thing);
char *Name(dbref thing);
char *PureName(dbref thing);
void s_Name(dbref thing, char *s);
void s_Pass(dbref thing, const char *s);
ATTR *atr_str(char *s);
void anum_extend(int newtop);
ATTR *atr_num(int anum);
int mkattr(char *buff);
int Commer(dbref thing);
void al_extend(char **buffer, int *bufsiz, int len, int copy);
int al_size(char *astr);
void al_store(void);
char *al_fetch(dbref thing);
void al_add(dbref thing, int attrnum);
void al_delete(dbref thing, int attrnum);
void al_destroy(dbref thing);
void atr_clr(dbref thing, int atr);
void atr_add_raw(dbref thing, int atr, char *buff);
void atr_add(dbref thing, int atr, char *buff, dbref owner, int flags);
void atr_set_owner(dbref thing, int atr, dbref owner);
void atr_set_flags(dbref thing, int atr, dbref flags);
int get_atr(char *name);
char *atr_get_raw(dbref thing, int atr);
char *atr_get_str(char *s, dbref thing, int atr, dbref * owner,
    int *flags);
char *atr_get(dbref thing, int atr, dbref * owner, int *flags);
int atr_get_info(dbref thing, int atr, dbref * owner, int *flags);
void atr_free(dbref thing);
void atr_collect(dbref thing);
void atr_cpy(dbref player, dbref dest, dbref source);
void atr_chown(dbref obj);
int atr_next(char **attrp);
void atr_push(void);
void atr_pop(void);
int atr_head(dbref thing, char **attrp);
void initialize_objects(dbref first, dbref last);
void db_grow(dbref newtop);
void db_free(void);
dbref parse_dbref(const char *s);
void putref(FILE * f, dbref ref);
void putstring(FILE * f, const char *s);
const char *getstring_noalloc(FILE * f, int new_strings);
dbref getref(FILE * f);
void free_boolexp(BOOLEXP * b);
BOOLEXP *dup_bool(BOOLEXP * b);
void clone_object(dbref a, dbref b);
int init_gdbm_db(char *gdbmfile);
int check_zone(dbref player, dbref thing);

#endif				/* _P_SA_DB_H */
