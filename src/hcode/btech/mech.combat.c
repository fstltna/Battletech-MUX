
/*
 * $Id: mech.combat.c 1.17 04/05/11 16:04:14+02:00 thomas@centurion.xs4all.nl $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1997-2002 Markus Stenberg
 *  Copyright (c) 1998-2002 Thomas Wouters
 *  Copyright (c) 2000-2002 Cord Awtry
 *       All rights reserved
 *
 * Created: Thu Feb 13 21:19:23 1997 fingon
 * Last modified: Mon Jul 20 00:35:23 1998 fingon
 *
 * Last modified: Mon Sep 28 08:56:46 1998 fingon
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/file.h>

#include "mech.h"
#include "btmacros.h"
#include "mech.events.h"
#include "create.h"
#include "failures.h"
#include "mech.combat.h"
#include "mech.partnames.h"
#include "p.mech.ice.h"
#include "p.pcombat.h"
#include "p.mech.utils.h"
#include "p.mech.los.h"
#include "p.mech.build.h"
#include "p.map.obj.h"
#include "p.mech.combat.h"
#include "p.btechstats.h"
#include "p.mech.update.h"
#include "p.bsuit.h"
#include "p.artillery.h"
#include "p.mech.fire.h"
#include "p.mech.c3.h"
#include "p.mech.c3i.h"
#include "p.mech.c3.misc.h"
#include "p.mine.h"
#include "p.mech.hitloc.h"
#include "p.eject.h"
#include "p.crit.h"
#include "p.mech.ood.h"
#include "p.map.conditions.h"
#include "p.luck.h"
#include "p.template.h"
#include "p.mech.pickup.h"
#include "p.event.h"
#include "p.mech.tag.h"
#include "p.mech.ecm.h"
#include "mech.ecm.h"
#include "p.mech.enhanced.criticals.h"
#include "p.mech.spot.h"
#include "p.mech.combat.misc.h"
#include "p.mech.combat.missile.h"
#include "p.mech.bth.h"
#include "p.mech.damage.h"

extern int arc_override;
extern dbref pilot_override;

/*
Optional firing modes:
Autocannons:
Rapid-Fire:
std and light ACs only
fires like ultra
fails on roll of 2-4
failure results in 1 round in chamber exploding
explosion does not cause MW damage
MGs
Rapid-Fire
once set can't unset (lame?)
roll 1d6
result is heat generated
result is damage inflicted
ammo == damage * 3
LRMs
Hotloading
no min range
roll 3d6 for number of missiles hit and take lowest two rolls
can not un-hotload
critical hit to launcher destroys launcher and does damage equal to one flight of missiles
roll 2d6. On roll of 2-5 launcher explosion triggers ammo boom of launcher's ammo that's in the
same loc as the launcher.
PPC
Disengage field inhibitor
removes min range
roll 2d6 for feed back check and refer to chart below.
If failure, mech takes 10 points of internal damage to loc of PPC
Target distance Avoid feedback on:
1 10+
2 6+
3 3+
*/
void mech_target(dbref player, void *data, char *buffer)
{
    MECH *mech = (MECH *) data;
    MECH *target;
    char *args[5];
    int argc;
    char section[50];
    char type, move, index;

    cch(MECH_USUALO);
    argc = mech_parseattributes(buffer, args, 5);
    DOCHECK(argc != 1, "Invalid number of arguments to function!");
    if (!strcmp(args[0], "-")) {
	MechAim(mech) = NUM_SECTIONS;
	notify(player, "Targetting disabled.");
	return;
    }
    DOCHECK(MechTarget(mech) < 0 ||
	!(target =
	    FindObjectsData(MechTarget(mech))),
	"Error: You need to be locked at something to target it's part!");
    type = MechType(target);
    move = MechMove(target);
    DOCHECK((index =
	    ArmorSectionFromString(type, move, args[0])) < 0,
	"Invalid location!");
    MechAim(mech) = index;
    MechAimType(mech) = type;
    ArmorStringFromIndex(index, section, type, move);
    notify(player, tprintf("%s targetted.", section));
}

/* Varying messages based on the distance to foe, and size of your vehicle
   vs size of the guy targeting you: */

/*-20 (or less), -15 to 15, and 20+ ton difference (targetertons - yourtons)*/

/*Distance: <9, <20, rest */

/* Idea: Tonseverity + 3 * distseverity */
char *ss_messages[] = {
    "You feel you'll have your hands full before too long..",
    "You have a bad feeling about this..",
    "You feel a homicidal maniac is about to pounce you!",

    "You think something is amiss..",
    "You have a slightly bad feeling about this..",
    "You think someone thinks ill of you..",

    "Something makes you somewhat feel uneasy..",
    "Something makes you definitely feel uneasy..",
    "Something makes you feel out of your element.."
};

#define SSDistMod(r) ((r < 9) ? 0 : ((r < 20) ? 1 : 2))
#define SSTonMod(d)  ((d <= -20) ? 0 : (d >= 20) ? 2 : 1)

static void mech_ss_event(EVENT * ev)
{
    MECH *mech = (MECH *) ev->data;
    int i = (int) ev->data2;

    if (Uncon(mech))
	return;
    if (!RGotPilot(mech))
	return;
    mech_notify(mech, MECHPILOT, ss_messages[BOUNDED(0, i, 8)]);
}

void sixth_sense_check(MECH * mech, MECH * target)
{
    float r;
    int d;

    if (!(MechSpecials(target) & SS_ABILITY) ||
	MechIsObservator(mech))
	return;
    if (Destroyed(target))
	return;
    if (NRoll2(mech, target) > 8)
	return;
    r = FaMechRange(mech, target);
    d = (MechRTonsV(mech) - MechRTonsV(target)) / 1024;
    MECHEVENT(target, EVENT_SS, mech_ss_event, Number(1, 3),
	((3 * (SSDistMod(r))) + (SSTonMod(d))));
}

