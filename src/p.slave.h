
/*
   p.slave.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:08 CET 1999 from slave.c */

#ifndef _P_SLAVE_H
#define _P_SLAVE_H

/* slave.c */
char *format_inet_addr(char *dest, long addr);
char *stpcpy(char *dest, const char *src);
void child_timeout_signal(void);
int query(char *ip, char *orig_arg);
void child_signal(void);
void alarm_signal(void);
void main(int argc, char **argv);

#endif				/* _P_SLAVE_H */
