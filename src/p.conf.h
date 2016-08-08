
/*
   p.conf.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:42 CET 1999 from conf.c */

#ifndef _P_CONF_H
#define _P_CONF_H

/* conf.c */
void cf_init(void);
void cf_log_notfound(dbref player, char *cmd, const char *thingname,
    char *thing);
void cf_log_syntax(dbref player, char *cmd, const char *template,
    char *arg);
int cf_status_from_succfail(dbref player, char *cmd, int success,
    int failure);
int cf_int(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_bool(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_option(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_string(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_alias(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_flagalias(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_or_in_bits(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_modify_bits(int *vp, char *str, long extra, dbref player,
    char *cmd);
int cf_set_bits(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_set_flags(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_badname(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_site(long **vp, char *str, long extra, dbref player, char *cmd);
int cf_cf_access(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_include(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_set(char *cp, char *ap, dbref player);
void do_admin(dbref player, dbref cause, int extra, char *kw, char *value);
int cf_read(char *fn);
void list_cf_access(dbref player);

#endif				/* _P_CONF_H */