void mech_settarget(dbref player, void *data, char *buffer)
{
    MECH *mech = (MECH *) data, *target;
    MAP *mech_map;
    char *args[5];
    char targetID[2];
    int argc;
    int LOS = 1;
    int newx, newy;
    dbref targetref;
    int mode;

    cch(MECH_USUALO);

    argc = mech_parseattributes(buffer, args, 5);
    switch (argc) {
    case 1:
	mech_map = getMap(mech->mapindex);
	if (args[0][0] == '-') {
	    MechTarget(mech) = -1;
	    MechTargX(mech) = -1;
	    MechTargY(mech) = -1;
	    mech_notify(mech, MECHALL, "All locks cleared.");
	    StopLock(mech);
	    if (MechSpotter(mech) == mech->mynum)
		ClearFireAdjustments(mech_map, mech->mynum);
	    return;
	}
	targetID[0] = args[0][0];
	targetID[1] = args[0][1];
	targetref = FindTargetDBREFFromMapNumber(mech, targetID);
	target = getMech(targetref);
	if (target)
	    LOS =
		InLineOfSight(mech, target, MechX(target), MechY(target),
		FlMechRange(mech_map, mech, target));
	else
	    targetref = -1;
	DOCHECK(targetref == -1 ||
	    !LOS, "That is not a valid targetID. Try again.");

	if (MechSwarmTarget(mech) > 0) {
	    if (MechSwarmTarget(mech) != target->mynum) {
		mech_notify(mech, MECHALL,
		    "You're a bit to busy holding on for dear life to lock a target!");
		return;
	    }
	}

	mech_notify(mech, MECHALL, tprintf("Target set to %s.",
		GetMechToMechID(mech, target)));
	StopLock(mech);
	MechTarget(mech) = targetref;
	MechStatus(mech) |= LOCK_TARGET;
	sixth_sense_check(mech, target);
#if LOCK_TICK > 0
	if (!arc_override)
	    MECHEVENT(mech, EVENT_LOCK, mech_lock_event, LOCK_TICK, 0);
#endif
	break;
    case 2:
	/* Targetted a square */
	if (MechSwarmTarget(mech) > 0) {
	    mech_notify(mech, MECHALL,
		"You're a bit to busy holding on for dear life to lock a target!");
	    return;
	}

	mech_map = getMap(mech->mapindex);
	newx = atoi(args[0]);
	newy = atoi(args[1]);
	ValidCoord(mech_map, newx, newy);
	MechTarget(mech) = -1;
	MechTargX(mech) = newx;
	MechTargY(mech) = newy;
	MechFireAdjustment(mech) = 0;
	if (MechSpotter(mech) == mech->mynum)
	    ClearFireAdjustments(mech_map, mech->mynum);
	MechTargZ(mech) = Elevation(mech_map, newx, newy);
	notify(player, tprintf("Target coordinates set at (X,Y) %d, %d",
		newx, newy));
	StopLock(mech);
	MechStatus(mech) |= LOCK_TARGET;
#if LOCK_TICK > 0
	if (!arc_override)
	    MECHEVENT(mech, EVENT_LOCK, mech_lock_event, LOCK_TICK, 0);
#endif
	break;
    case 3:
	/* Targetted a square w/ special mode (hex / building) */
	if (MechSwarmTarget(mech) > 0) {
	    mech_notify(mech, MECHALL,
		"You're a bit to busy holding on for dear life to lock a target!");
	    return;
	}

	DOCHECK(strlen(args[2]) > 1, "Invalid lock mode!");
	switch (toupper(args[2][0])) {
	case 'B':
	    mode = LOCK_BUILDING;
	    break;
	case 'I':
	    mode = LOCK_HEX_IGN;
	    break;
	case 'C':
	    mode = LOCK_HEX_CLR;
	    break;
	case 'H':
	    mode = LOCK_HEX;
	    break;
	default:
	    notify(player, "Invalid mode selected!");
	    return;
	}
	mech_map = getMap(mech->mapindex);
	newx = atoi(args[0]);
	newy = atoi(args[1]);
	ValidCoord(mech_map, newx, newy);
	MechTarget(mech) = -1;
	MechTargX(mech) = newx;
	MechTargY(mech) = newy;
	MechFireAdjustment(mech) = 0;
	if (MechSpotter(mech) == mech->mynum)
	    ClearFireAdjustments(mech_map, mech->mynum);
	MechTargZ(mech) = Elevation(mech_map, newx, newy);
	switch (mode) {
	case LOCK_HEX:
	    notify(player,
		tprintf("Target coordinates set to hex at (X,Y) %d, %d",
		    newx, newy));
	    break;
	case LOCK_HEX_CLR:
	    notify(player,
		tprintf
		("Target coordinates set to clearing hex at (X,Y) %d, %d",
		    newx, newy));
	    break;
	case LOCK_HEX_IGN:
	    notify(player,
		tprintf
		("Target coordinates set to igniting hex at (X,Y) %d, %d",
		    newx, newy));
	    break;
	default:
	    notify(player,
		tprintf
		("Target coordinates set to building at (X,Y) %d, %d",
		    newx, newy));
	    break;
	}

	StopLock(mech);
	MechStatus(mech) |= mode;
#if LOCK_TICK > 0
	if (!arc_override)
	    MECHEVENT(mech, EVENT_LOCK, mech_lock_event, LOCK_TICK, 0);
#endif
    }
}

/*
 * Fire weapon command handler
 */
void mech_fireweapon(dbref player, void *data, char *buffer)
{
    MECH *mech = (MECH *) data;
    MAP *mech_map;
    char *args[5];
    int argc;
    int weapnum;

    mech_map = getMap(mech->mapindex);
    cch(MECH_USUALO);
    argc = mech_parseattributes(buffer, args, 5);
    DOCHECK(argc < 1, "Not enough arguments to the function");
    weapnum = atoi(args[0]);
    FireWeaponNumber(player, mech, mech_map, weapnum, argc, args, 0);
}

#define ARCCHECK(mech,ex,ey,sec,crit,msg) \
if (AeroUnusableArcs(mech)) \
{ int ar; ar = InWeaponArc(mech, ex, ey); \
DOCHECK0((!arc_override && (AeroUnusableArcs(mech) & ar)) || \
(arc_override && !(arc_override & ar)), \
"That arc's weapons aren't under your control!"); \
}; \
DOCHECK0(!IsInWeaponArc (mech, ex, ey, sec, crit), \
msg);

/*
 * Main weapon firing routine
 */
