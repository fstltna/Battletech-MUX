
/*
 * $Id: glue.scode.c 1.17 04/05/11 16:01:39+02:00 thomas@centurion.xs4all.nl $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1996 Markus Stenberg
 *  Copyright (c) 1998-2002 Thomas Wouters 
 *  Copyright (c) 2000-2002 Cord Awtry 
 *       All rights reserved
 *
 * Created: Wed Oct  9 19:13:52 1996 fingon
 * Last modified: Tue Sep  8 10:00:29 1998 fingon
 *
 */

#include <stdio.h>
#include <string.h>

#include "mech.h"
#include "mech.events.h"
#include "glue.h"
#include "extern.h"
#include "coolmenu.h"
#include "mycool.h"
#include "turret.h"
#include "mech.custom.h"
#include "scen.h"
#include "p.template.h"
#include "p.mech.tech.h"
#include "p.mech.utils.h"
#include "p.mech.partnames.h"
#include "p.econ.h"
#include "p.map.obj.h"
#include "p.mech.tech.damages.h"
#include "p.mech.status.h"
#include "p.mech.sensor.h"
#include "p.btechstats.h"
#include "p.mech.combat.h"
#include "p.mech.damage.h"
#include "p.mechrep.h"
#include "p.mech.move.h"
#include "p.mech.los.h"
#include "p.event.h"
#include "p.mech.restrict.h"
#include "mech.partnames.h"

extern SpecialObjectStruct SpecialObjects[];
dbref match_thing(dbref player, char *name);
char *mechref_path(char *id);
char *setarmorstatus_func(MECH * mech, char *sectstr, char *typestr,
    char *valuestr);

typedef struct {
    int gtype;
    char *name;
    int rel_addr;
    int type;
    int size;
} GMV;

static MECH tmpm;
static MAP tmpmap;
static TURRET_T tmpturret;
static CUSTOM tmpcustom;
static SCEN tmpscen;
static SSIDE tmpsside;
static SSOBJ tmpssobj;

enum { TYPE_STRING, TYPE_CHAR, TYPE_SHORT, TYPE_INT, TYPE_FLOAT,
    TYPE_DBREF, TYPE_STRFUNC, TYPE_STRFUNC_S, TYPE_BV,
    TYPE_STRFUNC_BD, TYPE_CBV, TYPE_CHAR_RO, TYPE_SHORT_RO, TYPE_INT_RO,
    TYPE_FLOAT_RO, TYPE_DBREF_RO, TYPE_LAST_TYPE
};

/* INDENT OFF */
static int scode_in_out[TYPE_LAST_TYPE] = 
/* st ch sh in fl db sf sfs bv sfb cbv ro-ch ro-sh ro-in ro-fl ro-db*/
{   3, 3, 3, 3, 3, 3, 1, 2,  3, 3,  3,   1,    1,    1,    1,    1  };
/* INDENT ON */


#define Uglie(dat) ((int) &dat(&tmpm)) - ((int) &tmpm)
#define UglieV(dat,val) ((int) &dat(&tmpm,val)) - ((int) &tmpm)

#define MeEntry(Name,Func,Type) \
{GTYPE_MECH,Name,Uglie(Func),Type,0}

#define MeEntryS(Name,Func,Type,Size) \
{GTYPE_MECH,Name,Uglie(Func),Type,Size}

#define MeVEntry(Name,Func,Val,Type) \
{GTYPE_MECH,Name,UglieV(Func,Val),Type,0}

#define UglieM(dat) ((int) &tmpmap.dat) - ((int) &tmpmap)
#define MaEntry(Name,Func,Type) \
{GTYPE_MAP,Name,UglieM(Func),Type,0}
#define MaEntryS(Name,Func,Type,Size) \
{GTYPE_MAP,Name,UglieM(Func),Type,Size}

#define UglieT(dat) ((int) &tmpturret.dat) - ((int) &tmpturret)
#define TuEntry(Name,Func,Type) \
{GTYPE_TURRET,Name,UglieT(Func),Type,0}
#define TuEntryS(Name,Func,Type,Size) \
{GTYPE_TURRET,Name,UglieT(Func),Type,Size}

#define UglieC(dat) ((int) &tmpcustom.dat) - ((int) &tmpcustom)
#define CuEntry(Name,Func,Type) \
{GTYPE_CUSTOM,Name,UglieC(Func),Type,0}
#define CuEntryS(Name,Func,Type,Size) \
{GTYPE_CUSTOM,Name,UglieC(Func),Type,Size}

#define UglieScen(dat) ((int) &tmpscen.dat) - ((int) &tmpscen)
#define SEntry(Name,Func,Type) \
{GTYPE_SCEN,Name,UglieScen(Func),Type,0}
#define SEntryS(Name,Func,Type,Size) \
{GTYPE_SCEN,Name,UglieScen(Func),Type,Size}

#define UglieSside(dat) ((int) &tmpsside.dat) - ((int) &tmpsside)
#define SSEntry(Name,Func,Type) \
{GTYPE_SSIDE,Name,UglieSside(Func),Type,0}
#define SSEntryS(Name,Func,Type,Size) \
{GTYPE_SSIDE,Name,UglieSside(Func),Type,Size}

#define UglieSsobj(dat) ((int) &tmpssobj.dat) - ((int) &tmpssobj)
#define SSOEntry(Name,Func,Type) \
{GTYPE_SSOBJ,Name,UglieSsobj(Func),Type,0}
#define SSOEntryS(Name,Func,Type,Size) \
{GTYPE_SSOBJ,Name,UglieSsobj(Func),Type,Size}


char *mechIDfunc(int mode, MECH * mech)
{
    static char buf[3];

    buf[0] = MechID(mech)[0];
    buf[1] = MechID(mech)[1];
    buf[2] = 0;
    return buf;
}

#ifdef BT_ENABLED

static char *mech_getset_ref(int mode, MECH * mech, char *data)
{
    if (mode) {
	strncpy(MechType_Ref(mech), data, 14);
	MechType_Ref(mech)[14] = '\0';
	return NULL;
    } else
	return MechType_Ref(mech);
}

extern char *mech_types[];
extern char *move_types[];

char *mechTypefunc(int mode, MECH * mech, char *arg)
{
    int i;

    if (!mode)
	return mech_types[(short) MechType(mech)];
    /* Should _alter_ mechtype.. weeeel. */
    if ((i = compare_array(mech_types, arg)) >= 0)
	MechType(mech) = i;
    return NULL;
}

char *mechMovefunc(int mode, MECH * mech, char *arg)
{
    int i;

    if (!mode)
	return move_types[(short) MechMove(mech)];
    if ((i = compare_array(move_types, arg)) >= 0)
	MechMove(mech) = i;
    return NULL;
}

char *mechTechTimefunc(int mode, MECH * mech)
{
    static char buf[MBUF_SIZE];
    int n = figure_latest_tech_event(mech);

    sprintf(buf, "%d", n);
    return buf;
}

