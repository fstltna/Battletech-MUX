
/*
 * $Id: eject.c 1.5 02/12/11 13:44:55-00:00 twouters@ $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1996 Markus Stenberg
 *  Copyright (c) 1998-2002 Thomas Wouters
 *  Copyright (c) 2000-2002 Cord Awtry
 *       All rights reserved
 *
 * Created: Mon Oct  7 01:09:08 1996 fingon
 * Last modified: Sat Jun  6 20:45:29 1998 fingon
 *
 */

/* Ejection code */
#include <math.h>
#include "mech.h"
#include "mech.events.h"
#include "p.btechstats.h"
#include "p.mechrep.h"
#include "p.mech.restrict.h"
#include "p.mech.update.h"
#include "p.bsuit.h"
#include "autopilot.h"
#include "p.mech.combat.h"
#include "p.mech.utils.h"
#include "p.btechstats.h"
#include "p.econ_cmds.h"
#include "p.mech.los.h"
#include "p.mech.ood.h"
#include "p.mech.pickup.h"
#include "p.bsuit.h"
#include "p.mech.tag.h"

int tele_contents(dbref from, dbref to, int flag)
{
    dbref i, tmpnext;
    int count = 0;

    SAFE_DOLIST(i, tmpnext, Contents(from))
	if ((flag & TELE_ALL) || !Wiz(i)) {
	if ((flag & TELE_SLAVE) && !Wiz(i)) {
	    s_Slave(i);
	    silly_atr_set(i, A_LOCK, "");
	}
	if (flag & TELE_XP && !Wiz(i))
	    lower_xp(i, mudconf.btech_xploss);
	if (flag & TELE_LOUD)
	    loud_teleport(i, to);
	else
	    hush_teleport(i, to);
	count++;
    }
    return count;
}

/* Delayed blast event, for various reasons */
static void mech_discard_event(EVENT * e)
{
    MECH *mech = (MECH *) e->data;
    dbref i = mech->mynum;

    c_Hardcode(i);
    handle_xcode(GOD, i, 1, 0);
    s_Going(i);
    s_Dark(i);
    s_Zombie(i);
    hush_teleport(i, USED_MW_STORE);
}

void discard_mw(MECH * mech)
{
    if (In_Character(mech->mynum))
	MECHEVENT(mech, EVENT_NUKEMECH, mech_discard_event, 10, 0);
}

void enter_mw_bay(MECH * mech, dbref bay)
{
    tele_contents(mech->mynum, bay, TELE_ALL);	/* Even immortals must get going */
    discard_mw(mech);
}

void pickup_mw(MECH * mech, MECH * target)
{
    dbref mw;

    mw = Contents(target->mynum);
    DOCHECKMA((MechType(mech) != CLASS_MECH) &&
	(MechType(mech) != CLASS_VEH_GROUND) &&
	(MechType(mech) != CLASS_VTOL) &&
	!(MechSpecials(mech) & SALVAGE_TECH),
	"You can't pick up, period.") if (mw > 0)
	notify(mw,
	    tprintf("%s scoops you up and brings you into the cockpit.",
		GetMechToMechID(target, mech)));
    /* Put the player in the picker uppper and clear him from the map */
    MechLOSBroadcast(mech, tprintf("picks up %s.", GetMechID(target)));
    mech_notify(mech, MECHALL,
	tprintf("You pick up the stray mechwarrior from the field."));
    if (MechTeam(target) != MechTeam(mech))
	tele_contents(target->mynum, mech->mynum, TELE_ALL | TELE_SLAVE);
    else
	tele_contents(target->mynum, mech->mynum, TELE_ALL);
    discard_mw(target);
}

