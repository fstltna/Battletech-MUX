
/*
   p.mail.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:52 CET 1999 from mail.c */

#ifndef _P_MAIL_H
#define _P_MAIL_H

/* mail.c */
char *get_mail_message(int number);
void do_mail_change_folder(dbref player, char *fld, char *newname);
void do_mail_tag(dbref player, char *msglist);
void do_mail_safe(dbref player, char *msglist);
void do_mail_clear(dbref player, char *msglist);
void do_mail_untag(dbref player, char *msglist);
void do_mail_unclear(dbref player, char *msglist);
void do_mail_file(dbref player, char *msglist, char *folder);
void do_mail_read(dbref player, char *msglist);
void do_mail_retract(dbref player, char *name, char *msglist);
void do_mail_review(dbref player, char *name, char *msglist);
void do_mail_list(dbref player, char *msglist, int sub);
void do_mail_purge(dbref player);
void do_mail_fwd(dbref player, char *msg, char *tolist);
struct mail *mail_fetch(dbref player, int num);
void count_mail(dbref player, int folder, int *rcount, int *ucount,
    int *ccount);
void urgent_mail(dbref player, int folder, int *ucount);
void do_mail_nuke(dbref player);
void do_mail_debug(dbref player, char *action, char *victim);
void do_mail_stats(dbref player, char *name, int full);
void do_mail_stub(dbref player, char *arg1, char *arg2);
void do_mail(dbref player, dbref cause, int key, char *arg1, char *arg2);
int dump_mail(FILE * fp);
int load_mail(FILE * fp);
void add_folder_name(dbref player, int fld, char *name);
void set_player_folder(dbref player, int fnum);
void check_mail_expiration(void);
void check_mail(dbref player, int folder, int silent);
void do_malias_switch(dbref player, char *a1, char *a2);
void do_malias(dbref player, dbref cause, int key, char *arg1, char *arg2);
void do_malias_send(dbref player, char *tolist, char *listto,
    char *subject, int number, mail_flag flags, int silent);
struct malias *get_malias(dbref player, char *alias);
void do_malias_create(dbref player, char *alias, char *tolist);
void do_malias_list(dbref player, char *alias);
void do_malias_list_all(dbref player);
void load_malias(FILE * fp);
void save_malias(FILE * fp);
void malias_read(FILE * fp);
void malias_write(FILE * fp);
void do_expmail_start(dbref player, char *arg, char *subject);
void do_mail_cc(dbref player, char *arg);
void do_mail_quick(dbref player, char *arg1, char *arg2);
void mail_to_list(dbref player, char *list, char *subject, char *message,
    int flags, int silent);
void do_expmail_stop(dbref player, int flags);
void do_expmail_abort(dbref player);
void do_prepend(dbref player, dbref cause, int key, char *text);
void do_postpend(dbref player, dbref cause, int key, char *text);
void do_malias_desc(dbref player, char *alias, char *desc);
void do_malias_chown(dbref player, char *alias, char *owner);
void do_malias_add(dbref player, char *alias, char *person);
void do_malias_remove(dbref player, char *alias, char *person);
void do_malias_rename(dbref player, char *alias, char *newname);
void do_malias_delete(dbref player, char *alias);
void do_malias_adminlist(dbref player);
void do_malias_status(dbref player);

#endif				/* _P_MAIL_H */
