
/*
   p.bsd.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:39 CET 1999 from bsd.c */

#ifndef _P_BSD_H
#define _P_BSD_H

/* bsd.c */
void set_lastsite(DESC * d, char *lastsite);
void boot_slave(void);
int make_socket(int port);
void shovechars(int port);
DESC *new_connection(int sock);
void shutdownsock(DESC * d, int reason);
void make_nonblocking(int s);
DESC *initializesock(int s, struct sockaddr_in *a);
int process_output(DESC * d);
int fatal_bug(void);
int process_input(DESC * d);
void close_sockets(int emergency, char *message);
void emergency_shutdown(void);
void set_signals(void);
void log_signal(const char *signame);
void log_commands(int sig);

#endif				/* _P_BSD_H */
