
/*
   p.match.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:52 CET 1999 from match.c */

#ifndef _P_MATCH_H
#define _P_MATCH_H

/* match.c */
void match_player(void);
void match_absolute(void);
void match_numeric(void);
void match_me(void);
void match_home(void);
void match_here(void);
void match_possession(void);
void match_neighbor(void);
void match_exit(void);
void match_exit_with_parents(void);
void match_carried_exit(void);
void match_carried_exit_with_parents(void);
void match_master_exit(void);
void match_zone_exit(void);
void match_everything(int key);
dbref match_result(void);
dbref last_match_result(void);
dbref match_status(dbref player, dbref match);
dbref noisy_match_result(void);
dbref dispatched_match_result(dbref player);
int matched_locally(void);
void save_match_state(MSTATE * mstate);
void restore_match_state(MSTATE * mstate);
void init_match(dbref player, const char *name, int type);
void init_match_check_keys(dbref player, const char *name, int type);

#endif				/* _P_MATCH_H */
