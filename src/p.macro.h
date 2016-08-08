
/*
   p.macro.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:51 CET 1999 from macro.c */

#ifndef _P_MACRO_H
#define _P_MACRO_H

/* macro.c */
void init_mactab(void);
int do_macro(dbref player, char *in, char **out);
void do_list_macro(dbref player, char *s);
void do_add_macro(dbref player, char *s);
void do_del_macro(dbref player, char *s);
void do_desc_macro(dbref player, char *s);
void do_chmod_macro(dbref player, char *s);
void do_gex_macro(dbref player, char *s);
void do_edit_macro(dbref player, char *s);
void do_status_macro(dbref player, char *s);
void do_ex_macro(dbref player, char *s);
void do_chown_macro(dbref player, char *cmd);
void do_clear_macro(dbref player, char *s);
void do_def_macro(dbref player, char *cmd);
void do_undef_macro(dbref player, char *cmd);
char *do_process_macro(dbref player, char *in, char *s);
struct macros *get_macro_set(dbref player, int which);
void do_sort_macro_set(struct macros *m);
void do_create_macro(dbref player, char *s);
int can_write_macros(dbref player, struct macros *m);
int can_read_macros(dbref player, struct macros *m);
void load_macros(FILE * fp);
void save_macros(FILE * fp);

#endif				/* _P_MACRO_H */