int FireWeaponNumber(dbref player,
    MECH * mech,
    MAP * mech_map, int weapnum, int argc, char **args, int sight)
{
    int weaptype;
    dbref target;
    char targetID[2];
    int mapx = 0, mapy = 0, LOS = 0;
    MECH *tempMech = NULL;
    int section, critical;
    float range = 0;
    float enemyX, enemyY, enemyZ;
    int ishex = 0;
    int wcDeadLegs = 0;

    if (!sight) {
	MechCritStatus(mech) &= ~HIDDEN;
	StopHiding(mech);
    }
    DOCHECK0(weapnum < 0,
	"The weapons system chirps: 'Illegal Weapon Number!'");
    weaptype =
	FindWeaponNumberOnMech_Advanced(mech, weapnum, &section, &critical,
	sight);
    DOCHECK0(weaptype == -1,
	"The weapons system chirps: 'Illegal Weapon Number!'");
    if (!sight) {
	DOCHECK0(PartTempNuke(mech, section, critical),
	    "The weapons system chirps: 'That Weapon is still unusable - please stand by.'");
	DOCHECK0(weaptype == -3,
	    "The weapon system chirps: 'That weapon is still reloading!'");
	DOCHECK0(weaptype == -4,
	    "The weapon system chirps: 'That weapon is still recharging!'");
	DOCHECK0(weaptype == -5,
	    "The limb holding that weapon is still retracting from an attack!");

	DOCHECK0(MechSections(mech)[section].specials & CARRYING_CLUB,
	    "You're carrying a club in that arm.");

	if (Fallen(mech) && MechType(mech) == CLASS_MECH) {
	    /* if a quad has 3 of 4 legs dead, it can't fire at all while prone */
	    wcDeadLegs = CountDestroyedLegs(mech);
	    if (MechIsQuad(mech))
		DOCHECK0(wcDeadLegs > 2,
		    "Quads need atleast 3 legs to fire while prone.");

	    /* quads with all 4 legs can fire all weapons while prone. They do not need to prop. */
	    if (!MechIsQuad(mech) || (MechIsQuad(mech) && wcDeadLegs > 0)) {
		DOCHECK0(section == RLEG || section == LLEG,
		    "You cannot fire leg mounted weapons when prone.");
		switch (section) {
		case RARM:
		    DOCHECK0(SectHasBusyWeap(mech, LARM) ||
			MechSections(mech)[LARM].recycle ||
			SectIsDestroyed(mech, LARM),
			"You currently can't use your Left Arm to prop yourself up.");
		    break;
		case LARM:
		    DOCHECK0(SectHasBusyWeap(mech, RARM) ||
			MechSections(mech)[RARM].recycle ||
			SectIsDestroyed(mech, RARM),
			"Your currently can't use your Right Arm to prop yourself up.");
		    break;
		default:
		    DOCHECK0((SectHasBusyWeap(mech, RARM) ||
			    MechSections(mech)[RARM].recycle ||
			    SectIsDestroyed(mech, RARM))
			&& (SectHasBusyWeap(mech, LARM) ||
			    MechSections(mech)[LARM].recycle ||
			    SectIsDestroyed(mech, LARM)),
			"You currently don't have any arms to spare to prop yourself up.");
		}
	    }
	}
    }

    if (CountSwarmers(mech) > 0) {
	DOCHECK0(((section == CTORSO) || (section == RTORSO) ||
		(section == LTORSO)),
	    "You can not fire torso mounted weapons while you have battlesuits on you!");
    }

    DOCHECK0((MechDugIn(mech)) && section != TURRET,
	"Only turret weapons are available while in cover.");
    DOCHECK0(IsAMS(weaptype), "That weapon is defensive only!");
    DOCHECK0(weaptype == -2 ||
	(PartTempNuke(mech, section, critical) == FAIL_DESTROYED),
	"The weapons system chirps: 'That Weapon has been destroyed!'");
    DOCHECK0(argc > 3, "Invalid number of arguments!");
    if ((MechWeapons[weaptype].special & IDF) && MechSpotter(mech) != -1 &&
	MechTarget(mech) == -1 && MechTargY(mech) == -1 &&
	MechTargX(mech) == -1)
	if (FireSpot(player, mech, mech_map, weapnum, weaptype, sight,
		section, critical))
	    return 1;
    switch (argc) {
    case 1:
	/* Fire at the default target */
	DOCHECK0(!FindTargetXY(mech, &enemyX, &enemyY, &enemyZ),
	    "You do not have a default target set!");

	if (MechTarget(mech) != -1) {
	    tempMech = getMech(MechTarget(mech));
	    DOCHECK0(!tempMech, "Error in FireWeaponNumber routine");
	    mapx = MechX(tempMech);
	    mapy = MechY(tempMech);
	    range = FaMechRange(mech, tempMech);
	    LOS = LOS_NB(mech, tempMech, mapx, mapy, range);
	    if (!(MechWeapons[weaptype].special & IDF)) {
		DOCHECK0(!LOS,
		    "That target is not in your line of sight!");
            } else if (MapIsUnderground(mech_map)) {
	        DOCHECK0(!LOS,
	            "That target is not in your direct line of sight, and "
	            "you cannot fire your IDF weapons underground!");
	    }
	} else
	    /* default target is a hex */
	{
	    ishex = 1;
	    if (!sight && !IsArtillery(weaptype) && MechLockFire(mech))
		/* look for enemies in the default hex cause they may have moved */
		if ((tempMech =
			find_mech_in_hex(mech, mech_map, MechTargX(mech),
			    MechTargY(mech), 0))) {
		    enemyX = MechFX(tempMech);
		    enemyY = MechFY(tempMech);
		    enemyZ = MechFZ(tempMech);
		    mapx = MechX(tempMech);
		    mapy = MechY(tempMech);
		}
	    if (!tempMech) {
		mapx = MechTargX(mech);
		mapy = MechTargY(mech);
		MechTargZ(mech) = Elevation(mech_map, mapx, mapy);
		enemyZ = ZSCALE * MechTargZ(mech);
		MapCoordToRealCoord(mapx, mapy, &enemyX, &enemyY);
	    }
	    /* don't check LOS for missile weapons firing at hex number */
	    range =
		FindRange(MechFX(mech), MechFY(mech), MechFZ(mech), enemyX,
		enemyY, enemyZ);
	    LOS = LOS_NB(mech, tempMech, mapx, mapy, range);
	    if (!(IsArtillery(weaptype) ||
		    (MechWeapons[weaptype].special & IDF))) {
		DOCHECK0(!LOS,
		    "That hex target is not in your line of sight!");
	    } else if (MapIsUnderground(mech_map)) {
	        DOCHECK0(!LOS,
	            "That target is not in your direct line of sight, and "
	            "you cannot fire your IDF weapons underground!");
	    }
	}

	if (MechType(mech) != CLASS_BSUIT) {
	    ARCCHECK(mech, enemyX, enemyY, section, critical,
		"Default target is not in your weapons arc!");
	}

	break;
    case 2:
	/* Fire at the numbered target */
	targetID[0] = args[1][0];
	targetID[1] = args[1][1];
	target = FindTargetDBREFFromMapNumber(mech, targetID);
	DOCHECK0(target == -1, "That target is not in your line of sight!");
	tempMech = getMech(target);
	DOCHECK0(!tempMech, "Error in FireWeaponNumber routine!");
	enemyX = MechFX(tempMech);
	enemyY = MechFY(tempMech);
	enemyZ = MechFZ(tempMech);
	mapx = MechX(tempMech);
	mapy = MechY(tempMech);

	range =
	    FindRange(MechFX(mech), MechFY(mech), MechFZ(mech), enemyX,
	    enemyY, enemyZ);
	LOS =
	    LOS_NB(mech, tempMech, MechX(tempMech), MechY(tempMech),
	    range);
	DOCHECK0(!LOS, "That target is not in your line of sight!");

	if (MechType(mech) != CLASS_BSUIT) {
	    ARCCHECK(mech, enemyX, enemyY, section, critical,
		"That target is not in your weapons arc!");
	}
	break;
    case 3:
	/* Fire at the Map X Y */
	mapx = atoi(args[1]);
	mapy = atoi(args[2]);
	ishex = 1;
	DOCHECK0(mapx < 0 || mapx >= mech_map->map_width || mapy < 0 ||
	    mapy >= mech_map->map_height, "Map coordinates out of range!");
	if (!sight && !IsArtillery(weaptype))
	    /* look for enemies in that hex... */
	    if ((tempMech =
		    find_mech_in_hex(mech, mech_map, MechTargX(mech),
			MechTargY(mech), 0))) {
		enemyX = MechFX(tempMech);
		enemyY = MechFY(tempMech);
		enemyZ = MechFZ(tempMech);
	    }
	if (!tempMech) {
	    MapCoordToRealCoord(mapx, mapy, &enemyX, &enemyY);
	    MechTargZ(mech) = Elevation(mech_map, mapx, mapy);
	    enemyZ = ZSCALE * MechTargZ(mech);
	}

	if (MechType(mech) != CLASS_BSUIT) {
	    ARCCHECK(mech, enemyX, enemyY, section, critical,
		"That hex target is not in your weapons arc!");
	}

	/* Don't check LOS for missile weapons */
	range =
	    FindRange(MechFX(mech), MechFY(mech), MechFZ(mech), enemyX,
	    enemyY, enemyZ);
	LOS = LOS_NB(mech, tempMech, mapx, mapy, range);
	if (!IsArtillery(weaptype))
	    DOCHECK0(!LOS,
		"That hex target is not in your line of sight!");
    }
    if (tempMech) {
	DOCHECK0(IsArtillery(weaptype),
	    "You can only target hexes with this kind of artillery.");
	DOCHECK0(MechSwarmTarget(tempMech) == mech->mynum,
	    "You are unable to use your weapons against a 'swarmer!");
	DOCHECK0(StealthArmorActive(tempMech) &&
	    ((MechTarget(mech) != tempMech->mynum) || Locking(mech)),
	    "You need a stable lock to fire on that target!");
	DOCHECK0(MechTeam(tempMech) == MechTeam(mech) &&
		 MechNoFriendlyFire(mech),
		 "You can't fire on a teammate with FFSafeties on!");
	DOCHECK0(MechType(tempMech) == CLASS_MW &&
	    MechType(mech) != CLASS_MW &&
	    !MechPKiller(mech),
	    "That's a living, breathing person! Switch off the safety first, if you really want to assassinate the target.");
    }

    FireWeapon(mech, mech_map, tempMech, LOS, weaptype, weapnum, section,
	critical, enemyX, enemyY, mapx, mapy, range, 1000, sight, ishex);
    return (1);
}

int weapon_failure_stuff(MECH * mech,
    int *weapnum,
    int *weapindx,
    int *section,
    int *critical,
    int *ammoLoc,
    int *ammoCrit,
    int *ammoLoc1,
    int *ammoCrit1,
    int *modifier,
    int *type, float range, int *range_ok, int wGattlingShots)
{
    CheckWeaponFailed(mech, *weapnum, *weapindx, *section, *critical,
	modifier, type);
    if (*type == POWER_SPIKE)
	return 1;
    if (*type == WEAPON_JAMMED || *type == WEAPON_DUD) {
	/* Just decrement ammunition */
	decrement_ammunition(mech, *weapindx, *section, *critical,
	    *ammoLoc, *ammoCrit, *ammoLoc1, *ammoCrit1, wGattlingShots);
	return 1;
    }
    if (*type == RANGE)
	if ((EGunRangeWithCheck(mech, *section,
		    *weapindx) - *modifier) < range) {
	    mech_notify(mech, MECHALL,
		"Due to weapons failure your shot fails short of its target!");
	    *range_ok = 0;
	}
    return 0;
}

void sendC3TrackEmit(MECH * mech, dbref c3Ref, MECH * c3Mech)
{
    if (c3Mech && (c3Mech->mynum != mech->mynum)) {
	mech_notify(mech, MECHALL,
	    tprintf("Using range data from %s [%s]",
		silly_atr_get(c3Mech->mynum,
		    A_MECHNAME), MechIDS(c3Mech, 1)));
    }
}

