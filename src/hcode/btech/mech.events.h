
/*
 * $Id: mech.events.h 1.5 03/09/14 16:51:33+02:00 thomas@debian.(none) $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1996 Markus Stenberg
 *  Copyright (c) 1998-2002 Thomas Wouters
 *  Copyright (c) 2000-2002 Cord Awtry
 *       All rights reserved
 *
 * Created: Fri Aug 30 15:32:12 1996 fingon
 * Last modified: Sat Jun  6 20:14:55 1998 fingon
 *
 */

#ifndef MECH_EVENTS_H
#define MECH_EVENTS_H

#include "mech.h"
#include "event.h"

/* Semi-combat-related events */
#define EVENT_MOVE         1	/* mech */
	/* Updates mech's position and the positionchanged flag */
#define EVENT_DHIT         2	/* <artydata> */
#define EVENT_STARTUP      3	/* mech,timer */
	/* Starts up da mech (timer = int which shows da stage of startup) */
#define EVENT_LOCK         4	/* mech,target */
	/* Engages lock between <mech>,and <target> (breaking LOS stops this) */
#define EVENT_STAND        5	/* mech */
	/* Makes da mech stand */
#define EVENT_JUMP         6
	/* Advances us one jump 'step' */
#define EVENT_RECYCLE      7	/* mech */
	/* Weapons recycling.. */
#define EVENT_JUMPSTABIL   8	/* mech */
	/* If event of this type doesn't exist for mech, we've finished
	   stabilizing after last jump */
#define EVENT_RECOVERY     9	/* mech */
       /* Mech's pilot has chance of recovering from uncon */
#define EVENT_SCHANGE     10	/* mech, <modes> (with first as higher bytes) */
       /* Sensor mode's changing.. */
#define EVENT_DECORATION  11	/* timed decoration removal/happening thingy */
       /* map, mapobj */
#define EVENT_SPOT_LOCK   12	/* spot-lock (Nim's stuff) */
#define EVENT_PLOS        13	/* Possible-lock (mech) */
#define EVENT_SPOT_CHECK  14	/* Range-check for IDF */
#define EVENT_TAKEOFF     15	/* Aero takeoff (mech, secstilllaunch) */
#define EVENT_FALL        16	/* Shutdown mech falling */
#define EVENT_BREGEN      17	/* Building regeneration */
#define EVENT_BREBUILD    18	/* Building rebuild */
#define EVENT_DUMP        19	/* mech, loc : Dump something */

#define EVENT_MASC_FAIL   20	/* MASC roll for failure of stuff */
#define EVENT_MASC_REGEN  21	/* MASC recovery during non-use */
#define EVENT_AMMOWARN    22	/* Warn of running out of ammo */

#define FIRST_AUTO_EVENT  23
#define EVENT_AUTOGOTO    23	/* Autopilot goto */
#define EVENT_AUTOLEAVE   24	/* Autopilot leavebase */
#define EVENT_AUTOCOM     25	/* Autopilot next command */
#define EVENT_AUTOGUN     26	/* Autopilot gun control */
#define EVENT_AUTOGS      27	/* Autopilot gun/sensor change */
#define EVENT_AUTOFOLLOW  28	/* Autopilot follow */
#define LAST_AUTO_EVENT   28

#define EVENT_MRECOVERY   29	/* mech */
#define EVENT_BLINDREC    30
#define EVENT_BURN        31
#define EVENT_SS          32

#define EVENT_HIDE        33
#define EVENT_OOD         34
#define EVENT_NUKEMECH    35
#define EVENT_LATERAL     36
#define EVENT_EXPLODE     37
#define EVENT_AUTO_REPLY  38
#define EVENT_DIG         39

#define FIRST_TECH_EVENT  40

#define EVENT_REPAIR_REPL  FIRST_TECH_EVENT	/* mech,<part> */
#define EVENT_REPAIR_REPLG (FIRST_TECH_EVENT+1)	/* mech,<part> */
#define EVENT_REPAIR_REAT  (FIRST_TECH_EVENT+2)	/* mech,<location> */
#define EVENT_REPAIR_RELO  (FIRST_TECH_EVENT+3)	/* mech,<part/amount> */
#define EVENT_REPAIR_FIX   (FIRST_TECH_EVENT+4)	/* mech,<loc/amount/type> */
#define EVENT_REPAIR_FIXI  (FIRST_TECH_EVENT+5)	/* mech,<loc/amount/type> */
#define EVENT_REPAIR_SCRL  (FIRST_TECH_EVENT+6)	/* mech, loc */
#define EVENT_REPAIR_SCRP  (FIRST_TECH_EVENT+7)	/* mech, loc, part */
#define EVENT_REPAIR_SCRG  (FIRST_TECH_EVENT+8)	/* mech, loc, part */
#define EVENT_REPAIR_REPAG (FIRST_TECH_EVENT+9)	/* mech,<part> */
#define EVENT_REPAIR_REPAP (FIRST_TECH_EVENT+10)	/* mech,<part> */
#define EVENT_REPAIR_MOB   (FIRST_TECH_EVENT+11)	/* mech,<part> */
#define EVENT_REPAIR_UMOB  (FIRST_TECH_EVENT+12)	/* mech,<part> */
#define EVENT_REPAIR_RESE  (FIRST_TECH_EVENT+13)	/* mech,<location> */
#define EVENT_REPAIR_REPSUIT (FIRST_TECH_EVENT+14)	/* mech */
#define EVENT_REPAIR_REPENHCRIT (FIRST_TECH_EVENT+15)	/* mech */

#define LAST_TECH_EVENT    EVENT_REPAIR_REPENHCRIT

#define EVENT_STANDFAIL			53
#define EVENT_SLITECHANGING		54
#define EVENT_HEATCUTOFFCHANGING	55
#define EVENT_VEHICLEBURN		56	/* Burn a side of a vehicle */
#define EVENT_UNSTUN_CREW		57	/* Unstun the crew */
#define EVENT_UNJAM_TURRET		58
#define EVENT_UNJAM_AMMO		59
#define EVENT_STEALTH_ARMOR		60
#define EVENT_NSS 			61
#define EVENT_TAG_RECYCLE		62
#define EVENT_REMOVE_PODS		63
#define EVENT_VEHICLE_EXTINGUISH	64
#define EVENT_ENTER_HANGAR		65
#define EVENT_CHANGING_HULLDOWN		66

/* Not used in the stable branch, just devel */
/* EVENT_BOGDOWNWAIT			67 */

#define EVENT_SCHARGE_FAIL		68	/* SCHARGE roll for failure of stuff */
#define EVENT_SCHARGE_REGEN		69	/* SCHARGE recovery during non-use */

#define EVENT_CHECK_STAGGER		70

#define ETEMPL(a) void a (EVENT *e)

#include "p.aero.move.h"
#include "p.mech.move.h"
#include "p.mech.events.h"

#endif				/* MECH_EVENTS_H */