static void char_eject(dbref player, MECH * mech)
{
    MECH *m;
    dbref suit;
    char *d;

    suit = create_object(tprintf("MechWarrior - %s", Name(player)));
    silly_atr_set(suit, A_XTYPE, "MECH");
    s_Hardcode(suit);
    handle_xcode(GOD, suit, 0, 1);
    d = silly_atr_get(player, A_MWTEMPLATE);
    if (!(m = getMech(suit))) {
	SendError(tprintf
	    ("Unable to create special obj for #%d's ejection.", player));
	destroy_object(suit);
	notify(player,
	    "Sorry, something serious went wrong, contact a Wizard (can't create RS object)");
	return;
    }
    if (!mech_loadnew(GOD, m, (!d || !*d ||
		!strcmp(d, "#-1")) ? "MechWarrior" : d)) {
	SendError(tprintf
	    ("Unable to load mechwarrior template for #%d's ejection. (%s)",
		player, (!d || !*d) ? "Default template" : d));
	destroy_object(suit);
	notify(player,
	    "Sorry, something serious went wrong, contact a Wizard (can't load MWTemplate)");
	return;
    }
    silly_atr_set(suit, A_MECHNAME, "MechWarrior");
    MechTeam(m) = MechTeam(mech);
    mech_Rsetmapindex(GOD, (void *) m, tprintf("%d", mech->mapindex));
    mech_Rsetxy(GOD, (void *) m, tprintf("%d %d", MechX(mech),
	    MechY(mech)));
    mech_Rsetteam(GOD, (void *) m, tprintf("%d", MechTeam(mech)));
    hush_teleport(suit, mech->mapindex);
    hush_teleport(player, suit);
    MechLOSBroadcast(m, tprintf("ejected from %s!", GetMechID(mech)));
    s_In_Character(suit);
    initialize_pc(player, m);
    MechPilot(m) = player;
    MechTeam(m) = MechTeam(mech);
#ifdef COPY_CHANS_ON_EJECT
    memcpy(m->freq, mech->freq, FREQS * sizeof(m->freq[0]));
    memcpy(m->freqmodes, mech->freqmodes, FREQS * sizeof(m->freqmodes[0]));
#else
#ifdef RANDOM_CHAN_ON_EJECT
    m->freq[0] = random() % 1000000;
    notify(player, tprintf("Emergency radio channel set to %d.", m->freq[0]));
#endif
#endif
    notify(player, "You eject from the unit!");
    if (MechType(mech) == CLASS_MECH) {
	DestroyPart(mech, HEAD, 2);
    }
    if (!Destroyed(mech))
	DestroyAndDump(mech);
}

void mech_eject(dbref player, void *data, char *buffer)
{
    MECH *mech = (MECH *) data;

    cch(MECH_USUALS);
    DOCHECK(IsDS(mech), "Dropships do not support ejection.");
    DOCHECK(!((MechType(mech) == CLASS_MECH) ||
	    (MechType(mech) == CLASS_VTOL) ||
	    (MechType(mech) == CLASS_VEH_GROUND)),
	"This unit has no ejection seat!");
    DOCHECK(FlyingT(mech) &&
	!Landed(mech),
	"Regrettably, right now you can only eject when landed, sorry - no parachute :P");
    DOCHECK(!In_Character(mech->mynum), "This unit isn't in character!");
    DOCHECK(!mudconf.btech_ic, "This MUX isn't in character!");
    DOCHECK(!In_Character(Location(mech->mynum)),
	"Your location isn't in character!");
    DOCHECK(Started(mech) &&
	MechPilot(mech) != player,
	"You aren't in da pilot's seat - no ejection for you!");
    if (!Started(mech)) {
	DOCHECK((char_lookupplayer(GOD, GOD, 0, silly_atr_get(mech->mynum,
			A_PILOTNUM))) != player,
	    "You aren't the official pilot of this thing. Try 'disembark'");
    }
    if (MechType(mech) == CLASS_MECH)
	DOCHECK(PartIsNonfunctional(mech, HEAD, 2),
	    "The parts of cockpit that control ejection are already used. Try 'disembark'");
    /* Ok.. time to eject ourselves */
    char_eject(player, mech);
}