void FireWeapon(MECH * mech,
    MAP * mech_map,
    MECH * target,
    int LOS,
    int weapindx,
    int weapnum,
    int section,
    int critical,
    float enemyX,
    float enemyY,
    int mapx,
    int mapy, float range, int indirectFire, int sight, int ishex)
{
    MECH *altTarget;
    int baseToHit;
    int ammoLoc;
    int ammoCrit;
    int ammoLoc1;
    int ammoCrit1;
    int roll;
    int r1, r2, r3, rtmp;
    int type = -1, modifier;
    int isarty = (IsArtillery(weapindx));
    int range_ok = 1;
    int wGattlingShots = 0;	/* If we're a gattling MG, then we need to figure out how many shots */
    char buf[SBUF_SIZE];
    char buf3[SBUF_SIZE];
    char buf2[LBUF_SIZE];
    int wRACHeat = 0;
    int wHGRPSkillMod = 0;
    int tIsSwarmAttack = 0;
    dbref c3Ref = -1;
    MECH *c3Mech = NULL;
    int firstCrit = 0;

    DOCHECKMA((SectionUnderwater(mech, section) &&
	    (MechWeapons[weapindx].shortrange_water <= 0)),
	"This weapon may not be fired underwater.");
    DOCHECKMA(CrewStunned(mech), "You are too stunned to fire a weapon!");
    DOCHECKMA(UnjammingTurret(mech),
	"You are too busy unjamming your turret!");
    DOCHECKMA(UnJammingAmmo(mech), "You are too busy unjamming a weapon!");
    DOCHECKMA(RemovingPods(mech), "You are too busy removing iNARC pods!");

    DOCHECKMA((MechSwarmTarget(mech) > 0) && ((!target) ||
	    (MechSwarmTarget(mech) != target->mynum)),
	"You're too busy holding on for dear life!");

    if (MechSwarmTarget(mech) > 0) {
	if (target && (MechSwarmTarget(mech) == target->mynum))
	    tIsSwarmAttack = 1;
    }

    /*
       * Gattling MGs do d6 damage (all as one hit), causing the same in heat
       * and using 3 * damage in ammo.
     */
    if (GetPartFireMode(mech, section, critical) & GATTLING_MODE)
	wGattlingShots = Number(1, 6);

    /* Find and check Ammunition */
    if (!sight)
	if (!FindAndCheckAmmo(mech, weapindx, section, critical, &ammoLoc,
		&ammoCrit, &ammoLoc1, &ammoCrit1, &wGattlingShots))
	    return;

			/****************************************
			* START: Calc BTH and Roll
		****************************************/
    if (!isarty) {
	baseToHit =
	    FindNormalBTH(mech, mech_map, section, critical, weapindx,
	    range, target, indirectFire, &c3Ref);

	if (c3Ref) {
	    c3Mech = getMech(c3Ref);

	    if (c3Mech && ((MechTeam(c3Mech) != MechTeam(mech)) ||
		    (c3Ref == mech->mynum))) {
		c3Mech = NULL;
	    }
	}

    } else
	baseToHit = FindArtilleryBTH(mech, section, weapindx, !LOS, range);

    /* If it's a swarm attack, make the BTH 0 'cause they always hit */
    if (tIsSwarmAttack)
	baseToHit = 0;

    /* Mod the roll for DFMs and ELRMS */
    if ((MechWeapons[weapindx].special & DFM) ||
	((MechWeapons[weapindx].special & ELRM) &&
	    range < MechWeapons[weapindx].min)) {
	r1 = Number(1, 6);
	r2 = Number(1, 6);
	r3 = Number(1, 6);
	/* Sort 'em to ascending order */
	if (r1 > r2)
	    Swap(r1, r2);
	if (r2 > r3)
	    Swap(r2, r3);
	roll = r1 + r2;
    } else {
	if (target)
	    roll = PRoll2(mech, target);
	else
	    roll = PRoll(mech);
    }
    buf[0] = 0;
    if (LOS)
	sprintf(buf, "Roll: %d ", roll);

			/****************************************
			* END: Calc BTH and Roll
		****************************************/

		/****************************************
		* START: Do all the necessary emits for sighting and firing
		****************************************/
    if (target && !ishex) {
	range = FaMechRange(mech, target);
	strcpy(buf2, "");
	if (MechAim(mech) != NUM_SECTIONS &&
	    MechAimType(mech) == MechType(target) &&
	    !IsMissile(weapindx)) {
	    ArmorStringFromIndex(MechAim(mech), buf3, MechType(target),
		MechMove(target));
	    sprintf(buf2, "'s %s", buf3);
	}

	if (sight) {
	    DOCHECKMA(baseToHit >= 900,
		tprintf("You aim %s at %s%s - Out of range.",
		    &MechWeapons[weapindx].name[3],
		    GetMechToMechID(mech, target), buf2));

	    sendC3TrackEmit(mech, c3Ref, c3Mech);

	    mech_notify(mech, MECHALL,
		tprintf("You aim %s at %s%s - BTH: %d %s",
		    &MechWeapons[weapindx].name[3],
		    GetMechToMechID(mech, target), buf2,
		    baseToHit,
		    MechStatus(target) & PARTIAL_COVER ?
		    "(Partial cover)" : ""));
	    return;
	}
	if (baseToHit > 12) {
	    DOCHECKMA(baseToHit >= 900,
		tprintf("Fire %s at %s%s - Out of range.",
		    &MechWeapons[weapindx].name[3],
		    GetMechToMechID(mech, target), buf2));
	    mech_notify(mech, MECHALL,
		tprintf
		("Fire %s at %s%s - BTH: %d  Roll: Impossible! %s",
		    &MechWeapons[weapindx].name[3],
		    GetMechToMechID(mech, target), buf2, baseToHit,
		    MechStatus(target) & PARTIAL_COVER ?
		    "(Partial cover)" : ""));
	    return;
	}
    } else {
	/* Hex target sight info */
	if (sight) {
	    if (baseToHit > 900)
		mech_notify(mech, MECHPILOT,
		    tprintf
		    ("You aim your %s at (%d,%d) - Out of Range.",
			&MechWeapons[weapindx].name[3], mapx, mapy));
	    else {
		sendC3TrackEmit(mech, c3Ref, c3Mech);

		mech_notify(mech, MECHPILOT,
		    tprintf("You aim your %s at (%d,%d) - BTH: %d",
			&MechWeapons[weapindx].name[3], mapx,
			mapy, baseToHit));
	    }

	    return;
	}
	if (!isarty && baseToHit > 12) {
	    mech_notify(mech, MECHALL,
		tprintf
		("Fire %s at (%d,%d) - BTH: %d  Roll: Impossible!",
		    &MechWeapons[weapindx].name[3], mapx, mapy,
		    baseToHit));
	    return;
	}
    }
    if (target && !ishex) {
	sendC3TrackEmit(mech, c3Ref, c3Mech);

	mech_notify(mech, MECHALL,
	    tprintf("You fire %s at %s%s - BTH: %d  %s%s",
		&MechWeapons[weapindx].name[3],
		GetMechToMechID(mech, target), buf2, baseToHit,
		buf,
		MechStatus(target) & PARTIAL_COVER ?
		"(Partial cover)" : ""));
	SendXP(tprintf
	    ("#%i attacks #%i (weapon) (%i/%i)", mech->mynum,
		target->mynum, baseToHit, roll));
    } else {
	sendC3TrackEmit(mech, c3Ref, c3Mech);

	mech_notify(mech, MECHALL,
	    tprintf("You fire %s %s (%d,%d) - BTH: %d  %s",
		&MechWeapons[weapindx].name[3],
		hex_target_id(mech), mapx, mapy, baseToHit, buf));
	SendXP(tprintf
	    ("#%i attacks %d,%d (%s) (weapon) (%i/%i)",
		mech->mynum, mapx, mapy, short_hextarget(mech),
		baseToHit, roll));
    }

		/****************************************
		* END: Do all the necessary emits for sighting and firing
		****************************************/

    /* Check for weapon failures */
    if (weapon_failure_stuff(mech, &weapnum, &weapindx, &section,
	    &critical, &ammoLoc, &ammoCrit, &ammoLoc1,
	    &ammoCrit1, &modifier, &type, range,
	    &range_ok, wGattlingShots))
	return;

    /* See if our streaks work */
    if (MechWeapons[weapindx].special & STREAK) {
	if (target && (AngelECMDisturbed(mech) || AngelECMProtected(target)))
	    mech_notify(mech, MECHALL,
		"The ECM confuses your streak homing system!");
	else if (roll < baseToHit) {
	    SetRecyclePart(mech, section, critical,
		MechWeapons[weapindx].vrt);
	    mech_notify(mech, MECHALL, "Your streak fails to lock on.");
	    return;
	}
    }

    /* Check for RFAC explosion/jams */
    if (GetPartFireMode(mech, section, critical) & RFAC_MODE) {
	if (roll == 2) {
	    mech_notify(mech, MECHALL,
		tprintf
		("%%ch%%crA catastrophic misload on your %s destroys it and causes an internal explosion!%%cn",
		    &(MechWeapons[weapindx].name[3])));
	    firstCrit =
		FindFirstWeaponCrit(mech, section, -1, 0,
		I2Weapon(weapindx), GetWeaponCrits(mech, weapindx));
	    DestroyWeapon(mech, section, I2Weapon(weapindx), firstCrit,
		GetWeaponCrits(mech, weapindx), GetWeaponCrits(mech,
		    weapindx));
	    MechLOSBroadcast(mech, "shudders from an internal explosion!");
	    DamageMech(mech, mech, 0, -1, section, 0, 0, 0,
		MechWeapons[weapindx].damage, -1, 0, -1, 0, 1);
	    decrement_ammunition(mech, weapindx, section, critical,
		ammoLoc, ammoCrit, ammoLoc1, ammoCrit1, wGattlingShots);
	    return;
	} else if (roll < 5) {
	    mech_notify(mech, MECHALL,
		tprintf
		("%%ch%%crThe ammo loader mechanism jams on your %s!%%cn",
		    &(MechWeapons[weapindx].name[3])));
	    SetPartTempNuke(mech, section, critical, FAIL_AMMOJAMMED);
	    return;
	}
    }

    /* Check for RAC explosion/jams */
    if (MechWeapons[weapindx].special & RAC) {
	if (((GetPartFireMode(mech, section, critical) & RAC_TWOSHOT_MODE)
		&& (roll == 2)) ||
	    ((GetPartFireMode(mech, section, critical) & RAC_FOURSHOT_MODE)
		&& (roll <= 3)) ||
	    ((GetPartFireMode(mech, section, critical) & RAC_SIXSHOT_MODE)
		&& (roll <= 4))) {
	    mech_notify(mech, MECHALL,
		tprintf
		("%%ch%%crThe ammo loader mechanism jams on your %s!%%cn",
		    &(MechWeapons[weapindx].name[3])));
	    SetPartTempNuke(mech, section, critical, FAIL_AMMOJAMMED);
	    return;
	}
    }

    /* Check for Ultra explosion/jams */
    if (GetPartFireMode(mech, section, critical) & ULTRA_MODE) {
	if (roll == 2) {
	    mech_notify(mech, MECHALL,
		tprintf
		("The loader jams on your %s, destroying it!",
		    &(MechWeapons[weapindx].name[3])));
	    firstCrit =
		FindFirstWeaponCrit(mech, section, -1, 0,
		I2Weapon(weapindx), GetWeaponCrits(mech, weapindx));
	    DestroyWeapon(mech, section, I2Weapon(weapindx), firstCrit,
		GetWeaponCrits(mech, weapindx), GetWeaponCrits(mech,
		    weapindx));
	    return;
	}
    }

    /* See if the sucker will explode from damage taken */
    if (canWeapExplodeFromDamage(mech, section, critical, roll)) {
	if (IsEnergy(weapindx)) {
	    mech_notify(mech, MECHALL,
		tprintf
		("%%ch%%crThe damaged charging crystal on your %s overloads!%%cn",
		    &(MechWeapons[weapindx].name[3])));
	} else {
	    mech_notify(mech, MECHALL,
		tprintf
		("%%ch%%crThe damaged ammo feed on your %s triggers an internal explosion!%%cn",
		    &(MechWeapons[weapindx].name[3])));
	    decrement_ammunition(mech, weapindx, section, critical,
		ammoLoc, ammoCrit, ammoLoc1, ammoCrit1, wGattlingShots);
	}

	MechLOSBroadcast(mech, "shudders from an internal explosion!");
	firstCrit =
	    FindFirstWeaponCrit(mech, section, -1, 0, I2Weapon(weapindx),
	    GetWeaponCrits(mech, weapindx));
	DestroyWeapon(mech, section, I2Weapon(weapindx), firstCrit,
	    GetWeaponCrits(mech, weapindx), GetWeaponCrits(mech,
		weapindx));
	DamageMech(mech, mech, 0, -1, section, 0, 0, 0,
	    MechWeapons[weapindx].damage, -1, 0, -1, 0, 1);

	return;
    }

    /* See if the sucker will jam from damage taken */
    if (canWeapJamFromDamage(mech, section, critical, roll)) {
	mech_notify(mech, MECHALL, tprintf
	    ("%%ch%%crThe ammo loader mechanism jams on your %s!%%cn",
		&(MechWeapons[weapindx].name[3])));
	SetPartTempNuke(mech, section, critical, FAIL_AMMOCRITJAMMED);

	return;
    }

    /* Trash our cocoon if we're OODing */
    if (OODing(mech)) {
	if (MechZ(mech) > MechElevation(mech)) {
	    if (MechJumpSpeed(mech) >= MP1) {
		mech_notify(mech, MECHALL,
		    "You initiate your jumpjets to compensate for the opened cocoon!");
		MechCocoon(mech) = -1;
	    } else {
		mech_notify(mech, MECHALL,
		    "Your action splits open the cocoon - have a nice fall!");
		MechLOSBroadcast(mech,
		    "starts plummeting down, as the cocoon opens!.");
		MechCocoon(mech) = 0;
		StopOOD(mech);
		MECHEVENT(mech, EVENT_FALL, mech_fall_event,
		    FALL_TICK, -1);
	    }
	}
    }

    if (!isarty)
	MechFireBroadcast(mech, ishex ? NULL : target, mapx, mapy,
	    mech_map, &MechWeapons[weapindx].name[3],
	    (roll >= baseToHit) && range_ok);

    /* Tell our target they were just shot at... */
    if (target) {
	if (InLineOfSight(target, mech, MechX(mech), MechY(mech), range))
	    mech_notify(target, MECHALL,
		tprintf("%s has fired a %s at you!",
		    GetMechToMechID(target, mech),
		    &MechWeapons[weapindx].name[3]));
	else
	    mech_notify(target, MECHALL,
		tprintf
		("Something has fired a %s at you from bearing %d!",
		    &MechWeapons[weapindx].name[3],
		    FindBearing(MechFX(target), MechFY(target),
			MechFX(mech), MechFY(mech))));
    }

    /* Time to decide if we've really hit them and wot to do */
    MechStatus(mech) |= FIRED;

    if (isarty) {
	artillery_shoot(mech, mapx, mapy, weapindx,
	    GetPartAmmoMode(mech, section, critical), baseToHit <= roll);
    } else if (range_ok) {
	if (IsMissile(weapindx)) {
	    HitTarget(mech, weapindx, section, critical, target, mapx,
		mapy, LOS, type, modifier, roll >= baseToHit &&
		range_ok, baseToHit, wGattlingShots, tIsSwarmAttack);
	} else {
	    if (roll >= baseToHit) {
		HitTarget(mech, weapindx, section, critical, target, mapx,
		    mapy, LOS, type, modifier, 1, baseToHit,
		    wGattlingShots, tIsSwarmAttack);
	    } else {
		int tTryClear = 1;

		if (target) {
		    if ((MechType(target) == CLASS_BSUIT) &&
			(MechSwarmTarget(target) > -1) &&
			(altTarget = getMech(MechSwarmTarget(target)))) {

			baseToHit =
			    FindNormalBTH(mech, mech_map, section,
			    critical, weapindx, range, altTarget,
			    indirectFire, &c3Ref);

			if (roll >= baseToHit) {
			    mech_notify(altTarget, MECHALL,
				"The shot hits you instead!");
			    MechLOSBroadcast(altTarget,
				"manages to get in the way of the shot instead!");
			    HitTarget(mech, weapindx, section, critical,
				altTarget, mapx, mapy, LOS, type, modifier,
				1, baseToHit, wGattlingShots,
				tIsSwarmAttack);

			    tTryClear = 0;
			} else {
			    if (Elevation(mech_map, MechX(target),
				    MechY(target)) < (MechZ(target) - 2))
				tTryClear = 0;
			}
		    }
		}

		if (tTryClear) {
		    int tempDamage =
			determineDamageFromHit(mech, section, critical,
			target, mapx,
			mapy, weapindx, wGattlingShots,
			MechWeapons[weapindx].damage,
			GetPartAmmoMode(mech, section, critical), type,
			modifier, 1);

		    possibly_ignite_or_clear(mech, weapindx,
			GetPartAmmoMode(mech, section, critical),
			tempDamage, mapx, mapy, 0);
		}
	    }
	}
    }

    /* Recycle the weapon */
    SetRecyclePart(mech, section, critical, MechWeapons[weapindx].vrt);

		/****************************************
		* START: Set the heat after firing
		****************************************/
    if (type == HEAT)
	MechWeapHeat(mech) += (float) modifier;

    if (GetPartFireMode(mech, section, critical) & GATTLING_MODE) {
	MechWeapHeat(mech) += wGattlingShots;
    } else if (MechWeapons[weapindx].special & RAC) {
	if (GetPartFireMode(mech, section, critical) & RAC_TWOSHOT_MODE)
	    wRACHeat = 2;
	else if (GetPartFireMode(mech, section,
		critical) & RAC_FOURSHOT_MODE)
	    wRACHeat = 4;
	else if (GetPartFireMode(mech, section,
		critical) & RAC_SIXSHOT_MODE)
	    wRACHeat = 6;
	else
	    wRACHeat = 1;

	MechWeapHeat(mech) +=
	    (float) (MechWeapons[weapindx].heat * wRACHeat);

	if (type == HEAT)
	    MechWeapHeat(mech) += (float) (modifier * wRACHeat);
    } else {
	MechWeapHeat(mech) += (float) MechWeapons[weapindx].heat;

	if (IsEnergy(weapindx))
	    MechWeapHeat(mech) +=
		(float) getCritAddedHeat(mech, section, critical);

	if ((GetPartFireMode(mech, section, critical) & ULTRA_MODE) ||
	    (GetPartFireMode(mech, section, critical) & RFAC_MODE)) {

	    if (type == HEAT)
		MechWeapHeat(mech) += (float) modifier;

	    MechWeapHeat(mech) += (float) MechWeapons[weapindx].heat;
	}
    }

		/****************************************
		* END: Set the heat after firing
		****************************************/

    /* Decrement Ammunition */
    decrement_ammunition(mech, weapindx, section, critical, ammoLoc,
	ammoCrit, ammoLoc1, ammoCrit1, wGattlingShots);

    /* Special for Heavy Gauss Rifles */
    if ((MechWeapons[weapindx].special & HVYGAUSS) &&
	(MechType(mech) == CLASS_MECH)) {
	if (abs(MechSpeed(mech)) > 0.0) {
	    mech_notify(mech, MECHALL,
		"You realize that moving while firing this weapon may not be a good idea afterall.");
	    if (MechTons(mech) <= 35)
		wHGRPSkillMod = 2;
	    else if (MechTons(mech) <= 55)
		wHGRPSkillMod = 1;
	    else if (MechTons(mech) <= 75)
		wHGRPSkillMod = 0;
	    else
		wHGRPSkillMod = -1;
	    if (!MadePilotSkillRoll(mech, wHGRPSkillMod)) {
		mech_notify(mech, MECHALL,
		    "The weapon's recoil knocks you to the ground!");
		MechLOSBroadcast(mech,
		    tprintf("topples over from the %s's recoil!",
			&MechWeapons[weapindx].name[3]));
		MechFalls(mech, 1, 0);
	    }
	}
    }
}

