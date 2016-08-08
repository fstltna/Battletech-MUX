
/*
 * $Id: luck.c 1.1 02/01/03 01:00:28-00:00 twouters@ $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1998 Markus Stenberg
 *       All rights reserved
 *
 * Created: Tue Mar 10 16:51:50 1998 fingon
 * Last modified: Thu Dec 10 22:51:02 1998 fingon
 *
 */

#include "mech.h"
#include "p.mech.utils.h"

int player_luck(dbref player)
{
    char *buf;

    if (!Good_obj(player))
	return 0;
    buf = silly_atr_get(player, A_LUCK);
    if (!buf || !*buf)
	return 0;
    return BOUNDED(-15, atoi(buf), 15);
}

int luck_die_mod_base(int mod, int l)
{
    return Roll();
}

int luck_die_mod(dbref player, int mod)
{
    return luck_die_mod_base(mod, player_luck(player));
}
