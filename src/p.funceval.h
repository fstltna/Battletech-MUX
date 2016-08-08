
/*
   p.funceval.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:46 CET 1999 from funceval.c */

#ifndef _P_FUNCEVAL_H
#define _P_FUNCEVAL_H

/* funceval.c */
void fun_cwho(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_clist(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_beep(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_ansi(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_zone(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_link(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_tel(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_pemit(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_create(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_set(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_zwho(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_zplayers(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_inzone(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_children(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_encrypt(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_decrypt(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_objeval(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_squish(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_stripansi(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_zfun(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_columns(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_objmem(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_playmem(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_orflags(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_andflags(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_strtrunc(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_ifelse(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_inc(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_dec(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_mail(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_mailfrom(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_hasattr(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_hasattrp(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_default(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_edefault(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_udefault(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_findable(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_isword(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_visible(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_elements(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_grab(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_scramble(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_shuffle(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_sortby(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_last(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_matchall(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_ports(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_mix(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_foreach(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_munge(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
int getrandom(int x);
void fun_die(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_lit(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_shl(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_shr(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_vadd(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_vsub(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_vmul(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_vmag(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_vunit(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_vdim(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_strcat(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
char *grep_util(dbref player, dbref thing, char *pattern, char *lookfor,
    int len, int insensitive);
void fun_grep(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_grepi(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_art(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_alphamax(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_alphamin(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_valid(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_hastype(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_lparent(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
int stacksize(dbref doer);
void fun_lstack(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_empty(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_items(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_peek(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_pop(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_push(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_regmatch(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);
void fun_translate(char *buff, char **bufc, dbref player, dbref cause,
    char *fargs[], int nfargs, char *cargs[], int ncargs);

#endif				/* _P_FUNCEVAL_H */
