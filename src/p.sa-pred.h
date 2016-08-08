
/*
   p.sa-pred.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:04 CET 1999 from sa-pred.c */

#ifndef _P_SA_PRED_H
#define _P_SA_PRED_H

/* sa-pred.c */
char *tprintf(char *format, ...);
void safe_tprintf_str(char *str, char **bp, char *format, ...);
dbref insert_first(dbref head, dbref thing);
dbref remove_first(dbref head, dbref thing);
dbref reverse_list(dbref list);
int member(dbref thing, dbref list);
int is_integer(char *str);
int is_number(char *str);
void add_quota(dbref who, int payment);
void giveto(dbref who, int pennies);
int ok_name(const char *name);
int ok_player_name(const char *name);
int ok_attr_name(const char *attrname);
int ok_password(const char *password);
dbref where_is(dbref what);
dbref where_room(dbref what);
int locatable(dbref player, dbref it, dbref cause);
int nearby(dbref player, dbref thing);
int exit_visible(dbref exit, dbref player, int key);
int exit_displayable(dbref exit, dbref player, int key);
dbref next_exit(dbref player, dbref this, int exam_here);

#endif				/* _P_SA_PRED_H */