void apply_mechDamage(MECH * omech, char *buf)
{
    MECH mek;
    MECH *mech = &mek;
    int i, j, i1, i2, i3;
    char *s;
    int do_mag = 0;

    memcpy(mech, omech, sizeof(MECH));
    for (i = 0; i < NUM_SECTIONS; i++) {
	SetSectInt(mech, i, GetSectOInt(mech, i));
	SetSectArmor(mech, i, GetSectOArmor(mech, i));
	SetSectRArmor(mech, i, GetSectORArmor(mech, i));
	for (j = 0; j < NUM_CRITICALS; j++)
	    if (GetPartType(mech, i, j) &&
		!IsCrap(GetPartType(mech, i, j))) {
		if (PartIsDestroyed(mech, i, j))
		    UnDestroyPart(mech, i, j);
		if (IsAmmo(GetPartType(mech, i, j)))
		    SetPartData(mech, i, j, FullAmmo(mech, i, j));
		else
		    SetPartTempNuke(mech, i, j, 0);
	    }
    }
    s = buf;
    while (*s) {
	while (*s && (*s == ' ' || *s == ','))
	    s++;
	if (!(*s))
	    break;
	/* Parse the keyword ; it's one of the many known types */
	if (sscanf(s, "A:%d/%d", &i1, &i2) == 2) {
	    /* Ordinary armor damage */
	    if (i1 >= 0 && i1 < NUM_SECTIONS)
		SetSectArmor(mech, i1, GetSectOArmor(mech, i1) - i2);
	} else if (sscanf(s, "A(R):%d/%d", &i1, &i2) == 2) {
	    /* Ordinary rear armor damage */
	    if (i1 >= 0 && i1 < NUM_SECTIONS)
		SetSectRArmor(mech, i1, GetSectORArmor(mech, i1) - i2);
	} else if (sscanf(s, "I:%d/%d", &i1, &i2) == 2) {
	    /* Ordinary int damage */
	    if (i1 >= 0 && i1 < NUM_SECTIONS)
		SetSectInt(mech, i1, GetSectOInt(mech, i1) - i2);
	} else if (sscanf(s, "C:%d/%d", &i1, &i2) == 2) {
	    /* Dest'ed crit */
	    if (i1 >= 0 && i1 < NUM_SECTIONS)
		DestroyPart(mech, i1, i2);
	} else if (sscanf(s, "G:%d/%d(%d)", &i1, &i2, &i3) == 3) {
	    /* Glitch */
	    if (i1 >= 0 && i1 < NUM_SECTIONS)
		if (i2 >= 0 && i2 < NUM_CRITICALS)
		    SetPartTempNuke(mech, i1, i2, i3);
	} else if (sscanf(s, "R:%d/%d(%d)", &i1, &i2, &i3) == 3) {
	    /* Reload */
	    if (i1 >= 0 && i1 < NUM_SECTIONS)
		if (i2 >= 0 && i2 < NUM_CRITICALS)
		    SetPartData(mech, i1, i2, FullAmmo(mech, i1, i2) - i3);
	}
	while (*s && (*s != ' ' && *s != ','))
	    s++;
    }
    for (i = 0; i < NUM_SECTIONS; i++) {
	if (GetSectInt(mech, i) != GetSectInt(omech, i))
	    SetSectInt(omech, i, GetSectInt(mech, i));
	if (GetSectArmor(mech, i) != GetSectArmor(omech, i))
	    SetSectArmor(omech, i, GetSectArmor(mech, i));
	if (GetSectRArmor(mech, i) != GetSectRArmor(omech, i))
	    SetSectRArmor(omech, i, GetSectRArmor(mech, i));
	for (j = 0; j < NUM_CRITICALS; j++)
	    if (GetPartType(mech, i, j) &&
		!IsCrap(GetPartType(mech, i, j))) {
		if (PartIsDestroyed(mech, i, j) &&
		    !PartIsDestroyed(omech, i, j)) {
		    /* Blast a part */
		    DestroyPart(omech, i, j);
		    do_mag = 1;
		} else if (!PartIsDestroyed(mech, i, j) &&
		    PartIsDestroyed(omech, i, j)) {
		    mech_RepairPart(omech, i, j);
		    SetPartTempNuke(omech, i, j, 0);
		    do_mag = 1;
		}
		if (IsAmmo(GetPartType(mech, i, j))) {
		    if (GetPartData(mech, i, j) != GetPartData(omech, i,
			    j))
			SetPartData(omech, i, j, GetPartData(mech, i, j));
		} else {
		    if (PartTempNuke(mech, i, j) != PartTempNuke(omech, i,
			    j))
			SetPartTempNuke(omech, i, j,
			    PartTempNuke(mech, i, j));

		}
	    }
    }
    if (do_mag && MechType(omech) == CLASS_MECH)
	do_magic(omech);
}

#define ADD(foo...) { if (count++) strcat(buf,","); sprintf(buf+strlen(buf), foo); }

char *mechDamagefunc(int mode, MECH * mech, char *arg)
{
    /* Lists damage in form:
       A:LOC/num[,LOC/num[,LOC(R)/num]],I:LOC/num
       C:LOC/num,R:LOC/num(num),G:LOC/num(num) */
    int i, j;
    static char buf[LBUF_SIZE];
    int count = 0;

    if (mode) {
	apply_mechDamage(mech, arg);
	return "?";
    };
    buf[0] = 0;
    for (i = 0; i < NUM_SECTIONS; i++)
	if (GetSectOInt(mech, i)) {
	    if (GetSectArmor(mech, i) != GetSectOArmor(mech, i))
		ADD("A:%d/%d", i, GetSectOArmor(mech,
			i) - GetSectArmor(mech, i));
	    if (GetSectRArmor(mech, i) != GetSectORArmor(mech, i))
		ADD("A(R):%d/%d", i, GetSectORArmor(mech,
			i) - GetSectRArmor(mech, i));
	}
    for (i = 0; i < NUM_SECTIONS; i++)
	if (GetSectOInt(mech, i))
	    if (GetSectInt(mech, i) != GetSectOInt(mech, i))
		ADD("I:%d/%d", i, GetSectOInt(mech, i) - GetSectInt(mech,
			i));
    for (i = 0; i < NUM_SECTIONS; i++)
	for (j = 0; j < CritsInLoc(mech, i); j++) {
	    if (GetPartType(mech, i, j) &&
		!IsCrap(GetPartType(mech, i, j))) {
		if (PartIsDestroyed(mech, i, j)) {
		    ADD("C:%d/%d", i, j);
		} else {
		    if (IsAmmo(GetPartType(mech, i, j))) {
			if (GetPartData(mech, i, j) != FullAmmo(mech, i,
				j))
			    ADD("R:%d/%d(%d)", i, j, FullAmmo(mech,
				    i, j) - GetPartData(mech, i, j));
		    } else if (PartTempNuke(mech, i, j))
			ADD("G:%d/%d(%d)", i, j, PartTempNuke(mech, i, j));
		}
	    }
	}
    return buf;
}

