
/*
   p.predicates.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:58 CET 1999 from predicates.c */

#ifndef _P_PREDICATES_H
#define _P_PREDICATES_H

/* predicates.c */
char *tprintf(const char *format, ...);
void safe_tprintf_str(char *str, char **bp, const char *format, ...);
dbref insert_first(dbref head, dbref thing);
dbref remove_first(dbref head, dbref thing);
dbref reverse_list(dbref list);
int member(dbref thing, dbref list);
int is_integer(char *str);
int is_number(char *str);
int could_doit(dbref player, dbref thing, int locknum);
int can_see(dbref player, dbref thing, int can_see_loc);
int canpayfees(dbref player, dbref who, int pennies, int quota);
int payfor(dbref who, int cost);
void add_quota(dbref who, int payment);
void giveto(dbref who, int pennies);
int ok_name(const char *name);
int ok_player_name(const char *name);
int ok_attr_name(const char *attrname);
int ok_password(const char *password);
void handle_ears(dbref thing, int could_hear, int can_hear);
void do_switch(dbref player, dbref cause, int key, char *expr,
    char *args[], int nargs, char *cargs[], int ncargs);
void do_addcommand(dbref player, dbref cause, int key, char *name,
    char *command);
void do_listcommands(dbref player, dbref cause, int key, char *name);
void do_delcommand(dbref player, dbref cause, int key, char *name,
    char *command);
void handle_prog(DESC * d, char *message);
void do_quitprog(dbref player, dbref cause, int key, char *name);
void do_prog(dbref player, dbref cause, int key, char *name,
    char *command);
void do_restart(int player, int cause, int key);
void do_comment(dbref player, dbref cause, int key);
dbref match_possessed(dbref player, dbref thing, char *target, dbref dflt,
    int check_enter);
void parse_range(char **name, dbref * low_bound, dbref * high_bound);
int parse_thing_slash(dbref player, char *thing, char **after, dbref * it);
int get_obj_and_lock(dbref player, char *what, dbref * it, ATTR ** attr,
    char *errmsg, char **bufc);
dbref where_is(dbref what);
dbref where_room(dbref what);
int locatable(dbref player, dbref it, dbref cause);
int nearby(dbref player, dbref thing);
int exit_visible(dbref exit, dbref player, int key);
int exit_displayable(dbref exit, dbref player, int key);
dbref next_exit(dbref player, dbref this, int exam_here);
void did_it(dbref player, dbref thing, int what, const char *def,
    int owhat, const char *odef, int awhat, char *args[], int nargs);
void do_verb(dbref player, dbref cause, int key, char *victim_str,
    char *args[], int nargs);

#endif				/* _P_PREDICATES_H */
