
/*
 * $Id: aero.bomb.h 1.1 02/01/03 01:01:15-00:00 twouters@ $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1997 Markus Stenberg
 *  Copyright (c) 1998-2002 Thomas Wouters
 *  Copyright (c) 2000-2002 Cord Awtry 
 *       All rights reserved
 *
 * Created: Mon Jan  6 15:59:37 1997 fingon
 * Last modified: Mon Jan  6 16:57:26 1997 fingon
 *
 */

#ifndef AERO_BOMB_H
#define AERO_BOMB_H

typedef struct {
    char *name;
    int aff;
    int type;			/* 0 = standard, 1 = inferno, 2 = cluster */
    int weight;
} BOMBINFO;

typedef struct {
    int x, y, type;
    MAP *map;
} bomb_shot;

#endif				/* AERO_BOMB_H */
