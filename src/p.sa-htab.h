
/*
   p.sa-htab.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:02 CET 1999 from sa-htab.c */

#ifndef _P_SA_HTAB_H
#define _P_SA_HTAB_H

/* sa-htab.c */
int hashval(char *str, int hashmask);
int get_hashmask(int *size);
void hashinit(HASHTAB * htab, int size);
void hashreset(HASHTAB * htab);
int *hashfind(char *str, HASHTAB * htab);
int hashadd(char *str, int *hashdata, HASHTAB * htab);
void hashdelete(char *str, HASHTAB * htab);
void hashflush(HASHTAB * htab, int size);
int hashrepl(char *str, int *hashdata, HASHTAB * htab);
void hashreplall(int *old, int *new, HASHTAB * htab);
char *hashinfo(const char *tab_name, HASHTAB * htab);
int *hash_firstentry(HASHTAB * htab);
int *hash_nextentry(HASHTAB * htab);
char *hash_firstkey(HASHTAB * htab);
char *hash_nextkey(HASHTAB * htab);

#endif				/* _P_SA_HTAB_H */