char *mechCentBearingfunc(int mode, MECH * mech, char *arg)
{
    int x = MechX(mech);
    int y = MechY(mech);
    float fx, fy;
    static char buf[SBUF_SIZE];

    MapCoordToRealCoord(x, y, &fx, &fy);
    sprintf(buf, "%d", FindBearing(MechFX(mech), MechFY(mech), fx, fy));
    return buf;
}

char *mechCentDistfunc(int mode, MECH * mech, char *arg)
{
    int x = MechX(mech);
    int y = MechY(mech);
    float fx, fy;
    static char buf[SBUF_SIZE];

    MapCoordToRealCoord(x, y, &fx, &fy);
    sprintf(buf, "%.2f", FindHexRange(fx, fy, MechFX(mech), MechFY(mech)));
    return buf;
}

#endif

/* Mode:
   0 = char -> bit field
   1 = bit field -> char
   */


static int bv_val(int in, int mode)
{
    int p = 0;

    if (mode == 0) {
	if (in >= 'a' && in <= 'z')
	    return 1 << (in - 'a');
	return 1 << ('z' - 'a' + 1 + (in - 'A'));
    }
    while (in > 0) {
	p++;
	in >>= 1;
    }
    /* Hmm. */
    p--;
    if (p > ('z' - 'a'))
	return 'A' + (p - ('z' - 'a' + 1));
    return 'a' + p;
}

static int text2bv(char *text)
{
    char *c;
    int j = 0;
    int mode_not = 0;

    if (!Readnum(j, text))
	return j;		/* Allow 'old style' as well */

    /* Valid bitvector letters are: a-z (=27), A-Z (=27 more) */
    for (c = text; *c; c++) {
	if (*c == '!') {
	    mode_not = 1;
	    c++;
	};
	if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z')) {
	    int k = bv_val(*c, 0);

	    if (k) {
		if (mode_not)
		    j &= ~k;
		else
		    j |= k;
	    }
	}
	mode_not = 0;
    }
    return j;
}

static char *bv2text(int i)
{
    static char buf[SBUF_SIZE];
    int p = 1;
    char *c = buf;

    while (i > 0) {
	if (i & 1)
	    *(c++) = bv_val(p, 1);
	i >>= 1;
	p <<= 1;
    }
    if (c == buf)
	*(c++) = '-';
    *c = 0;
    return buf;
}

static GMV xcode_data[] = {

    {GTYPE_MECH, "mapindex", ((int) &tmpm.mapindex) - ((int) &tmpm),
	TYPE_DBREF_RO},

    {GTYPE_MECH, "id", (int) mechIDfunc, TYPE_STRFUNC},
    MeEntryS("mechname", MechType_Name, TYPE_STRING, 31),
    MeEntry("maxspeed", MechMaxSpeed, TYPE_FLOAT),
    MeEntry("pilotnum", MechPilot, TYPE_DBREF),
    MeEntry("pilotdam", MechPilotStatus, TYPE_CHAR),
    MeEntry("si", AeroSI, TYPE_CHAR),
    MeEntry("si_orig", AeroSIOrig, TYPE_CHAR),
    MeEntry("speed", MechSpeed, TYPE_FLOAT),
    MeEntry("basewalkspeed", MechBaseWalk, TYPE_INT),
    MeEntry("baserunspeed", MechBaseRun, TYPE_INT),
    MeEntry("heading", MechRFacing, TYPE_SHORT),
    MeEntry("stall", MechStall, TYPE_INT),
    MeEntry("status", MechStatus, TYPE_BV),
    MeEntry("critstatus", MechCritStatus, TYPE_BV),
    MeEntry("tankcritstatus", MechTankCritStatus, TYPE_BV),
    MeEntry("specialsstatus", MechSpecialsStatus, TYPE_BV),
    MeEntry("target", MechTarget, TYPE_DBREF),
    MeEntry("team", MechTeam, TYPE_INT),
    MeEntry("tons", MechTons, TYPE_INT),
    MeEntry("towing", MechCarrying, TYPE_INT_RO),
    MeEntry("heat", MechPlusHeat, TYPE_FLOAT),
    MeEntry("disabled_hs", MechDisabledHS, TYPE_INT_RO),
    MeEntry("overheat", MechHeat, TYPE_FLOAT),
    MeEntry("dissheat", MechMinusHeat, TYPE_FLOAT),
    MeEntry("last_startup", MechLastStartup, TYPE_INT),
    MeEntry("C3iNetworkSize", MechC3iNetworkSize, TYPE_INT_RO),
    MeEntry("MaxSuits", MechMaxSuits, TYPE_INT),
    MeEntry("realweight", MechRTonsV, TYPE_INT),
    MeEntry("StaggerDamage", StaggerDamage, TYPE_INT_RO),
    MeEntry("MechPrefs", MechPrefs, TYPE_BV),

#ifdef BT_ENABLED
    {GTYPE_MECH, "mechtype", (int) mechTypefunc, TYPE_STRFUNC_BD},
    {GTYPE_MECH, "mechmovetype", (int) mechMovefunc, TYPE_STRFUNC_BD},
    {GTYPE_MECH, "mechdamage", (int) mechDamagefunc, TYPE_STRFUNC_BD},
    {GTYPE_MECH, "techtime", (int) mechTechTimefunc, TYPE_STRFUNC},
    {GTYPE_MECH, "centdist", (int) mechCentDistfunc, TYPE_STRFUNC},
    {GTYPE_MECH, "centbearing", (int) mechCentBearingfunc, TYPE_STRFUNC},
    {GTYPE_MECH, "sensors", (int) mechSensorInfo, TYPE_STRFUNC},
    {GTYPE_MECH, "mechref", (int) mech_getset_ref, TYPE_STRFUNC_BD},
#endif

    MeEntry("fuel", AeroFuel, TYPE_INT),
    MeEntry("fuel_orig", AeroFuelOrig, TYPE_INT),
    MeEntry("cocoon", MechCocoon, TYPE_INT_RO),
    MeEntry("numseen", MechNumSeen, TYPE_SHORT),

    MeEntry("fx", MechFX, TYPE_FLOAT),
    MeEntry("fy", MechFY, TYPE_FLOAT),
    MeEntry("fz", MechFZ, TYPE_FLOAT),
    MeEntry("x", MechX, TYPE_SHORT),
    MeEntry("y", MechY, TYPE_SHORT),
    MeEntry("z", MechZ, TYPE_SHORT),

    MeEntry("lrsrange", MechLRSRange, TYPE_CHAR),
    MeEntry("radiorange", MechRadioRange, TYPE_SHORT),
    MeEntry("scanrange", MechScanRange, TYPE_CHAR),
    MeEntry("tacrange", MechTacRange, TYPE_CHAR),
    MeEntry("radiotype", MechRadioType, TYPE_CHAR),
    MeEntry("bv", MechBV, TYPE_INT),
    MeEntry("cargospace", CargoSpace, TYPE_INT),

    MeVEntry("bay0", AeroBay, 0, TYPE_DBREF),
    MeVEntry("bay1", AeroBay, 1, TYPE_DBREF),
    MeVEntry("bay2", AeroBay, 2, TYPE_DBREF),
    MeVEntry("bay3", AeroBay, 3, TYPE_DBREF),

    MeVEntry("turret0", AeroTurret, 0, TYPE_DBREF),
    MeVEntry("turret1", AeroTurret, 1, TYPE_DBREF),
    MeVEntry("turret2", AeroTurret, 2, TYPE_DBREF),

    MeEntry("unusablearcs", AeroUnusableArcs, TYPE_INT_RO),
    MeEntry("maxjumpspeed", MechJumpSpeed, TYPE_FLOAT),
    MeEntry("jumpheading", MechJumpHeading, TYPE_SHORT),
    MeEntry("jumplength", MechJumpLength, TYPE_SHORT),

    MaEntry("buildflag", buildflag, TYPE_CHAR),
    MaEntry("buildonmap", onmap, TYPE_DBREF_RO),
    MaEntry("cf", cf, TYPE_SHORT),
    MaEntry("cfmax", cfmax, TYPE_SHORT),
    MaEntry("gravity", grav, TYPE_CHAR),
    MaEntry("maxcf", cfmax, TYPE_SHORT),
    MaEntry("firstfree", first_free, TYPE_CHAR_RO),
    MaEntry("mapheight", map_height, TYPE_SHORT_RO),
    MaEntry("maplight", maplight, TYPE_CHAR),
    MaEntryS("mapname", mapname, TYPE_STRING, 30),
    MaEntry("mapvis", mapvis, TYPE_CHAR),
    MaEntry("mapwidth", map_width, TYPE_SHORT_RO),
    MaEntry("maxvis", maxvis, TYPE_SHORT),
    MaEntry("temperature", temp, TYPE_CHAR),
    MaEntry("winddir", winddir, TYPE_SHORT),
    MaEntry("windspeed", windspeed, TYPE_SHORT),
    MaEntry("cloudbase", cloudbase, TYPE_SHORT),
    MaEntry("flags", flags, TYPE_CBV),
    MaEntry("sensorflags", sensorflags, TYPE_BV),

    TuEntry("arcs", arcs, TYPE_INT),
    TuEntry("parent", parent, TYPE_DBREF),
    TuEntry("gunner", gunner, TYPE_DBREF),
    TuEntry("target", target, TYPE_DBREF),
    TuEntry("targx", target, TYPE_SHORT),
    TuEntry("targy", target, TYPE_SHORT),
    TuEntry("targz", target, TYPE_SHORT),
    TuEntry("lockmode", lockmode, TYPE_INT),

    CuEntry("state", state, TYPE_INT),
    CuEntry("user", user, TYPE_DBREF),
    CuEntry("submit", submit, TYPE_DBREF),
    CuEntry("allow", allow, TYPE_INT),

    SEntry("state", state, TYPE_INT),
    SEntry("start", start_t, TYPE_INT),
    SEntry("end", end_t, TYPE_INT),

    SSEntryS("slet", slet, TYPE_STRING, 10),
    SSOEntry("state", state, TYPE_INT),

    MeEntry("radio", MechRadio, TYPE_CHAR),
    MeEntry("computer", MechComputer, TYPE_CHAR),
    MeEntry("perception", MechPer, TYPE_INT),

    {-1, NULL, 0, TYPE_STRING}
};

