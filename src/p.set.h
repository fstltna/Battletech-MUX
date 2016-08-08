
/*
   p.set.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:08 CET 1999 from set.c */

#ifndef _P_SET_H
#define _P_SET_H

/* set.c */
dbref match_controlled(dbref player, const char *name);
dbref match_controlled_quiet(dbref player, const char *name);
dbref match_affected(dbref player, const char *name);
dbref match_examinable(dbref player, const char *name);
void do_chzone(dbref player, dbref cause, int key, const char *name,
    const char *newobj);
void do_name(dbref player, dbref cause, int key, const char *name,
    char *newname);
void do_alias(dbref player, dbref cause, int key, char *name, char *alias);
void do_lock(dbref player, dbref cause, int key, char *name,
    char *keytext);
void do_unlock(dbref player, dbref cause, int key, char *name);
void do_unlink(dbref player, dbref cause, int key, char *name);
void do_chown(dbref player, dbref cause, int key, char *name,
    char *newown);
void set_attr_internal(dbref player, dbref thing, int attrnum,
    char *attrtext, int key);
void do_set(dbref player, dbref cause, int key, char *name, char *flag);
void do_power(dbref player, dbref cause, int key, char *name, char *flag);
void do_setattr(dbref player, dbref cause, int attrnum, char *name,
    char *attrtext);
void do_cpattr(dbref player, dbref cause, int key, char *oldpair,
    char *newpair[], int nargs);
void do_mvattr(dbref player, dbref cause, int key, char *what,
    char *args[], int nargs);
int parse_attrib(dbref player, char *str, dbref * thing, int *atr);
int parse_attrib_wild(dbref player, char *str, dbref * thing,
    int check_parents, int get_locks, int df_star);
void edit_string(char *src, char **dst, char *from, char *to);
void edit_string_ansi(char *src, char **dst, char **returnstr, char *from,
    char *to);
void do_edit(dbref player, dbref cause, int key, char *it, char *args[],
    int nargs);
void do_wipe(dbref player, dbref cause, int key, char *it);
void do_trigger(dbref player, dbref cause, int key, char *object,
    char *argv[], int nargs);
void do_use(dbref player, dbref cause, int key, char *object);
void do_setvattr(dbref player, dbref cause, int key, char *arg1,
    char *arg2);

#endif				/* _P_SET_H */
