
/*
   p.powers.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:58 CET 1999 from powers.c */

#ifndef _P_POWERS_H
#define _P_POWERS_H

/* powers.c */
int ph_any(dbref target, dbref player, POWER power, int fpowers,
    int reset);
int ph_god(dbref target, dbref player, POWER power, int fpowers,
    int reset);
int ph_wiz(dbref target, dbref player, POWER power, int fpowers,
    int reset);
int ph_wizroy(dbref target, dbref player, POWER power, int fpowers,
    int reset);
int ph_inherit(dbref target, dbref player, POWER power, int fpowers,
    int reset);
void init_powertab(void);
void display_powertab(dbref player);
POWERENT *find_power(dbref thing, char *powername);
int decode_power(dbref player, char *powername, POWERSET * pset);
void power_set(dbref target, dbref player, char *power, int key);
int has_power(dbref player, dbref it, char *powername);
char *power_description(dbref player, dbref target);
void decompile_powers(dbref player, dbref thing, char *thingname);

#endif				/* _P_POWERS_H */