static void char_disembark(dbref player, MECH * mech)
{
    MECH *m;
    dbref suit;
    char *d;
    MAP *mymap;
    int initial_speed;

    suit = create_object(tprintf("MechWarrior - %s", Name(player)));
    silly_atr_set(suit, A_XTYPE, "MECH");
    s_Hardcode(suit);
    handle_xcode(GOD, suit, 0, 1);
    d = silly_atr_get(player, A_MWTEMPLATE);
    if (!(m = getMech(suit))) {
	SendError(tprintf
	    ("Unable to create special obj for #%d's disembarkation.",
		player));
	destroy_object(suit);
	notify(player,
	    "Sorry, something serious went wrong, contact a Wizard (can't create RS object)");
	return;
    }
    if (!mech_loadnew(GOD, m, (!d || !*d ||
		!strcmp(d, "#-1")) ? "MechWarrior" : d)) {
	SendError(tprintf
	    ("Unable to load mechwarrior template for #%d's disembarkation. (%s)",
		player, (!d || !*d) ? "Default template" : d));
	destroy_object(suit);
	notify(player,
	    "Sorry, something serious went wrong, contact a Wizard (can't load MWTemplate)");
	return;
    }
    silly_atr_set(suit, A_MECHNAME, "MechWarrior");
    MechTeam(m) = MechTeam(mech);
    mech_Rsetmapindex(GOD, (void *) m, tprintf("%d", mech->mapindex));
    mech_Rsetxy(GOD, (void *) m, tprintf("%d %d", MechX(mech),
	    MechY(mech)));
    MechZ(m) = MechZ(mech);
    mech_Rsetteam(GOD, (void *) m, tprintf("%d", MechTeam(mech)));
    hush_teleport(suit, mech->mapindex);
    hush_teleport(player, suit);
    s_In_Character(suit);
    initialize_pc(player, m);
    MechPilot(m) = player;
    MechTeam(m) = MechTeam(mech);
#ifdef COPY_CHANS_ON_EJECT
    memcpy(m->freq, mech->freq, FREQS * sizeof(m->freq[0]));
    memcpy(m->freqmodes, mech->freqmodes, FREQS * sizeof(m->freqmodes[0]));
#else
#ifdef RANDOM_CHAN_ON_EJECT
    m->freq[0] = random() % 1000000;
    notify(player, tprintf("Emergency radio channel set to %d.", m->freq[0]));
#endif
#endif
    mymap = getMap(m->mapindex);
    if ((MechZ(m) > (Elevation(mymap, MechX(m), MechY(m)) + 1)) &&
	(MechZ(m) > 0)) {
	notify(player,
	    "You open the hatch and climb out of the unit. Maybe you should have done this while the thing was closer to the ground...");
	MechLOSBroadcast(m, tprintf("jumps out of %s... in mid air !",
		GetMechID(mech)));
	initial_speed =
	    ((MechSpeed(mech) + MechVerticalSpeed(mech)) / MP1) / 2 + 4;
	MECHEVENT(m, EVENT_FALL, mech_fall_event, FALL_TICK,
	    -initial_speed);
    } else {
	MechLOSBroadcast(m, tprintf("climbs out of %s!", GetMechID(mech)));
	notify(player, "You climb out of the unit.");
    }
}


void mech_disembark(dbref player, void *data, char *buffer)
{
    MECH *mech = (MECH *) data;

    cch(MECH_USUALS);
    DOCHECK(!((MechType(mech) == CLASS_MECH) ||
	    (MechType(mech) == CLASS_VTOL) ||
	    (MechType(mech) == CLASS_VEH_GROUND)),
	"The door ! The door ? The Door ?!? Where's the exit in this damned thing ?");

/*  DOCHECK(FlyingT(mech) && !Landed(mech), "What, in the air ? Are you suicidal ?"); */
    DOCHECK(!In_Character(mech->mynum), "This unit isn't in character!");
    DOCHECK(!mudconf.btech_ic, "This MUX isn't in character!");
    DOCHECK(!In_Character(Location(mech->mynum)),
	"Your location isn't in character!");
    DOCHECK(Started(mech) && (MechPilot(mech) == player),
	"While it's running!? Don't be daft.");
    DOCHECK(fabs(MechSpeed(mech)) > 25.,
	"Are you suicidal ? That thing is moving too fast !");
    /* Ok.. time to disembark ourselves */
    char_disembark(player, mech);
}

