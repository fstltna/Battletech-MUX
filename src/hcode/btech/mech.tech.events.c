
/*
 * $Id: mech.tech.events.c 1.2 02/02/01 15:02:47-00:00 twouters@ $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1996 Markus Stenberg
 *  Copyright (c) 1998-2002 Thomas Wouters
 *  Copyright (c) 2000-2002 Cord Awtry
 *       All rights reserved
 *
 * Created: Sun Sep  1 16:06:19 1996 fingon
 * Last modified: Sun Jun 14 16:45:08 1998 fingon
 *
 */

#include "mech.h"
#include "event.h"
#include "mech.events.h"
#include "mech.tech.h"
#include "failures.h"
#include "p.econ.h"
#include "p.mech.utils.h"
#include "p.mech.tech.do.h"
#include "p.mech.status.h"

#define VERBOSE_ENDS



static int completely_intact_int(MECH * mech)
{
    int i;

    for (i = 0; i < NUM_SECTIONS; i++)
	if (!GetSectInt(mech, i) && GetSectOInt(mech, i))
	    return 0;
    return 1;
}

void event_mech_removesection(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    char buf[MBUF_SIZE];
    int loc, pos, extra;

    /* changed Special2I to Special on AddPartsM statements */
    UNPACK_LOCPOS_E(earg, loc, pos, extra);
    AddPartsM(mech, ProperInternal(mech), 0, (2 * GetSectInt(mech,
		loc)) / extra);
    AddPartsM(mech, ProperArmor(mech), 0, (2 * GetSectArmor(mech,
		loc)) / extra);
    AddPartsM(mech, Cargo(S_ELECTRONIC), 0, (GetSectInt(mech,
		loc)) / extra);
    mech_Detach(mech, loc);
    ArmorStringFromIndex(loc, buf, MechType(mech), MechMove(mech));
    do {
	int i = 0;

	if (Destroyed(mech))
	    i = 1;
	MechStatus(mech) &= ~DESTROYED;
	mech_notify(mech, MECHALL, tprintf("%s has been removed.", buf));
	if (i)
	    MechStatus(mech) |= DESTROYED;
    } while (0);
}

void event_mech_removegun(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    int loc, pos, i, extra;
    char buf[MBUF_SIZE];

    UNPACK_LOCPOS_E(earg, loc, pos, extra);
    for (i = pos;
	i < (pos + GetWeaponCrits(mech, Weapon2I(GetPartType(mech, loc,
			pos)))); i++)
	DestroyPart(mech, loc, i);
    ArmorStringFromIndex(loc, buf, MechType(mech), MechMove(mech));
    if (extra == 2 && (e->function != very_fake_func)) {
	AddPartsM(mech, FindAmmoType(mech, loc, pos), GetPartBrand(mech,
		loc, pos), 1);
	do {
	    int i = 0;

	    if (Destroyed(mech))
		i = 1;
	    MechStatus(mech) &= ~DESTROYED;
	    mech_notify(mech, MECHALL,
		tprintf("%s from %s has been removed.", pos_part_name(mech,
			loc, pos), buf));
	    if (i)
		MechStatus(mech) |= DESTROYED;
	} while (0);
    } else {
	do {
	    int i = 0;

	    if (Destroyed(mech))
		i = 1;
	    MechStatus(mech) &= ~DESTROYED;
	    mech_notify(mech, MECHALL,
		tprintf("%s from %s has been removed and scrapped.",
		    pos_part_name(mech, loc, pos), buf));
	    if (i)
		MechStatus(mech) |= DESTROYED;
	} while (0);
    }
}


void event_mech_removepart(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    int loc, pos, extra;
    char buf[MBUF_SIZE];

    UNPACK_LOCPOS_E(earg, loc, pos, extra);
    DestroyPart(mech, loc, pos);
    if (MechType(mech) == CLASS_MECH)
	do_magic(mech);
    ArmorStringFromIndex(loc, buf, MechType(mech), MechMove(mech));
    if (extra == 2 && (e->function != very_fake_func)) {
	AddPartsM(mech, FindAmmoType(mech, loc, pos), GetPartBrand(mech,
		loc, pos), 1);
	do {
	    int i = 0;

	    if (Destroyed(mech))
		i = 1;
	    MechStatus(mech) &= ~DESTROYED;
	    mech_notify(mech, MECHALL,
		tprintf("%s from %s has been removed.", pos_part_name(mech,
			loc, pos), buf));
	    if (i)
		MechStatus(mech) |= DESTROYED;
	} while (0);
    } else {
	do {
	    int i = 0;

	    if (Destroyed(mech))
		i = 1;
	    MechStatus(mech) &= ~DESTROYED;
	    mech_notify(mech, MECHALL,
		tprintf("%s from %s has been removed and scrapped.",
		    pos_part_name(mech, loc, pos), buf));
	    if (i)
		MechStatus(mech) |= DESTROYED;
	} while (0);
    }
}