int determineDamageFromHit(MECH * mech,
    int wSection,
    int wCritSlot,
    MECH * hitMech,
    int hitX,
    int hitY,
    int weapindx,
    int wGattlingShots,
    int wBaseWeapDamage,
    int wAmmoMode, int type, int modifier, int isTempCalc)
{
    MAP *mech_map;
    float fRange = 0.0;
    int wWeapDamage = wBaseWeapDamage;
    int wClearDamage = 0;

    /* Find the range to our target */
    if (hitMech)
	fRange = FaMechRange(mech, hitMech);
    else {
        float fx, fy;
        MapCoordToRealCoord(hitX, hitY, &fx, &fy);
	fRange = FindHexRange(MechFX(mech), MechFY(mech), fx, fy);
    }

    /* If our Gattling shots are greater then 0, use that as the damage. */
    if (wGattlingShots > 0)
	wWeapDamage = wGattlingShots;

    /* If we're a heavy gauss rifle, damage gets altered by range. */
    if (MechWeapons[weapindx].special & HVYGAUSS) {
	if (fRange > MechWeapons[weapindx].medrange)
	    wWeapDamage = 10;
	else if (fRange > MechWeapons[weapindx].shortrange)
	    wWeapDamage = 20;
    }

    wWeapDamage -= getCritSubDamage(mech, wSection, wCritSlot);

    /* See if we're using flechette ammo */
    if (hitMech) {
	if (wAmmoMode & AC_FLECHETTE_MODE) {
	    if (MechType(hitMech) == CLASS_MW) {
		if (MechRTerrain(hitMech) == GRASSLAND)
		    wWeapDamage *= 4;
		else
		    wWeapDamage *= 2;
	    } else if (MechType(hitMech) != CLASS_BSUIT)
		wWeapDamage /= 2;
	}

	if (wAmmoMode & AC_INCENDIARY_MODE) {
	    if (MechType(hitMech) == CLASS_MW)
		wWeapDamage += 2;
	}
    }

    /* Check to see if we have an energy weapon and we're modding the damage based on range */
    if (mudconf.btech_moddamagewithrange && IsEnergy(weapindx)) {
	if (fRange <= 1.0)
	    wWeapDamage++;
	else {
	    if (SectionUnderwater(mech, wSection)) {
		if (fRange > MechWeapons[weapindx].longrange_water)
		    wWeapDamage = (wWeapDamage / 2);
		else if (fRange > MechWeapons[weapindx].medrange_water)
		    wWeapDamage--;
	    } else {
		if (fRange > MechWeapons[weapindx].longrange)
		    wWeapDamage = (wWeapDamage / 2);
		else if (fRange > MechWeapons[weapindx].medrange)
		    wWeapDamage--;
	    }
	}
    }

    /* Check to see if we're modding the damage based on woods cover */
    mech_map = getMap(mech->mapindex);

    /* If there was a damage type failure, mod the damage */
    if (type == DAMAGE)
	wWeapDamage -= modifier;

    if (hitMech && !isTempCalc) {
	if (mudconf.btech_moddamagewithwoods &&
	    IsForestHex(mech_map, MechX(hitMech), MechY(hitMech)) &&
	    ((MechZ(hitMech) - 2) <= Elevation(mech_map, MechX(hitMech),
		    MechY(hitMech)))) {
	    wClearDamage = wWeapDamage;

	    if (GetRTerrain(mech_map, MechX(hitMech),
		    MechY(hitMech)) == LIGHT_FOREST)
		wWeapDamage -= 2;
	    else if (GetRTerrain(mech_map, MechX(hitMech),
		    MechY(hitMech)) == HEAVY_FOREST)
		wWeapDamage -= 4;

	    mech_notify(mech, MECHALL,
		"The woods absorb some of your shot!");
	    mech_notify(hitMech, MECHALL,
		"The woods absorb some of the damage!");

	    possibly_ignite_or_clear(mech, weapindx, wAmmoMode,
		wClearDamage, MechX(hitMech), MechY(hitMech), 1);
	}
    }

    if (wWeapDamage <= 0)
	wWeapDamage = 1;

    return wWeapDamage;
}