FUNCTION(fun_btsetxcodevalue)
{
    /* fargs[0] = id of the mech
       fargs[1] = name of the value
       fargs[2] = what the value's to be set as
     */
    dbref it;
    int i, spec;
    void *foo;
    void *bar;
    void *(*tempfun) ();

    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1");
    spec = WhichSpecial(it);
    FUNCHECK(!(foo = FindObjectsData(it)), "#-1");
    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    for (i = 0; xcode_data[i].name; i++)
	if (!strcasecmp(fargs[1], xcode_data[i].name) &&
	    xcode_data[i].gtype == spec &&
	    (scode_in_out[xcode_data[i].type] & 2)) {
	    bar = (void *) ((int) foo + xcode_data[i].rel_addr);
	    switch (xcode_data[i].type) {
	    case TYPE_STRFUNC_BD:
	    case TYPE_STRFUNC_S:
		tempfun = (void *) xcode_data[i].rel_addr;
		tempfun(1, (MECH *) foo, (char *) fargs[2]);
		break;
	    case TYPE_STRING:
		strncpy((char *) bar, fargs[2], xcode_data[i].size - 1);
		((char *)bar)[xcode_data[i].size - 1] = '\0';
		break;
	    case TYPE_DBREF:
		*((dbref *) bar) = atoi(fargs[2]);
		break;
	    case TYPE_CHAR:
		*((char *) bar) = atoi(fargs[2]);
		break;
	    case TYPE_SHORT:
		*((short *) bar) = atoi(fargs[2]);
		break;
	    case TYPE_INT:
		*((int *) bar) = atoi(fargs[2]);
		break;
	    case TYPE_FLOAT:
		*((float *) bar) = atof(fargs[2]);
		break;
	    case TYPE_BV:
		*((int *) bar) = text2bv(fargs[2]);
		break;
	    case TYPE_CBV:
		*((byte *) bar) = (byte) text2bv(fargs[2]);
		break;
	    }
	    safe_tprintf_str(buff, bufc, "1");
	    return;
	}
    safe_tprintf_str(buff, bufc, "#-1");
    return;
}

static char *RetrieveValue(void *data, int i)
{
    void *bar = (void *) ((int) data + xcode_data[i].rel_addr);
    static char buf[LBUF_SIZE];
    char *(*tempfun) ();

    switch (xcode_data[i].type) {
    case TYPE_STRFUNC_BD:
    case TYPE_STRFUNC:
	tempfun = (void *) xcode_data[i].rel_addr;
	sprintf(buf, "%s", (char *) tempfun(0, (MECH *) data));
	break;
    case TYPE_STRING:
	sprintf(buf, "%s", (char *) bar);
	break;
    case TYPE_DBREF:
    case TYPE_DBREF_RO:
	sprintf(buf, "%d", (dbref) * ((dbref *) bar));
	break;
    case TYPE_CHAR:
    case TYPE_CHAR_RO:
	sprintf(buf, "%d", (char) *((char *) bar));
	break;
    case TYPE_SHORT:
    case TYPE_SHORT_RO:
	sprintf(buf, "%d", (short) *((short *) bar));
	break;
    case TYPE_INT:
    case TYPE_INT_RO:
	sprintf(buf, "%d", (int) *((int *) bar));
	break;
    case TYPE_FLOAT:
    case TYPE_FLOAT_RO:
	sprintf(buf, "%.2f", (float) *((float *) bar));
	break;
    case TYPE_BV:
	strcpy(buf, bv2text((int) *((int *) bar)));
	break;
    case TYPE_CBV:
	strcpy(buf, bv2text((int) *((char *) bar)));
	break;
    }
    return buf;
}