void event_mech_repairarmor(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    int loc = earg % 16;
    int amount = (earg / 16) % 256;
    int player = ((int) e->data2) / PLAYERPOS;
    char buf[MBUF_SIZE];

    if (loc >= 8) {
	SetSectRArmor(mech, loc % 8, MIN(GetSectRArmor(mech, loc % 8) + 1,
		GetSectORArmor(mech, loc % 8)));
    } else {
	SetSectArmor(mech, loc, MIN(GetSectArmor(mech, loc) + 1,
		GetSectOArmor(mech, loc)));
    }
    amount--;
    if (amount < 0)
	return;
    if (amount <= 0) {
	ArmorStringFromIndex(loc % 8, buf, MechType(mech), MechMove(mech));
	if (loc >= 8) {
	    do {
		int i = 0;

		if (Destroyed(mech))
		    i = 1;
		MechStatus(mech) &= ~DESTROYED;
		mech_notify(mech, MECHALL,
		    tprintf("%s's rear armor repairs have been finished.",
			buf));
		if (i)
		    MechStatus(mech) |= DESTROYED;
	    } while (0);
	} else {
	    do {
		int i = 0;

		if (Destroyed(mech))
		    i = 1;
		MechStatus(mech) &= ~DESTROYED;
		mech_notify(mech, MECHALL,
		    tprintf("%s's armor repairs have been finished.",
			buf));
		if (i)
		    MechStatus(mech) |= DESTROYED;
	    } while (0);
	}

	if (MechType(mech) != CLASS_MECH && completely_intact_int(mech))
	    do_magic(mech);
	return;
    }
    REPAIREVENT(FIXARMOR_TIME, mech, (amount * 16 + loc),
	event_mech_repairarmor, EVENT_REPAIR_FIX);
}

void event_mech_repairinternal(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    int loc = earg % 16;
    int amount = (earg / 16) % 256;
    int player = ((int) e->data2) / PLAYERPOS;
    char buf[MBUF_SIZE];

    SetSectInt(mech, loc, GetSectInt(mech, loc) + 1);
    if (GetSectInt(mech, loc) > GetSectOInt(mech, loc))
	SetSectInt(mech, loc, GetSectOInt(mech, loc));
    amount--;
    if (amount < 0)
	return;
    if (amount <= 0) {
	ArmorStringFromIndex(loc, buf, MechType(mech), MechMove(mech));
	do {
	    int i = 0;

	    if (Destroyed(mech))
		i = 1;
	    MechStatus(mech) &= ~DESTROYED;
	    mech_notify(mech, MECHALL,
		tprintf("%s's internal repairs have been finished.", buf));
	    if (i)
		MechStatus(mech) |= DESTROYED;
	} while (0);
	if (MechType(mech) != CLASS_MECH && completely_intact_int(mech))
	    do_magic(mech);
	return;
    }
    REPAIREVENT(FIXINTERNAL_TIME, mech, (amount * 16 + loc),
	event_mech_repairinternal, EVENT_REPAIR_FIXI);
}

void event_mech_reattach(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    char buf[MBUF_SIZE];

    /* Basically: Unset the limb destroyed, without doing a thing to
       damaged parts */
    mech_ReAttach(mech, earg);
    ArmorStringFromIndex(earg, buf, MechType(mech), MechMove(mech));
    if (completely_intact_int(mech))
	do_magic(mech);
    do {
	int i = 0;

	if (Destroyed(mech))
	    i = 1;
	MechStatus(mech) &= ~DESTROYED;
	mech_notify(mech, MECHALL, tprintf("%s has been reattached.",
		buf));
	if (i)
	    MechStatus(mech) |= DESTROYED;
    } while (0);
}

void event_mech_replacesuit(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    char buf[MBUF_SIZE];

    ArmorStringFromIndex(earg, buf, MechType(mech), MechMove(mech));
    mech_ReplaceSuit(mech, earg);
    do_magic(mech);

    mech_notify(mech, MECHALL, tprintf("%s has been replaced.", buf));
}

/*
 * Added for new flood code by Kipsta
 * 8/4/99
 */

void event_mech_reseal(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    char buf[MBUF_SIZE];

    mech_ReSeal(mech, earg);
    ArmorStringFromIndex(earg, buf, MechType(mech), MechMove(mech));
    mech_notify(mech, MECHALL, tprintf("%s has been resealed.", buf));
}

