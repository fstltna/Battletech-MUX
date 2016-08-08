
/*
   p.log.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:50 CET 1999 from log.c */

#ifndef _P_LOG_H
#define _P_LOG_H

/* log.c */
char *strip_ansi(const char *raw);
char *normal_to_white(const char *raw);
int start_log(const char *primary, const char *secondary);
void end_log(void);
void log_perror(const char *primary, const char *secondary,
    const char *extra, const char *failing_object);
void log_text(char *text);
void log_number(int num);
void log_name(dbref target);
void log_name_and_loc(dbref player);
char *OBJTYP(dbref thing);
void log_type_and_name(dbref thing);
void log_type_and_num(dbref thing);

#endif				/* _P_LOG_H */