FUNCTION(fun_btgetxcodevalue)
{
    /* fargs[0] = id of the mech
       fargs[1] = name of the value
     */
    dbref it;
    int i;
    void *foo;
    int spec;

    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1");
    spec = WhichSpecial(it);
    FUNCHECK(!(foo = FindObjectsData(it)), "#-1");
    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    for (i = 0; xcode_data[i].name; i++)
	if (!strcasecmp(fargs[1], xcode_data[i].name) &&
	    xcode_data[i].gtype == spec &&
	    (scode_in_out[xcode_data[i].type] & 1)) {
	    safe_tprintf_str(buff, bufc, "%s", RetrieveValue(foo, i));
	    return;
	}
    safe_tprintf_str(buff, bufc, "#-1");
    return;
}

FUNCTION(fun_btgetrefxcodevalue)
{
    /* fargs[0] = mech ref
       fargs[1] = name of the value
     */
    int i;
    MECH *foo;
    int spec;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    FUNCHECK((foo = load_refmech(fargs[0])) == NULL, "#-1 NO SUCH MECH");
    spec = GTYPE_MECH;
    for (i = 0; xcode_data[i].name; i++)
	if (!strcasecmp(fargs[1], xcode_data[i].name) &&
	    xcode_data[i].gtype == spec &&
	    (scode_in_out[xcode_data[i].type] & 1)) {
	    safe_tprintf_str(buff, bufc, "%s", RetrieveValue(foo, i));
	    return;
	}
    safe_tprintf_str(buff, bufc, "#-1");
    return;
}

void set_xcodestuff(dbref player, void *data, char *buffer)
{
    char *args[2];
    int t, i;
    void *bar;
    void *(*tempfun) ();

    memset(args, 0, sizeof(char *) * 2);

    DOCHECK(silly_parseattributes(buffer, args, 2) != 2,
	"Invalid arguments!");
    t = WhichSpecial(Location(player));
    for (i = 0; xcode_data[i].name; i++)
	if (xcode_data[i].gtype == t)
	    break;
    DOCHECK(!xcode_data[i].name,
	"Error: No xcode values for this type of object found.");
    for (i = 0; xcode_data[i].name; i++)
	if (!strcasecmp(args[0], xcode_data[i].name) &&
	    xcode_data[i].gtype == t &&
	    (scode_in_out[xcode_data[i].type] & 2))
	    break;
    DOCHECK(!xcode_data[i].name,
	"Error: No matching xcode value for this type of object found.");
    bar =
	(void *) ((int) FindObjectsData(Location(player)) +
	xcode_data[i].rel_addr);
    switch (xcode_data[i].type) {
    case TYPE_STRFUNC_BD:
    case TYPE_STRFUNC_S:
	tempfun = (void *) xcode_data[i].rel_addr;
	tempfun(1, getMech(Location(player)), (char *) args[1]);
	break;
    case TYPE_STRING:
	strncpy((char *) bar, args[1], xcode_data[i].size - 1);
	((char *)bar)[xcode_data[i].size - 1] = '\0';
	break;
    case TYPE_DBREF:
	*((dbref *) bar) = atoi(args[1]);
	break;
    case TYPE_CHAR:
	*((char *) bar) = atoi(args[1]);
	break;
    case TYPE_SHORT:
	*((short *) bar) = atoi(args[1]);
	break;
    case TYPE_INT:
	*((int *) bar) = atoi(args[1]);
	break;
    case TYPE_FLOAT:
	*((float *) bar) = atof(args[1]);
	break;
    case TYPE_BV:
	*((int *) bar) = text2bv(args[1]);
	break;
    case TYPE_CBV:
    	*((byte *) bar) = (byte) text2bv(args[1]);
    }
}

void list_xcodestuff(dbref player, void *data, char *buffer)
{
    int t, i, flag = CM_TWO, se_len = 37;
    coolmenu *c = NULL;

    t = WhichSpecial(Location(player));
    for (i = 0; xcode_data[i].name; i++)
	if (xcode_data[i].gtype == t &&
	    (scode_in_out[xcode_data[i].type] & 1))
	    break;
    DOCHECK(!xcode_data[i].name,
	"Error: No xcode values for this type of object found.");
    addline();
    cent(tprintf("Data for %s (%s)", Name(Location(player)),
	    SpecialObjects[t].type));
    addline();
    if (*buffer == '1') {
	flag = CM_ONE;
	se_len = se_len * 2;
    };
    if (*buffer == '4') {
	flag = CM_FOUR;
	se_len = se_len / 2;
    };
    if (*buffer == '1' || *buffer == '4')
	buffer++;
    for (i = 0; xcode_data[i].name; i++) {
	if (xcode_data[i].gtype == t &&
	    (scode_in_out[xcode_data[i].type] & 1)) {
	    /* 1/3(left) = name, 2/3(right)=value */
	    char mask[SBUF_SIZE];
	    char lab[SBUF_SIZE];

	    if (*buffer)
		if (strncasecmp(xcode_data[i].name, buffer,
			strlen(buffer)))
		    continue;
	    strcpy(lab, xcode_data[i].name);
	    lab[se_len / 3] = 0;
	    sprintf(mask, "%%-%ds%%%ds", se_len / 3, se_len * 2 / 3);
	    sim(tprintf(mask, lab, RetrieveValue(data, i)), flag);
	}
    }
    addline();
    ShowCoolMenu(player, c);
    KillCoolMenu(c);
}


#ifdef BT_ENABLED
FUNCTION(fun_btunderrepair)
{
    /* fargs[0] = ref of the mech to be checked */
    int n;
    MECH *mech;
    dbref it;

    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1");
    FUNCHECK(!IsMech(it), "#-2");
    mech = FindObjectsData(it);
    n = figure_latest_tech_event(mech);
    safe_tprintf_str(buff, bufc, "%d", n > 0);
}


FUNCTION(fun_btstores)
{
    /* fargs[0] = id of the bay/mech */
    /* fargs[1] = (optional) name of the part */
    dbref it;
    int i, x = 0;
    int p, b;
    int pile[BRANDCOUNT + 1][NUM_ITEMS];
    char *t;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    FUNCHECK(nfargs < 1 || nfargs > 2,
	     "#-1 FUNCTION (BTSTORES) EXPECTS 1 OR 2 ARGUMENTS");
    it = match_thing(player, fargs[0]);
    FUNCHECK(!Good_obj(it), "#-1 INVALID TARGET");
    if (nfargs > 1) {
	i = -1;
	if (!find_matching_long_part(fargs[1], &i, &p, &b)) {
	    i = -1;
	    FUNCHECK(!find_matching_vlong_part(fargs[1], &i, &p, &b),
		     "#-1 INVALID PART NAME");
        }
        safe_tprintf_str(buff, bufc, "%d", econ_find_items(it, p, b));
    } else {
	memset(pile, 0, sizeof(pile));
	t = silly_atr_get(it, A_ECONPARTS);
	while (*t) {
	    if (*t == '[')
		if ((sscanf(t, "[%d,%d,%d]", &i, &p, &b)) == 3)
		    pile[p][i] += b;
	    t++;
	}
	for (i = 0; i < object_count; i++) {
	    UNPACK_PART(short_sorted[i]->index, p, b);
	    if (pile[b][p]) {
		if (x)
		    safe_str("|", buff, bufc);
		x = pile[b][p];
		safe_tprintf_str(buff, bufc, "%s:%d",
				 part_name_long(p, b), x);
	    }
	}
    }
}

