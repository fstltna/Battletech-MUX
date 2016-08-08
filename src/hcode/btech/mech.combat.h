
/*
 * $Id: mech.combat.h 1.2 02/02/25 16:15:37-00:00 twouters@ $
 *
 * Author: Cord Awtry <kipsta@mediaone.net>
 *  Copyright (c) 2000-2002 Cord Awtry
 *       All rights reserved
 *
 * Based on work that was:
 *  Copyright (c) 1997 Markus Stenberg
 *  Copyright (c) 1998-2000 Thomas Wouters
 */

#ifndef MECH_COMBAT_H
#define MECH_COMBAT_H

#define Clustersize(weapindx) (((MechWeapons[weapindx].special & (IDF | MRM | ROCKET)) && (MechWeapons[weapindx].damage == 1))? 5 : 1)

#define Swap(val1,val2) { rtmp = val1 ; val1 = val2 ; val2 = rtmp; }

#endif				/* MECH_COMBAT_H.ECM_H */
