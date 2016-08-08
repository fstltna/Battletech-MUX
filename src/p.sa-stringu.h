
/*
   p.sa-stringu.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:44:04 CET 1999 from sa-stringu.c */

#ifndef _P_SA_STRINGU_H
#define _P_SA_STRINGU_H

/* sa-stringu.c */
char *translate_string(const char *str, int type);
char *upcasestr(char *s);
char *skip_space(const char *s);
char *seek_char(const char *s, int c);
char *munge_space(char *string);
char *trim_spaces(char *string);
char *grabto(char **str, int targ);
int string_compare(const char *s1, const char *s2);
int string_prefix(const char *string, const char *prefix);
const char *string_match(const char *src, const char *sub);
char *replace_string(const char *old, const char *new, const char *string);
char *replace_string_inplace(const char *old, const char *new,
    char *string);
int count_chars(const char *str, int c);
int prefix_match(const char *s1, const char *s2);
int minmatch(char *str, char *target, int min);
char *strsave(const char *s);
int safe_copy_str(char *src, char *buff, char **bufp, int max);
int safe_copy_chr(int src, char *buff, char **bufp, int max);
int matches_exit_from_list(char *str, char *pattern);

#endif				/* _P_SA_STRINGU_H */