void event_mech_replacegun(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    int loc, pos, i, brand;
    char buf[MBUF_SIZE];

    UNPACK_LOCPOS_E(earg, loc, pos, brand);
    for (i = pos;
	i < (pos + GetWeaponCrits(mech, Weapon2I(GetPartType(mech, loc,
			pos)))); i++) {
	mech_RepairPart(mech, loc, i);
	ClearTempNuke(mech, loc, i);
	if (brand) {
	    SetPartBrand(mech, loc, i, brand);
	}
    }
    ArmorStringFromIndex(loc, buf, MechType(mech), MechMove(mech));
    do {
	int i = 0;

	if (Destroyed(mech))
	    i = 1;
	MechStatus(mech) &= ~(DESTROYED);
	mech_notify(mech, MECHALL, tprintf("%s on %s has been replaced.",
		pos_part_name(mech, loc, pos), buf));
	if (i)
	    MechStatus(mech) |= DESTROYED;
    } while (0);
}

void event_mech_repairgun(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    int loc, pos, i;
    char buf[MBUF_SIZE];

    UNPACK_LOCPOS(earg, loc, pos);
    for (i = pos;
	i < (pos + GetWeaponCrits(mech, Weapon2I(GetPartType(mech, loc,
			pos)))); i++) {
	mech_RepairPart(mech, loc, i);
	ClearTempNuke(mech, loc, i);
    }
    ArmorStringFromIndex(loc, buf, MechType(mech), MechMove(mech));
    do {
	int i = 0;

	if (Destroyed(mech))
	    i = 1;
	MechStatus(mech) &= ~DESTROYED;
	mech_notify(mech, MECHALL, tprintf("%s on %s has been repaired.",
		pos_part_name(mech, loc, pos), buf));
	if (i)
	    MechStatus(mech) |= DESTROYED;
    } while (0);
}

void event_mech_repairenhcrit(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    int loc, pos;
    char buf[MBUF_SIZE];
    int wCritType, wWeapSize, wFirstCrit;

    UNPACK_LOCPOS(earg, loc, pos);
    ArmorStringFromIndex(loc, buf, MechType(mech), MechMove(mech));
    mech_notify(mech, MECHALL, tprintf("%s on %s has been repaired.",
	    pos_part_name(mech, loc, pos), buf));
    UnDamagePart(mech, loc, pos);

    /* Get the crit type */
    wCritType = GetPartType(mech, loc, pos);

    /* Get the max number of crits for this weapon */
    wWeapSize = GetWeaponCrits(mech, Weapon2I(wCritType));

    /* Find the first crit */
    wFirstCrit =
	FindFirstWeaponCrit(mech, loc, pos, 0, wCritType, wWeapSize);

    SetPartTempNuke(mech, loc, wFirstCrit, 0);
}

void event_mech_repairpart(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    int loc, pos;
    char buf[MBUF_SIZE];

    UNPACK_LOCPOS(earg, loc, pos);
    mech_RepairPart(mech, loc, pos);
    ArmorStringFromIndex(loc, buf, MechType(mech), MechMove(mech));
    do {
	int i = 0;

	if (Destroyed(mech))
	    i = 1;
	MechStatus(mech) &= ~DESTROYED;
	mech_notify(mech, MECHALL, tprintf("%s on %s has been repaired.",
		pos_part_name(mech, loc, pos), buf));
	if (i)
	    MechStatus(mech) |= DESTROYED;
    } while (0);
}

void event_mech_reload(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    int earg = (int) (e->data2) % PLAYERPOS;
    int loc, pos, extra;
    char buf[MBUF_SIZE];

    UNPACK_LOCPOS_E(earg, loc, pos, extra);
    if (extra) {
	SetPartData(mech, loc, pos, 0);
	if (extra > 1)
	    AddPartsM(mech, FindAmmoType(mech, loc, pos),
		GetPartBrand(mech, loc, pos), 1);
    } else
	mech_FillPartAmmo(mech, loc, pos);
    ArmorStringFromIndex(loc, buf, MechType(mech), MechMove(mech));
    do {
	int i = 0;

	if (Destroyed(mech))
	    i = 1;
	MechStatus(mech) &= ~DESTROYED;
	mech_notify(mech, MECHALL, tprintf("%s on %s has been %sloaded.",
		pos_part_name(mech, loc, pos), buf, extra ? "un" : "re"));
	if (i)
	    MechStatus(mech) |= DESTROYED;
    } while (0);
}

void event_mech_mountbomb(EVENT * e)
{

/*    MECH *mech = (MECH *) e->data; */

/*    int earg = (int) (e->data2) % PLAYERPOS; */
}

void event_mech_umountbomb(EVENT * e)
{

/*    MECH *mech = (MECH *) e->data; */

/*    int earg = (int) (e->data2) % PLAYERPOS; */
}
