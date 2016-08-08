
/*
   p.htab.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:50 CET 1999 from htab.c */

#ifndef _P_HTAB_H
#define _P_HTAB_H

/* htab.c */
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
int *nhash_firstentry(NHSHTAB * htab);
int *nhash_nextentry(NHSHTAB * htab);
int *nhashfind(int val, NHSHTAB * htab);
int nhashadd(int val, int *hashdata, NHSHTAB * htab);
void nhashdelete(int val, NHSHTAB * htab);
void nhashflush(NHSHTAB * htab, int size);
int nhashrepl(int val, int *hashdata, NHSHTAB * htab);
int search_nametab(dbref player, NAMETAB * ntab, char *flagname);
NAMETAB *find_nametab_ent(dbref player, NAMETAB * ntab, char *flagname);
void display_nametab(dbref player, NAMETAB * ntab, char *prefix,
    int list_if_none);
void interp_nametab(dbref player, NAMETAB * ntab, int flagword,
    char *prefix, char *true_text, char *false_text);
void listset_nametab(dbref player, NAMETAB * ntab, int flagword,
    char *prefix, int list_if_none);
int cf_ntab_access(int *vp, char *str, long extra, dbref player,
    char *cmd);

#endif				/* _P_HTAB_H */
