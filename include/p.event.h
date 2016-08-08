
/*
   p.event.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Sat Jul 18 04:14:31 EEST 1998 from event.c */

#ifndef _P_EVENT_H
#define _P_EVENT_H

/* event.c */
void event_add(int time, int flags, int type, void (*func) (EVENT *),
    void *data, void *data2);
void event_run(void);
int event_run_by_type(int type);
int event_last_type(void);
void event_initialize(void);
void event_remove_data(void *data);
void event_remove_type_data(int type, void *data);
void event_remove_type_data2(int type, void *data);
void event_remove_type_data_data(int type, void *data, void *data2);
int event_type_data(int type, void *data);
void event_get_type_data(int type, void *data, int *data2);
int event_count_type(int type);
int event_count_type_data(int type, void *data);
int event_count_type_data2(int type, void *data);
int event_count_type_data_data(int type, void *data, void *data2);
int event_count_data(int type, void *data);
int event_count_data_data(int type, void *data, void *data2);
void event_gothru_type_data(int type, void *data, void (*func) (EVENT *));
void event_gothru_type(int type, void (*func) (EVENT *));
int event_last_type_data(int type, void *data);
int event_first_type_data(int type, void *data);

#endif				/* _P_EVENT_H */
