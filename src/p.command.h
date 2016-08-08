
/*
   p.command.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:40 CET 1999 from command.c */

#ifndef _P_COMMAND_H
#define _P_COMMAND_H

/* command.c */
void init_cmdtab(void);
void set_prefix_cmds(void);
int In_IC_Loc(dbref player);
int check_access(dbref player, int mask);
void process_cmdent(CMDENT * cmdp, char *switchp, dbref player,
    dbref cause, int interactive, char *arg, char *unp_command,
    char *cargs[], int ncargs);
void process_command(dbref player, dbref cause, int interactive,
    char *command, char *args[], int nargs);
int cf_access(int *vp, char *str, long extra, dbref player, char *cmd);
int cf_acmd_access(int *vp, char *str, long extra, dbref player,
    char *cmd);
int cf_attr_access(int *vp, char *str, long extra, dbref player,
    char *cmd);
int cf_cmd_alias(int *vp, char *str, long extra, dbref player, char *cmd);
void list_hashstat(dbref player, const char *tab_name, HASHTAB * htab);
void do_list(dbref player, dbref cause, int extra, char *arg);

#endif				/* _P_COMMAND_H */