void HitTarget(MECH * mech,
    int weapindx,
    int wSection,
    int wCritSlot,
    MECH * hitMech,
    int hitX,
    int hitY,
    int LOS,
    int type,
    int modifier,
    int reallyhit, int bth, int wGattlingShots, int tIsSwarmAttack)
{
    int isrear = 0, iscritical = 0;
    int hitloc = 0;
    int loop;
    int roll;
    int aim_hit = 0;
    int wBaseWeapDamage = MechWeapons[weapindx].damage;
    int wWeapDamage = 0;
    int num_missiles_hit;
    int wFireMode = GetPartFireMode(mech, wSection, wCritSlot);
    int wAmmoMode = GetPartAmmoMode(mech, wSection, wCritSlot);
    int tIsUltra = ((wFireMode & ULTRA_MODE) || (wFireMode & RFAC_MODE));
    int tIsRAC = (wFireMode & RAC_MODES);
    int tIsLBX = (wAmmoMode & LBX_MODE);
    int tIsSwarm = ((wAmmoMode & SWARM_MODE) || (wAmmoMode & SWARM1_MODE));
    char strMissileFakeName[30];
    int tFoundRACFake = 0;
    int tUsingTC = ((wFireMode & ON_TC) && !IsArtillery(weapindx) &&
	!IsMissile(weapindx) &&
	(!(MechCritStatus(mech) & TC_DESTROYED)) &&
	((MechAim(mech) != NUM_SECTIONS) && hitMech &&
	    (MechAimType(mech) == MechType(hitMech))));

    if (hitMech) {
	/* Check to see if we're aiming at a particular location. Swarm attacks can't aim. */
	if ((MechAim(mech) != NUM_SECTIONS) && hitMech && Immobile(hitMech)
	    && !tIsSwarmAttack) {
	    roll = NRoll2(hitMech, mech);

	    if (roll == 6 || roll == 7 || roll == 8)
		aim_hit = 1;
	}
    }

    if (!IsMissile(weapindx))
	wWeapDamage =
	    determineDamageFromHit(mech, wSection, wCritSlot, hitMech,
	    hitX, hitY, weapindx, wGattlingShots, wBaseWeapDamage,
	    wAmmoMode, type, modifier, 0);

    /*
       * Ok, if we're not an artillery weapon or missile and we're not in
       * LBX, RAC, Ultra or RFAC mode...
     */
    if (!IsArtillery(weapindx) && !IsMissile(weapindx) && !tIsUltra &&
	!tIsLBX & !tIsRAC) {
	if (hitMech) {
	    if ((MechWeapons[weapindx].special & CHEAT) &&
		(GetPartFireMode(mech, wSection, wCritSlot) & HEAT_MODE)) {
		mech_notify(hitMech, MECHALL,
		    "%cy%chThe flaming plasma sprays all over you!%cn");
		mech_notify(mech, MECHALL,
		    "%cgYou cover your target in flaming plasma!%cn");
		MechWeapHeat(hitMech) += (float) wBaseWeapDamage;
		return;
	    }

	    if (aim_hit)
		hitloc =
		    FindAimHitLoc(mech, hitMech, &isrear, &iscritical);
	    else if (tUsingTC)
		hitloc = FindTCHitLoc(mech, hitMech, &isrear, &iscritical);
	    else
		hitloc =
		    FindTargetHitLoc(mech, hitMech, &isrear, &iscritical);

	    DamageMech(hitMech, mech, LOS, GunPilot(mech), hitloc, isrear,
		iscritical, pc_to_dam_conversion(hitMech, weapindx,
		    wWeapDamage),
		0, weapindx, bth, weapindx, wAmmoMode, tIsSwarmAttack);
	} else {
	    hex_hit(mech, hitX, hitY, weapindx, wAmmoMode, wWeapDamage, 1);
	}

	return;
    }

    /*
       * Since we're here, we're either
       *      - A missile weapon
       *      - An artillery weapon
       *      - An AC in Ultra, RF or LBX mode
       *      - A RAC in RAC mode
     */

    /*
       * Do special case for RACs since they don't have an entry in the MissileHitTable.
       *
       * We're gonna fake it by pretending we're either an SRM-2, SRM-4 or SRM-6, depending
       * upon the mode
     */
    if (tIsRAC) {
	if (GetPartFireMode(mech, wSection, wCritSlot) & RAC_TWOSHOT_MODE)
	    strcpy(strMissileFakeName, "IS.SRM-2");
	else if (GetPartFireMode(mech, wSection,
		wCritSlot) & RAC_FOURSHOT_MODE)
	    strcpy(strMissileFakeName, "IS.SRM-4");
	else if (GetPartFireMode(mech, wSection,
		wCritSlot) & RAC_SIXSHOT_MODE)
	    strcpy(strMissileFakeName, "IS.SRM-6");
	for (loop = 0; MissileHitTable[loop].key != -1; loop++) {
	    if (!strcmp(MissileHitTable[loop].name, strMissileFakeName)) {
		tFoundRACFake = 1;
		break;
	    }
	}

	if (!tFoundRACFake)
	    return;
    } else {
	for (loop = 0; MissileHitTable[loop].key != -1; loop++)
	    if (MissileHitTable[loop].key == weapindx)
		break;
	if (!(MissileHitTable[loop].key == weapindx))
	    return;
    }

    if (IsMissile(weapindx)) {
	if (tIsSwarm && hitMech)	/* No swarms on hex hits */
	    SwarmHitTarget(mech, weapindx, wSection, wCritSlot, hitMech,
		LOS, bth, reallyhit ? bth + 1 : bth - 1,
		(type ==
		    CRAZY_MISSILES) ? MissileHitTable[loop].
		num_missiles[10] * modifier /
		100 : MissileHitTable[loop].num_missiles[10],
		(GetPartAmmoMode(mech, wSection, wCritSlot) & SWARM1_MODE),
		tIsSwarmAttack);
	else
	    MissileHitTarget(mech, weapindx, wSection, wCritSlot, hitMech,
		hitX, hitY, LOS ? 1 : 0, bth,
		reallyhit ? bth + 1 : bth - 1,
		(type ==
		    CRAZY_MISSILES) ? MissileHitTable[loop].
		num_missiles[10] * modifier /
		100 : MissileHitTable[loop].num_missiles[10],
		tIsSwarmAttack);

	return;
    }

    num_missiles_hit =
	MissileHitTable[loop].num_missiles[MissileHitIndex(mech, hitMech,
	    weapindx, wSection, wCritSlot)];
    /*
       * Check for non-missile, multiple hit weapons, like LBXs, RACs, RFACs and Ultras
     */
    if (LOS)
	mech_notify(mech, MECHALL,
	    tprintf("%%cgYou hit with %d %s%s!%%c",
		num_missiles_hit, (tIsUltra ||
		    tIsRAC ? "slug" : tIsLBX ? "pellet" : "missile"),
		(num_missiles_hit > 1 ? "s" : "")));

    if (tIsLBX)
	Missile_Hit(mech, hitMech, hitX, hitY, isrear, iscritical,
	    weapindx, wFireMode, wAmmoMode, num_missiles_hit,
	    tIsLBX ? 1 : wWeapDamage, Clustersize(weapindx), LOS, bth,
	    tIsSwarmAttack);
    else {
	while (num_missiles_hit) {
	    if (hitMech) {
		if (aim_hit)
		    hitloc =
			FindAimHitLoc(mech, hitMech, &isrear, &iscritical);
		if (tUsingTC)
		    hitloc =
			FindTCHitLoc(mech, hitMech, &isrear, &iscritical);
		else
		    hitloc =
			FindTargetHitLoc(mech, hitMech, &isrear,
			&iscritical);
		DamageMech(hitMech, mech, LOS, GunPilot(mech), hitloc,
		    isrear, iscritical, pc_to_dam_conversion(hitMech,
			weapindx, wWeapDamage), 0, weapindx, bth, weapindx,
		    wAmmoMode, tIsSwarmAttack);
	    } else
		hex_hit(mech, hitX, hitY, weapindx, wAmmoMode, wWeapDamage,
		    1);

	    num_missiles_hit--;
	}
    }

}