FUNCTION(fun_btmapterr)
{
    /* fargs[0] = reference of map
       fargs[1] = x
       fargs[2] = y
     */
    dbref it;
    MAP *map;
    int x, y;
    int spec;

    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1");
    spec = WhichSpecial(it);
    FUNCHECK(spec != GTYPE_MAP, "#-1");
    FUNCHECK(!(map = FindObjectsData(it)), "#-1");
    FUNCHECK(Readnum(x, fargs[1]), "#-2");
    FUNCHECK(Readnum(y, fargs[2]), "#-2");
    FUNCHECK(x < 0 || y < 0 || x >= map->map_width ||
	y >= map->map_height, "?");
    safe_tprintf_str(buff, bufc, "%c", GetTerrain(map, x, y));
}

FUNCTION(fun_btmapelev)
{
    /* fargs[0] = reference of map
       fargs[1] = x
       fargs[2] = y
     */
    dbref it;
    int i;
    MAP *map;
    int x, y;
    int spec;

    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1");
    spec = WhichSpecial(it);
    FUNCHECK(spec != GTYPE_MAP, "#-1");
    FUNCHECK(!(map = FindObjectsData(it)), "#-1");
    FUNCHECK(Readnum(x, fargs[1]), "#-2");
    FUNCHECK(Readnum(y, fargs[2]), "#-2");
    FUNCHECK(x < 0 || y < 0 || x >= map->map_width ||
	y >= map->map_height, "?");
    i = Elevation(map, x, y);
    if (i < 0)
	safe_tprintf_str(buff, bufc, "-%c", '0' + -i);
    else
	safe_tprintf_str(buff, bufc, "%c", '0' + i);
}

void list_xcodevalues(dbref player)
{
    int i;

    notify(player, "Xcode attributes accessible thru get/setxcodevalue:");
    for (i = 0; xcode_data[i].name; i++)
	notify(player, tprintf("\t%d\t%s", xcode_data[i].gtype,
		xcode_data[i].name));
}

/* Glue functions for easy scode interface to ton of hcode stuff */

FUNCTION(fun_btdesignex)
{
    char *id = fargs[0];

    if (mechref_path(id)) {
	safe_tprintf_str(buff, bufc, "1");
    } else
	safe_tprintf_str(buff, bufc, "0");
}

FUNCTION(fun_btdamages)
{
    /* fargs[0] = id of the mech
     */
    dbref it;
    char *damstr;
    MECH *mech;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1 NOT A MECH");
    FUNCHECK(!IsMech(it), "#-1 NOT A MECH");
    FUNCHECK(!(mech = FindObjectsData(it)), "#-1");
    damstr = damages_func(mech);
    safe_tprintf_str(buff, bufc, damstr ? damstr : "#-1 ERROR");
}

FUNCTION(fun_btcritstatus)
{
    /* fargs[0] = id of the mech
     * fargs[1] = location to show
     */
    dbref it;
    char *critstr;
    MECH *mech;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1 NOT A MECH");
    FUNCHECK(!IsMech(it), "#-1 NOT A MECH");
    FUNCHECK(!(mech = FindObjectsData(it)), "#-1");
    critstr = critstatus_func(mech, fargs[1]);	/* fargs[1] unguaranteed ! */
    safe_tprintf_str(buff, bufc, critstr ? critstr : "#-1 ERROR");
}

FUNCTION(fun_btarmorstatus)
{
    /* fargs[0] = id of the mech
     * fargs[1] = location to show
     */
    dbref it;
    char *infostr;
    MECH *mech;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1 NOT A MECH");
    FUNCHECK(!IsMech(it), "#-1 NOT A MECH");
    FUNCHECK(!(mech = FindObjectsData(it)), "#-1");
    infostr = armorstatus_func(mech, fargs[1]);	/* fargs[1] unguaranteed ! */
    safe_tprintf_str(buff, bufc, infostr ? infostr : "#-1 ERROR");
}

FUNCTION(fun_btweaponstatus)
{
    /* fargs[0] = id of the mech
     * fargs[1] = location to show
     */
    dbref it;
    char *infostr;
    MECH *mech;

    FUNCHECK(nfargs < 1 || nfargs > 2,
	"#-1 FUNCTION (BTWEAPONSTATUS) EXPECTS 1 OR 2 ARGUMENTS");

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1 NOT A MECH");
    FUNCHECK(!IsMech(it), "#-1 NOT A MECH");
    FUNCHECK(!(mech = FindObjectsData(it)), "#-1");
    infostr = weaponstatus_func(mech, nfargs == 2 ? fargs[1] : NULL);
    safe_tprintf_str(buff, bufc, infostr ? infostr : "#-1 ERROR");
}

FUNCTION(fun_btcritref)
{
    /* fargs[0] = ref of the mech
     * fargs[1] = location to show
     */
    char *critstr;
    MECH *mech;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    FUNCHECK((mech = load_refmech(fargs[0])) == NULL, "#-1 NO SUCH MECH");
    critstr = critstatus_func(mech, fargs[1]);	/* fargs[1] unguaranteed ! */
    safe_tprintf_str(buff, bufc, critstr ? critstr : "#-1 ERROR");
}

FUNCTION(fun_btarmorref)
{
    /* fargs[0] = ref of the mech
     * fargs[1] = location to show
     */
    char *infostr;
    MECH *mech;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    FUNCHECK((mech = load_refmech(fargs[0])) == NULL, "#-1 NO SUCH MECH");
    infostr = armorstatus_func(mech, fargs[1]);	/* fargs[1] unguaranteed ! */
    safe_tprintf_str(buff, bufc, infostr ? infostr : "#-1 ERROR");
}

FUNCTION(fun_btweaponref)
{
    /* fargs[0] = ref of the mech
     * fargs[1] = location to show
     */
    char *infostr;
    MECH *mech;

    FUNCHECK(nfargs < 1 || nfargs > 2,
	"#-1 FUNCTION (BTWEAPONREF) EXPECTS 1 OR 2 ARGUMENTS");

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    FUNCHECK((mech = load_refmech(fargs[0])) == NULL, "#-1 NO SUCH MECH");
    infostr = weaponstatus_func(mech, nfargs == 2 ? fargs[1] : NULL);
    safe_tprintf_str(buff, bufc, infostr ? infostr : "#-1 ERROR");
}