void mech_sdisembark(dbref player, void *data, char *buffer)
{
    MECH *mech = (MECH *) data;
    MECH *target;
    int newmech;
    MAP *mymap;
    int initial_speed;


    DOCHECK(MechType(mech) != CLASS_BSUIT, "This is not a battlesuit!");
    DOCHECK(In_Character(mech->mynum) && !Wiz(player) &&
	(char_lookupplayer(GOD, GOD, 0, silly_atr_get(mech->mynum,
		    A_PILOTNUM)) != player), "This isn't your mech!");

    newmech = Location(mech->mynum);
    DOCHECK(!(Good_obj(newmech) &&
	    Hardcode(newmech)), "You're not being carried!");
    DOCHECK(!(target = getMech(newmech)), "Not being carried!");
    mech->mapindex = target->mapindex;
    mech_Rsetmapindex(GOD, (void *) mech, tprintf("%d",
	    (int) target->mapindex));
    mech_Rsetxy(GOD, (void *) mech, tprintf("%d %d", MechX(target),
	    MechY(target)));
    MechZ(mech) = MechZ(target);
    mymap = getMap(mech->mapindex);
    loud_teleport(mech->mynum, mech->mapindex);
    MechPilot(mech) = player;
    initialize_pc(MechPilot(mech), mech);
    Startup(mech);
    MarkForLOSUpdate(mech);
    SetCargoWeight(mech);
    UnSetMechPKiller(mech);
    MechLOSBroadcast(mech, "powers up!");
    EvalBit(MechSpecials(mech), SS_ABILITY, ((MechPilot(mech) > 0 &&
		isPlayer(MechPilot(mech))) ? char_getvalue(MechPilot(mech),
		"Sixth_Sense") : 0));
    MechComm(mech) = DEFAULT_COMM;
    if (isPlayer(MechPilot(mech)) && !Quiet(mech->mynum)) {
	MechComm(mech) =
	    char_getskilltarget(MechPilot(mech), "Comm-Conventional", 0);
	MechPer(mech) =
	    char_getskilltarget(MechPilot(mech), "Perception", 0);
    } else {
	MechComm(mech) = 6;
	MechPer(mech) = 6;
    }
    MechCommLast(mech) = 0;
    UnZombifyMech(mech);
    CargoSpace(target) += (MechTons(mech) * 1024);
    MarkForLOSUpdate(target);


    if ((MechZ(mech) > (Elevation(mymap, MechX(mech), MechY(mech)) + 1)) &&
	(MechZ(mech) > 0)) {
	notify(player,
	    "You open the hatch and climb out of the unit. Maybe you should have done this while the thing was closer to the ground...");
	MechLOSBroadcast(mech, tprintf("jumps out of %s... in mid air !",
		GetMechID(target)));
	initial_speed =
	    ((MechSpeed(target) + MechVerticalSpeed(target)) / MP1) / 2 +
	    4;
	MECHEVENT(mech, EVENT_FALL, mech_fall_event, FALL_TICK,
	    -initial_speed);
    } else {
	MechLOSBroadcast(mech, tprintf("climbs out of %s!",
		GetMechID(target)));
	notify(player, "You climb out of the unit.");
    }

}