/****************************************
 * Start: Hex hitting related functions
 ****************************************/

char *hex_target_id(MECH * mech)
{
    if (MechStatus(mech) & LOCK_HEX_IGN)
	return "at the hex, trying to ignite it";
    if (MechStatus(mech) & LOCK_HEX_CLR)
	return "at the hex, trying to clear it";
    if (MechStatus(mech) & LOCK_HEX)
	return "at the hex";
    if (MechStatus(mech) & LOCK_BUILDING)
	return "at the building at";
    return "at";
}

/*
intentional:
	if ignite attack hits, roll 2D6 and consult table:
		(Success Numbers) Weapon Type:
			Flamer 4+,
			Incendiary LRMs 5+,
			Energy Weapon (minus small lasers) 7+,
			Missile or Ballistic (minus GR or SRM2s) 9+

Modifiers for terrain to those bths are as follows:
	Woods/Light Buildings no bth mod,
	Med Bldg +1,
	Heavy Bldg +2,
	Hardened Building +3

A unit attempting to clear a wooded hex runs the risk of setting fire to the woods accidently.
To represent this risk, the player rolls 2D6 before attempting to clear.
If the result is 5 or less, then the woods catch on fire.

If a weapon attack against a unit occupying a wooded hex misses its target and
the weapon can be used to start fires (weapons listed above),
the attacking player rolls 2D6. On a result of 2 or 3, the hex catches fire.
Buildings can't accidently be set on fire.

*/

