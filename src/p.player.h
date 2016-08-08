
/*
   p.player.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:56 CET 1999 from player.c */

#ifndef _P_PLAYER_H
#define _P_PLAYER_H

/* player.c */
void record_login(dbref player, int isgood, char *ldate, char *lhost,
    char *lusername);
int check_pass(dbref player, const char *password);
dbref connect_player(char *name, char *password, char *host,
    char *username);
dbref create_player(char *name, char *password, dbref creator, int isrobot,
    int isguest);
void do_password(dbref player, dbref cause, int key, char *oldpass,
    char *newpass);
void do_last(dbref player, dbref cause, int key, char *who);
int add_player_name(dbref player, char *name);
int delete_player_name(dbref player, char *name);
dbref lookup_player(dbref doer, char *name, int check_who);
void load_player_names(void);
void badname_add(char *bad_name);
void badname_remove(char *bad_name);
int badname_check(char *bad_name);
void badname_list(dbref player, const char *prefix);

#endif				/* _P_PLAYER_H */
