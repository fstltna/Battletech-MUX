
/*
 * $Id: mech.stat.h 1.1 02/01/03 01:00:33-00:00 twouters@ $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1997 Markus Stenberg
 *  Copyright (c) 1998-2002 Thomas Wouters
 *  Copyright (c) 2000-2002 Cord Awtry
 *       All rights reserved
 *
 * Created: Tue Aug 12 19:07:18 1997 fingon
 * Last modified: Tue Aug 12 19:14:45 1997 fingon
 *
 */

#ifndef MECH_STAT_H
#define MECH_STAT_H

typedef struct {
    int rolls[11];
    int hitrolls[11];
    int critrolls[11];
    int totrolls;
    int tothrolls;
    int totcrolls;
} stat_type;

#ifndef MECH_STAT_C
extern stat_type rollstat;
#endif
#endif				/* MECH.STAT_H */