int canWeaponIgnite(int weapindx)
{
    if (strcmp(&MechWeapons[weapindx].name[3], "ERSmallLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "SmallLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "SmallPulseLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "X-SmallPulseLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "ERSmallPulseLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "HeavySmallLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "GaussRifle") &&
	strcmp(&MechWeapons[weapindx].name[3], "LightGaussRifle") &&
	strcmp(&MechWeapons[weapindx].name[3], "HeavyGaussRifle") &&
	strcmp(&MechWeapons[weapindx].name[3], "MagshotGaussRifle") &&
	strcmp(&MechWeapons[weapindx].name[3], "MachineGun") &&
	strcmp(&MechWeapons[weapindx].name[3], "LightMachineGun") &&
	strcmp(&MechWeapons[weapindx].name[3], "HeavyMachineGun") &&
	strcmp(&MechWeapons[weapindx].name[3], "StreakSRM-2") &&
	strcmp(&MechWeapons[weapindx].name[3], "SRM-2") &&
	strcmp(&MechWeapons[weapindx].name[3], "NarcBeacon") &&
	strcmp(&MechWeapons[weapindx].name[3], "iNarcBeacon"))
	return 1;

    return 0;
}

int canWeaponClear(int weapindx)
{
    if (strcmp(&MechWeapons[weapindx].name[3], "ERSmallLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "SmallLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "SmallPulseLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "X-SmallPulseLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "ERSmallPulseLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "HeavySmallLaser") &&
	strcmp(&MechWeapons[weapindx].name[3], "MachineGun") &&
	strcmp(&MechWeapons[weapindx].name[3], "LightMachineGun") &&
	strcmp(&MechWeapons[weapindx].name[3], "HeavyMachineGun") &&
	strcmp(&MechWeapons[weapindx].name[3], "AC/2") &&
	strcmp(&MechWeapons[weapindx].name[3], "UltraAC/2") &&
	strcmp(&MechWeapons[weapindx].name[3], "CaselessAC/2") &&
	strcmp(&MechWeapons[weapindx].name[3], "HyperAC/2") &&
	strcmp(&MechWeapons[weapindx].name[3], "LightAC/2") &&
	strcmp(&MechWeapons[weapindx].name[3], "RotaryAC/2") &&
	strcmp(&MechWeapons[weapindx].name[3], "LB2-XAC") &&
	strcmp(&MechWeapons[weapindx].name[3], "AC/5") &&
	strcmp(&MechWeapons[weapindx].name[3], "UltraAC/5") &&
	strcmp(&MechWeapons[weapindx].name[3], "CaselessAC/5") &&
	strcmp(&MechWeapons[weapindx].name[3], "HyperAC/5") &&
	strcmp(&MechWeapons[weapindx].name[3], "LightAC/5") &&
	strcmp(&MechWeapons[weapindx].name[3], "RotaryAC/5") &&
	strcmp(&MechWeapons[weapindx].name[3], "LB5-XAC") &&
	strcmp(&MechWeapons[weapindx].name[3], "StreakSRM-2") &&
	strcmp(&MechWeapons[weapindx].name[3], "SRM-2"))
	return 1;

    return 0;
}

void possibly_ignite(MECH * mech, MAP * map, int weapindx, int ammoMode,
    int x, int y, int intentional)
{
    char terrain = GetTerrain(map, x, y);
    int roll = Roll();
    int bth = 13;

    if (MechWeapons[weapindx].special & PCOMBAT)
	return;

    if ((terrain != LIGHT_FOREST) && (terrain != HEAVY_FOREST))
	return;

    if (!strcmp(&MechWeapons[weapindx].name[3], "Flamer") ||
	!strcmp(&MechWeapons[weapindx].name[3], "HeavyFlamer"))
	bth = 4;
    else if (IsMissile(weapindx) && (ammoMode & INFERNO_MODE))
	bth = 5;
    else if (IsBallistic(weapindx) && (ammoMode & AC_FLECHETTE_MODE))
	bth = 5;
    else if (IsEnergy(weapindx) && canWeaponIgnite(weapindx))
	bth = 5;
    else if ((IsMissile(weapindx) || IsBallistic(weapindx)) &&
	canWeaponIgnite(weapindx))
	bth = 9;

    if (roll >= bth)
	fire_hex(mech, x, y, intentional);
}

void possibly_clear(MECH * mech, MAP * map, int weapindx, int ammoMode,
    int damage, int x, int y, int intentional)
{
    int igniteBTH = 5;		/* This is for intentional clearing */
    int igniteRoll = Roll();
    int clearRoll = Roll();

    if (MechWeapons[weapindx].special & PCOMBAT)
	return;

    if (!intentional)
	igniteBTH = 3;

    if (igniteRoll <= igniteBTH) {
	possibly_ignite(mech, map, weapindx, ammoMode, x, y, intentional);
	return;
    }

    if (!canWeaponClear(weapindx))
	return;

    if (clearRoll > damage)
	return;

    clear_hex(mech, x, y, intentional);
    possibly_remove_mines(mech, x, y);
}

void possibly_ignite_or_clear(MECH * mech, int weapindx, int ammoMode,
    int damage, int x, int y, int intentional)
{
    int r;
    MAP *map;

    r = Roll();
    map = FindObjectsData(mech->mapindex);

    if (!map)
	return;

    if (MechStatus(mech) & LOCK_HEX_IGN) {
	possibly_ignite(mech, map, weapindx, ammoMode, x, y, 1);
	return;
    }

    if (MechStatus(mech) & LOCK_HEX_CLR) {
	possibly_clear(mech, map, weapindx, ammoMode, damage, x, y, 1);
	return;
    }

    possibly_clear(mech, map, weapindx, ammoMode, damage, x, y,
	intentional);
}

void hex_hit(MECH * mech, int x, int y, int weapindx,
    int ammoMode, int damage, int ishit)
{
    if (!(MechStatus(mech) & (LOCK_BUILDING | LOCK_HEX | LOCK_HEX_IGN |
		LOCK_HEX_CLR)))
	return;

    /* Ok.. we either try to clear/ignite the hex, or alternatively we try to hit building in it */
    if (MechStatus(mech) & LOCK_BUILDING) {
	if (ishit > 0)
	    hit_building(mech, x, y, weapindx, damage);
    } else {
	possibly_ignite_or_clear(mech, weapindx, ammoMode, damage, x, y,
	    1);

	if (MechStatus(mech) & LOCK_HEX) {
	    possibly_blow_ice(mech, weapindx, x, y);
	    possibly_blow_bridge(mech, weapindx, x, y);
	}
    }
}

/****************************************
 * End: Hex hitting related functions
 ****************************************/
