
/*
   p.speech.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:09 CET 1999 from speech.c */

#ifndef _P_SPEECH_H
#define _P_SPEECH_H

/* speech.c */
int sp_ok(dbref player);
void do_think(dbref player, dbref cause, int key, char *message);
void do_say(dbref player, dbref cause, int key, char *message);
void do_page(dbref player, dbref cause, int key, char *tname,
    char *message);
void whisper_pose(dbref player, dbref target, char *message);
void do_pemit_list(dbref player, char *list, const char *message);
void do_pemit(dbref player, dbref cause, int key, char *recipient,
    char *message);

#endif				/* _P_SPEECH_H */