FUNCTION(fun_btsetarmorstatus)
{
    /* fargs[0] = id of the mech
     * fargs[1] = location to set
     * fargs[2] = what to change
     * fargs[3] = value to change to.
     */
    dbref it;
    char *infostr;
    MECH *mech;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1 NOT A MECH");
    FUNCHECK(!IsMech(it), "#-1 NOT A MECH");
    FUNCHECK(!(mech = FindObjectsData(it)), "#-1");
    infostr = setarmorstatus_func(mech, fargs[1], fargs[2], fargs[3]);	/* fargs[1] unguaranteed ! */
    safe_tprintf_str(buff, bufc, infostr ? infostr : "#-1 ERROR");
}

FUNCTION(fun_btthreshhold)
{
    /*
     * fargs[0] = skill to query
     */
    int xpth;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    xpth = btthreshhold_func(fargs[0]);
    safe_tprintf_str(buff, bufc, xpth < 0 ? "#%d ERROR" : "%d", xpth);
}

FUNCTION(fun_btdamagemech)
{
    /*
     * fargs[0] = dbref of MECH object
     * fargs[1] = total amount of damage
     * fargs[2] = clustersize
     * fargs[3] = direction of 'attack'
     * fargs[4] = (try to) force crit
     * fargs[5] = message to send to damaged 'mech
     * fargs[6] = message to MechLOSBroadcast, prepended by mech name
     */

    int totaldam, clustersize, direction, iscrit;
    MECH *mech;
    dbref it;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1 NOT A MECH");
    FUNCHECK(!IsMech(it), "#-1 NOT A MECH");
    FUNCHECK(!(mech = FindObjectsData(it)), "#-1 UNABLE TO GET MECHDATA");
    FUNCHECK(Readnum(totaldam, fargs[1]) || totaldam < 1,
	     "#-1 INVALID 2ND ARG");
    FUNCHECK(Readnum(clustersize, fargs[2]) || clustersize < 1,
	     "#-1 INVALID 3RD ARG");
    FUNCHECK(Readnum(direction, fargs[3]), "#-1 INVALID 4TH ARG");
    FUNCHECK(Readnum(iscrit, fargs[4]), "#-1 INVALID 5TH ARG");
    safe_tprintf_str(buff, bufc, "%d", dodamage_func(player, mech,
	    totaldam, clustersize, direction, iscrit, fargs[5], fargs[6]));
}

FUNCTION(fun_bttechstatus)
{
    /*
       * fargs[0] = dbref of MECH object
     */

    dbref it;
    MECH *mech;
    char *infostr;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    it = match_thing(player, fargs[0]);
    FUNCHECK(it == NOTHING || !Examinable(player, it), "#-1 NOT A MECH");
    FUNCHECK(!IsMech(it), "#-1 NOT A MECH");
    FUNCHECK(!(mech = FindObjectsData(it)), "#-1 UNABLE TO GET MECHDATA");
    infostr = techstatus_func(mech);
    safe_tprintf_str(buff, bufc, "%s", infostr ? infostr : "#-1 ERROR");
}

FUNCTION(fun_bttechref)
{
    /* fargs[0] = ref of the mech
     */
    char *infostr;
    MECH *mech;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    FUNCHECK((mech = load_refmech(fargs[0])) == NULL, "#-1 NO SUCH MECH");
    infostr = techstatus_func(mech);
    safe_tprintf_str(buff, bufc, infostr ? infostr : "#-1 ERROR");
}

FUNCTION(fun_bthexlosbroadcast)
{
    /* fargs[0] = mapref
       fargs[1] = x coordinate
       fargs[2] = y coordinate
       fargs[3] = message
     */
    MAP *map;
    int x = -1, y = -1;
    char *msg = fargs[3];
    dbref mapnum;
     
    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");

    while (msg && *msg && isspace(*msg))
    	msg++;
    FUNCHECK(!msg || !*msg, "#-1 INVALID MESSAGE");

    mapnum = match_thing(player, fargs[0]);
    FUNCHECK(mapnum < 0, "#-1 INVALID MAP");
    map = getMap(mapnum);
    FUNCHECK(!map, "#-1 INVALID MAP");

    x = atoi(fargs[1]);
    y = atoi(fargs[2]);
    FUNCHECK(x < 0 || x > map->map_width || y < 0 || y > map->map_height,
    	"#-1 INVALID COORDINATES");
    HexLOSBroadcast(map, x, y, msg);
    safe_tprintf_str(buff, bufc, "1");
}

FUNCTION(fun_btmakepilotroll)
{
    /* fargs[0] = mechref
       fargs[1] = roll modifier
       fargs[2] = damage modifier
     */
     
    MECH *mech;
    int rollmod = 0, dammod = 0;
    dbref mechnum;
    
    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    
    mechnum = match_thing(player, fargs[0]);
    FUNCHECK(mechnum == NOTHING || !Examinable(player, mechnum),
    	"#-1 INVALID MECH");
    FUNCHECK(!IsMech(mechnum), "#-1 INVALID MECH");
    FUNCHECK(!(mech = FindObjectsData(mechnum)), "#-1 INVALID MECH");

    /* No checking on rollmod/dammod, they're assumed to be 0 if invalid. */
    rollmod = atoi(fargs[1]);
    dammod = atoi(fargs[2]);
    
    if (MadePilotSkillRoll(mech, rollmod)) {
        safe_tprintf_str(buff, bufc, "1");
    } else {
        MechFalls(mech, dammod, 1);
        safe_tprintf_str(buff, bufc, "0");
    }
}

FUNCTION(fun_btgetreftech)
{
    /* fargs[0] = mech reference */
    char *infostr;
    MECH *mech;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    FUNCHECK((mech = load_refmech(fargs[0])) == NULL, "#-1 NO SUCH MECH");
    infostr = mechrep_gettechstring(mech);
    safe_tprintf_str(buff, bufc, infostr ? infostr : "#-1 ERROR");
}

FUNCTION(fun_btgetdbreffromid)
{
    /* fargs[0] = mech 
       fargs[1] = target ID */
    MECH *target;
    MECH *mech = NULL;
    dbref mechnum;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    mechnum = match_thing(player, fargs[0]);
    FUNCHECK(mechnum == NOTHING || !Examinable(player, mechnum),
    	"#-1 INVALID MECH/MAP");
    FUNCHECK(strlen(fargs[1]) != 2, "#-1 INVALID TARGETID");
    if (IsMech(mechnum)) {
    	FUNCHECK(!(mech = getMech(mechnum)), "#-1 INVALID MECH");
        mechnum = FindTargetDBREFFromMapNumber(mech, fargs[1]);
    } else if (IsMap(mechnum)) {
    	MAP *map;
    	FUNCHECK(!(map = getMap(mechnum)), "#-1 INVALID MAP");
    	mechnum = FindMechOnMap(map, fargs[1]);
    } else {
    	safe_str("#-1 INVALID MECH/MAP", buff, bufc);
    	return;
    }
    FUNCHECK(mechnum < 0, "#-1 INVALID TARGETID");
    if (mech) {
    	FUNCHECK(!(target = getMech(mechnum)), "#-1 INVALID TARGETID");
    	FUNCHECK(!InLineOfSight_NB(mech, target, MechX(target), MechY(target),
        	           FlMechRange(getMap(mech->mapindex), mech, target)),
        	 "#-1 INVALID TARGETID");
    }
    safe_tprintf_str(buff, bufc, "#%d", (int) mechnum);
}