void mech_embark(dbref player, void *data, char *buffer)
{
    MECH *mech = (MECH *) data;
    MECH *target;
    dbref target_num;
    MAP *newmap;
    int argc;
    char *args[4];

    if (player != GOD)
	cch(MECH_USUAL);
    DOCHECK((MechType(mech) != CLASS_MW) &&
	(MechType(mech) != CLASS_BSUIT),
	"... Why not just TELL it you love it?");
    if (MechType(mech) == CLASS_MW) {
	argc = mech_parseattributes(buffer, args, 1);
	DOCHECK(argc != 1, "Invalid number of arguements.");
	target_num = FindTargetDBREFFromMapNumber(mech, args[0]);
	DOCHECK(target_num == -1,
	    "That target is not in your line of sight.");
	target = getMech(target_num);
	DOCHECK(!target ||
	    !InLineOfSight(mech, target, MechX(target), MechY(target),
		FaMechRange(mech, target)),
	    "That target is not in your line of sight.");
	DOCHECK(MechZ(mech) > (MechZ(target) + 1),
	    "You are too high above the target.");
	DOCHECK(MechZ(mech) < (MechZ(target) - 1),
	    "You can't reach that high !");
	DOCHECK(MechX(mech) != MechX(target) ||
	    MechY(mech) != MechY(target),
	    "You need to be in the same hex!");
	DOCHECK(((MechType(target) != CLASS_MECH) &&
		(MechType(target) != CLASS_VTOL) &&
		(MechType(target) != CLASS_VEH_GROUND)) ||
	    (MechMove(target) == MOVE_NONE) || (!In_Character(mech->mynum))
	    || (!In_Character(target->mynum)) ||
	    (!mudconf.btech_ic),
	    "You dont really see a way to get in there.");
	DOCHECK(MechTeam(mech) != MechTeam(target), "Locked. Damn !");
	DOCHECK(fabs(MechSpeed(target)) > 15.,
	    "Are you suicidal ? That thing is moving too fast !");

	if (MechType(target) == CLASS_MECH) {
	    DOCHECK(!GetSectInt(target, HEAD),
		"Okay, just climb up to-- Wait... where did the head go??");
	    DOCHECK(PartIsDestroyed(target, HEAD, 2),
		"Okay, just climb up and open-- "
		"WTF ? Someone stole the cockpit!");
	    DOCHECK(PartIsNonfunctional(target, HEAD, 2),
		"Okay, just climb up and open-- hey, this door won't budge!");
	}
	mech_notify(mech, MECHALL, tprintf("You climb into %s.",
		GetMechID(target)));
	MechLOSBroadcast(mech, tprintf("climbs into %s.",
		GetMechID(target)));
	tele_contents(mech->mynum, target->mynum, TELE_ALL);
	discard_mw(mech);
	return;
    }
    /* What heppens with a Bsuit squad? */
    /* Check if the vechile has cargo capacity, or is an Omni Mech */
    argc = mech_parseattributes(buffer, args, 1);
    DOCHECK(argc != 1, "Invalid number of arguements.");
    target_num = FindTargetDBREFFromMapNumber(mech, args[0]);
    DOCHECK(target_num == -1, "That target is not in your line of sight.");
    target = getMech(target_num);
    DOCHECK(!target ||
	!InLineOfSight(mech, target, MechX(target), MechY(target),
	    FaMechRange(mech, target)),
	"That target is not in your line of sight.");
    DOCHECK(MechZ(mech) > (MechZ(target) + 1),
	"You are too high above the target.");
    DOCHECK(MechZ(mech) < (MechZ(target) - 1),
	"You can't reach that high !");
    DOCHECK(MechX(mech) != MechX(target) ||
	MechY(mech) != MechY(target), "You need to be in the same hex!");
    DOCHECK(((MechType(target) != CLASS_VTOL) &&
	    (MechType(target) != CLASS_VEH_GROUND)) ||
	(!In_Character(mech->mynum)) || (!In_Character(target->mynum)) ||
	(!mudconf.btech_ic), "You dont really see a way to get in there.");
    DOCHECK(MechTeam(mech) != MechTeam(target), "Locked. Damn !");
    DOCHECK(fabs(MechSpeed(target)) > 15.,
	"Are you suicidal ? That thing is moving too fast !");
    DOCHECK((MechTons(mech) * 1024 * CountBSuitMembers(mech)) >
	CargoSpace(target), "Not enough cargospace for you !");
    newmap = getMap(mech->mapindex);
    mech_notify(mech, MECHALL, tprintf("You climb into %s.",
	    GetMechID(target)));
    MechLOSBroadcast(mech, tprintf("climbs into %s.", GetMechID(target)));
    MarkForLOSUpdate(mech);
    mech_Rsetmapindex(GOD, (void *) mech, tprintf("%d", (int) -1));
    mech_Rsetxy(GOD, (void *) mech, tprintf("%d %d", 0, 0));
    remove_mech_from_map(newmap, mech);
    loud_teleport(mech->mynum, target->mynum);
    CargoSpace(target) -= (MechTons(mech) * 1024);
    Shutdown(mech);

}

