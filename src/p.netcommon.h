
/*
   p.netcommon.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:55 CET 1999 from netcommon.c */

#ifndef _P_NETCOMMON_H
#define _P_NETCOMMON_H

/* netcommon.c */
void make_portlist(dbref player, dbref target, char *buff, char **bufc);
struct timeval timeval_sub(struct timeval now, struct timeval then);
int msec_diff(struct timeval now, struct timeval then);
struct timeval msec_add(struct timeval t, int x);
struct timeval update_quotas(struct timeval last, struct timeval current);
void raw_notify_html(dbref player, const char *msg);
void raw_notify(dbref player, const char *msg);
void raw_notify_newline(dbref player);
void raw_broadcast(int inflags, char *template, ...);
void clearstrings(DESC * d);
void queue_write(DESC * d, const char *b, int n);
void queue_string(DESC * d, const char *s);
void freeqs(DESC * d);
void desc_addhash(DESC * d);
void welcome_user(DESC * d);
void save_command(DESC * d, CBLK * command);
void announce_disconnect(dbref player, DESC * d, const char *reason);
int boot_off(dbref player, char *message);
int boot_by_port(int port, int no_god, char *message);
void desc_reload(dbref player);
int fetch_idle(dbref target);
int fetch_connect(dbref target);
void check_idle(void);
void check_events(void);
void do_doing(dbref player, dbref cause, int key, char *arg);
void init_logout_cmdtab(void);
int do_command(DESC * d, char *command, int first);
void logged_out(dbref player, dbref cause, int key, char *arg);
void process_commands(void);
int site_check(struct in_addr host, SITE * site_list);
void list_siteinfo(dbref player);
void make_ulist(dbref player, char *buff, char **bufc);
dbref find_connected_name(dbref player, char *name);

#endif				/* _P_NETCOMMON_H */