FUNCTION(fun_btlostohex)
{
    /* fargs[0] = mech 
       fargs[1] = x
       fargs[2] = y
     */
     
    MECH *mech;
    MAP *map;
    int x = -1, y = -1, mechnum;
    float fx, fy;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    mechnum = match_thing(player, fargs[0]);
    FUNCHECK(mechnum == NOTHING || !Examinable(player, mechnum),
    	"#-1 INVALID MECH");
    FUNCHECK(!IsMech(mechnum), "#-1 INVALID MECH");
    FUNCHECK(!(mech = getMech(mechnum)), "#-1 INVALID MECH");
    FUNCHECK(!(map = getMap(mech->mapindex)), "#-1 INTERNAL ERROR");

    x = atoi(fargs[1]);
    y = atoi(fargs[2]);
    FUNCHECK(x < 0 || x > map->map_width || y < 0 || y > map->map_height,
    	"#-1 INVALID COORDINATES");
    MapCoordToRealCoord(x, y, &fx, &fy);
    if (InLineOfSight_NB(mech, NULL, x, y,
                 FindHexRange(MechFX(mech), MechFY(mech), fx, fy)))
        safe_tprintf_str(buff, bufc, "1");
    else
        safe_tprintf_str(buff, bufc, "0");
}

FUNCTION(fun_btlostomech)
{
    /* fargs[0] = mech
       fargs[1] = target
     */

    int mechnum;
    MECH *mech, *target;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    mechnum = match_thing(player, fargs[0]);
    FUNCHECK(mechnum == NOTHING || !Examinable(player, mechnum),
    	"#-1 INVALID MECH");
    FUNCHECK(!IsMech(mechnum), "#-1 INVALID MECH");
    FUNCHECK(!(mech = getMech(mechnum)), "#-1 INVALID MECH");

    mechnum = match_thing(player, fargs[1]);
    FUNCHECK(mechnum == NOTHING || !Examinable(player, mechnum),
    	"#-1 INVALID MECH");
    FUNCHECK(!IsMech(mechnum), "#-1 INVALID MECH");
    FUNCHECK(!(target = getMech(mechnum)), "#-1 INVALID MECH");
    
    if (InLineOfSight_NB(mech, target, MechX(mech), MechY(mech),
            FlMechRange(getmap(mech->mapindex), mech, target)))
        safe_tprintf_str(buff, bufc, "1");
    else
        safe_tprintf_str(buff, bufc, "0");
}

FUNCTION(fun_btaddparts)
{
    /* fargs[0] = mech/map
       fargs[1] = partname
       fargs[2] = quantity
     */
    int loc;
    int index = -1, id = 0, brand = 0, count;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    loc = match_thing(player, fargs[0]);
    FUNCHECK(!Good_obj(loc), "#-1 INVALID TARGET");
    FUNCHECK(strlen(fargs[1]) >= MBUF_SIZE, "#-1 PARTNAME TOO LONG");
    FUNCHECK(!fargs[1], "#-1 NEED PARTNAME");
    count = atoi(fargs[2]);
    FUNCHECK(!count, "1");
    FUNCHECK(!find_matching_short_part(fargs[1], &index, &id, &brand) &&
             !find_matching_vlong_part(fargs[1], &index, &id, &brand) &&
             !find_matching_long_part(fargs[1], &index, &id, &brand), "0");
    econ_change_items(loc, id, brand, count);
    SendEcon(tprintf("#%d added %d %s to #%d", player, count,
                     get_parts_vlong_name(id, brand), loc));
    safe_tprintf_str(buff, bufc, "1");
}

extern int xlate(char *);

FUNCTION(fun_btloadmap)
{
    /* fargs[0] = mapobject
       fargs[1] = mapname
       fargs[2] = clear or not to clear
    */
    int mapdbref;
    MAP * map;

    FUNCHECK(nfargs < 2 || nfargs > 3,
	     "#-1 BTLOADMAP TAKES 2 OR 3 ARGUMENTS");
    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    mapdbref = match_thing(player, fargs[0]);
    FUNCHECK(!Good_obj(mapdbref), "#-1 INVALID TARGET");
    map = getMap(mapdbref);
    FUNCHECK(!map, "#-1 INVALID TARGET");
    switch (map_load(map, fargs[1])) {
    case -1:
	safe_str("#-1 MAP NOT FOUND", buff, bufc);
	return;
    case -2:
	safe_str("#-1 INVALID MAP", buff, bufc);
	return;
    case 0:
	break;
    default:
	safe_str("#-1 UNKNOWN ERROR", buff, bufc);
	return;
    }	
    if (nfargs > 2 && xlate(fargs[2]))
	map_clearmechs(player, (void *)map, "");
    safe_str("1", buff, bufc);
}

FUNCTION(fun_btloadmech)
{
    /* fargs[0] = mechobject
       fargs[1] = mechref
    */
    int mechdbref;
    MECH *mech;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    mechdbref = match_thing(player, fargs[0]);
    FUNCHECK(!Good_obj(mechdbref), "#-1 INVALID TARGET");
    mech = getMech(mechdbref);
    FUNCHECK(!mech, "#-1 INVALID TARGET");
    if (mech_loadnew(player, mech, fargs[1]) == 1) {
	event_remove_data((void *)mech);
	clear_mech_from_LOS(mech);
	safe_str("1", buff, bufc);
    } else {
	safe_str("#-1 UNABLE TO LOAD TEMPLATE", buff, bufc);
    }
}   

extern char radio_colorstr[];

FUNCTION(fun_btmechfreqs)
{
    /* fargs[0] = mechobject
     */
    int mechdbref;
    MECH * mech;
    int i;

    FUNCHECK(!WizR(player), "#-1 PERMISSION DENIED");
    mechdbref = match_thing(player, fargs[0]);
    FUNCHECK(!Good_obj(mechdbref), "#-1 INVALID TARGET");
    mech = getMech(mechdbref);
    FUNCHECK(!mech, "#-1 INVALID TARGET");

    for (i = 0; i < MFreqs(mech); i++) {
	if (i)
	    safe_str(",", buff, bufc);
        safe_tprintf_str(buff, bufc, "%d|%d|%s",
			 i + 1, mech->freq[i],
			 bv2text(mech->freqmodes[i] % FREQ_REST));
	if (mech->freqmodes[i] / FREQ_REST) {
	    safe_tprintf_str(buff, bufc, "|%c",
		radio_colorstr[mech->freqmodes[i] / FREQ_REST - 1]);
	} else {
	    safe_str("|-", buff, bufc);
	}
    }
}

#endif
