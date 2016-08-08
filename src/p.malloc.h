
/*
   p.malloc.h

   Automatically created by protomaker (C) 1998 Markus Stenberg (fingon@iki.fi)
   Protomaker is actually only a wrapper script for cproto, but well.. I like
   fancy headers and stuff :)
   */

/* Generated at Thu Mar 11 17:43:52 CET 1999 from malloc.c */

#ifndef _P_MALLOC_H
#define _P_MALLOC_H

/* malloc.c */
void *valloc(size_t size);
void *malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);
void *_realloc(void *ptr, size_t size);
void _free_internal(void *ptr);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void *calloc(register size_t nmemb, register size_t size);
void *__default_morecore(ptrdiff_t increment);
void *memalign(size_t alignment, size_t size);

#endif				/* _P_MALLOC_H */
