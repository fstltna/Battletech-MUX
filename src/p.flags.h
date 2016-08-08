
/*
   p.flags.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:46 CET 1999 from flags.c */

#ifndef _P_FLAGS_H
#define _P_FLAGS_H

/* flags.c */
int fh_any(dbref target, dbref player, FLAG flag, int fflags, int reset);
int fh_god(dbref target, dbref player, FLAG flag, int fflags, int reset);
int fh_wiz(dbref target, dbref player, FLAG flag, int fflags, int reset);
int fh_fixed(dbref target, dbref player, FLAG flag, int fflags, int reset);
int fh_wizroy(dbref target, dbref player, FLAG flag, int fflags,
    int reset);
int fh_inherit(dbref target, dbref player, FLAG flag, int fflags,
    int reset);
int fh_wiz_bit(dbref target, dbref player, FLAG flag, int fflags,
    int reset);
int fh_dark_bit(dbref target, dbref player, FLAG flag, int fflags,
    int reset);
int fh_going_bit(dbref target, dbref player, FLAG flag, int fflags,
    int reset);
int fh_hear_bit(dbref target, dbref player, FLAG flag, int fflags,
    int reset);
int fh_xcode_bit(dbref target, dbref player, FLAG flag, int fflags,
    int reset);
void init_flagtab(void);
void display_flagtab(dbref player);
FLAGENT *find_flag(dbref thing, char *flagname);
void flag_set(dbref target, dbref player, char *flag, int key);
char *decode_flags(dbref player, FLAG flagword, FLAG flag2word,
    FLAG flag3word);
int has_flag(dbref player, dbref it, char *flagname);
char *flag_description(dbref player, dbref target);
char *unparse_object_numonly(dbref target);
char *unparse_object(dbref player, dbref target, int obey_myopic);
int convert_flags(dbref player, char *flaglist, FLAGSET * fset,
    FLAG * p_type);
void decompile_flags(dbref player, dbref thing, char *thingname);

#endif				/* _P_FLAGS_H */