void autoeject(dbref player, MECH * mech, int tIsBSuit)
{
    MECH *m;
    dbref suit;
    char *d;

    /* If we're not IC, return */
    if (!player || !In_Character(mech->mynum) || !mudconf.btech_ic ||
	!In_Character(Location(mech->mynum)))
	return;

    /* Create the MW object */
    suit = create_object(tprintf("MechWarrior - %s", Name(player)));
    silly_atr_set(suit, A_XTYPE, "MECH");
    s_Hardcode(suit);
    handle_xcode(GOD, suit, 0, 1);
    d = silly_atr_get(player, A_MWTEMPLATE);
    if (!(m = getMech(suit))) {
	SendError(tprintf
	    ("Unable to create special obj for #%d's ejection.", player));
	destroy_object(suit);
	notify(player,
	    "Sorry, something serious went wrong, contact a Wizard (can't create RS object)");
	return;
    }
    if (!mech_loadnew(GOD, m, (!d || !*d ||
		!strcmp(d, "#-1")) ? "MechWarrior" : d)) {
	SendError(tprintf
	    ("Unable to load mechwarrior template for #%d's ejection. (%s)",
		player, (!d || !*d) ? "Default template" : d));
	destroy_object(suit);
	notify(player,
	    "Sorry, something serious went wrong, contact a Wizard (can't load MWTemplate)");
	return;
    }
    silly_atr_set(suit, A_MECHNAME, "MechWarrior");
    MechTeam(m) = MechTeam(mech);
    mech_Rsetmapindex(GOD, (void *) m, tprintf("%d", mech->mapindex));
    mech_Rsetxy(GOD, (void *) m, tprintf("%d %d", MechX(mech),
	    MechY(mech)));
    mech_Rsetteam(GOD, (void *) m, tprintf("%d", MechTeam(mech)));

    /* Tele the MW to the map and player to the MW */
    hush_teleport(suit, mech->mapindex);
    hush_teleport(player, suit);

    /* Init the sucker */
    s_In_Character(suit);
    initialize_pc(player, m);
    MechPilot(m) = player;
    MechTeam(m) = MechTeam(mech);
#ifdef COPY_CHANS_ON_EJECT
    memcpy(m->freq, mech->freq, FREQS * sizeof(m->freq[0]));
    memcpy(m->freqmodes, mech->freqmodes, FREQS * sizeof(m->freqmodes[0]));
#else
#ifdef RANDOM_CHAN_ON_EJECT
    m->freq[0] = random() % 1000000;
    notify(player, tprintf("Emergency radio channel set to %d.", m->freq[0]));
#endif
#endif

    if (tIsBSuit) {
	MechLOSBroadcast(m, "climbs out of one of the destroyed suits!");
	notify(player, "You climb out of the unit!");
    } else {
	MechLOSBroadcast(m, tprintf("ejected from %s!", GetMechID(mech)));
	initiate_ood(player, m, tprintf("%d %d %d", MechX(m), MechY(m),
		150));
	notify(player, "You eject from the unit!");
    }
}
