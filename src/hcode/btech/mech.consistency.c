
/*
 * $Id: mech.consistency.c 1.9 03/09/30 02:37:39+02:00 thomas@debian.(none) $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1997 Markus Stenberg
 *       All rights reserved
 *
 * Created: Tue Mar  4 17:17:12 1997 fingon
 * Last modified: Thu Jul  9 02:33:13 1998 fingon
 *
 */

#include "mech.h"
#include "coolmenu.h"
#include "mycool.h"
#include "p.mech.custom.h"
#include "p.mech.utils.h"
#include "mech.partnames.h"
#include <math.h>

static char mech_loc_table[][2] = {
    {CTORSO, 1},
    {LTORSO, 2},
    {RTORSO, 2},
    {LARM, 3},
    {RARM, 3},
    {LLEG, 4},
    {RLEG, 4},
    {-1, 0}
};

static char quad_loc_table[][2] = {
    {CTORSO, 1},
    {LTORSO, 2},
    {RTORSO, 2},
    {LARM, 4},
    {RARM, 4},
    {LLEG, 4},
    {RLEG, 4},
    {-1, 0}
};

static char int_data[][5] = {
    {10, 4, 3, 1, 2},
    {15, 5, 4, 2, 3},
    {20, 6, 5, 3, 4},
    {25, 8, 6, 4, 6},
    {30, 10, 7, 5, 7},
    {35, 11, 8, 6, 8},
    {40, 12, 10, 6, 10},
    {45, 14, 11, 7, 11},
    {50, 16, 12, 8, 12},
    {55, 18, 13, 9, 13},
    {60, 20, 14, 10, 14},
    {65, 21, 15, 10, 15},
    {70, 22, 15, 11, 15},
    {75, 23, 16, 12, 16},
    {80, 25, 17, 13, 17},
    {85, 27, 18, 14, 18},
    {90, 29, 19, 15, 19},
    {95, 30, 20, 16, 20},
    {100, 31, 21, 17, 21},
    {-1, 0, 0, 0, 0}
};

static short engine_data[][2] = {
    {0, 0},
    {10, 1},
    {15, 1},
    {20, 1},
    {25, 1},
    {30, 2},
    {35, 2},
    {40, 2},
    {45, 2},
    {50, 3},
    {55, 3},
    {60, 3},
    {65, 4},
    {70, 4},
    {75, 4},
    {80, 5},
    {85, 5},
    {90, 6},
    {95, 6},
    {100, 6},
    {105, 7},
    {110, 7},
    {115, 8},
    {120, 8},
    {125, 8},
    {130, 9},
    {135, 9},
    {140, 10},
    {145, 10},
    {150, 11},
    {155, 11},
    {160, 12},
    {165, 12},
    {170, 12},
    {175, 14},
    {180, 14},
    {185, 15},
    {190, 15},
    {195, 16},
    {200, 17},
    {205, 17},
    {210, 18},
    {215, 19},
    {220, 20},
    {225, 20},
    {230, 21},
    {235, 22},
    {240, 23},
    {245, 24},
    {250, 25},
    {255, 26},
    {260, 27},
    {265, 28},
    {270, 29},
    {275, 31},
    {280, 32},
    {285, 33},
    {290, 35},
    {295, 36},
    {300, 38},
    {305, 39},
    {310, 41},
    {315, 43},
    {320, 45},
    {325, 47},
    {330, 49},
    {335, 51},
    {340, 54},
    {345, 57},
    {350, 59},
    {355, 63},
    {360, 66},
    {365, 69},
    {370, 73},
    {375, 77},
    {380, 82},
    {385, 87},
    {390, 92},
    {395, 98},
    {400, 105},
    {405, 113},
    {410, 122},
    {415, 133},
    {420, 145},
    {425, 159},
    {430, 87 * 2 + 1},
    {435, 97 * 2},
    {440, 107 * 2 + 1},
    {445, 119 * 2 + 1},
    {450, 133 * 2 + 1},
    {455, 150 * 2},
    {460, 168 * 2 + 1},
    {465, 190 * 2},
    {470, 214 * 2 + 1},
    {475, 243 * 2},
    {480, 275 * 2 + 1},
    {485, 313 * 2},
    {490, 356 * 2},
    {495, 405 * 2 + 1},
    {500, 462 * 2 + 1},
    {-1, 0}
};

static int susp_factor(MECH * mech)
{
    int t = MechTons(mech);

    if (MechMove(mech) == MOVE_TRACK)
	return 0;
    if (MechMove(mech) == MOVE_WHEEL)
	return 20;
#define MAP(a,b) if (t <= a) return b
    if (MechMove(mech) == MOVE_FOIL) {
	MAP(10, 60);
	MAP(20, 105);
	MAP(30, 150);
	MAP(40, 195);
	MAP(50, 255);
	MAP(60, 300);
	MAP(70, 345);
	MAP(80, 390);
	MAP(90, 435);
	return 480;
    }
    if (MechMove(mech) == MOVE_HOVER) {
	MAP(10, 40);
	MAP(20, 85);
	MAP(30, 130);
	MAP(40, 175);
	return 235;
    }
    if (MechMove(mech) == MOVE_HULL || MechMove(mech) == MOVE_SUB)
	return 30;
    if (MechMove(mech) == MOVE_VTOL) {
	MAP(10, 50);
	MAP(20, 95);
	return 140;
    }
    return 0;
}

static int round_to_halfton(int weight)
{
    int over = weight % 512;
    if (!over)
    	return weight;
    if (over < 2)
    	return weight - over;
    return weight + (512 - over);
}

static int engine_weight(MECH * mech)
{
    int s = MechEngineSize(mech);
    int i;

    if (MechType(mech) != CLASS_MECH)
	s -= susp_factor(mech);

    for (i = 0; engine_data[i][0] >= 0; i++)
	if (s == engine_data[i][0]) {
	    int weight = engine_data[i][1] * 512;

	    if (MechSpecials(mech) & ICE_TECH)
	    	return weight * 2;

	    if (MechType(mech) == CLASS_VEH_GROUND ||
	        MechType(mech) == CLASS_VTOL ||
	        MechType(mech) == CLASS_VEH_NAVAL)
	        /* Vehicles need extra shielding in case of a fusion engine */
	        weight = round_to_halfton(weight + weight / 2);
	    
	    if (MechSpecials(mech) & XL_TECH)
	    	return round_to_halfton(weight / 2);

	    if (MechSpecials(mech) & XXL_TECH)
	    	return round_to_halfton(weight / 3);
	    	
	    if (MechSpecials(mech) & LE_TECH)
	    	return round_to_halfton(weight * 3/4);
	    
	    if (MechSpecials(mech) & CE_TECH)
	    	return round_to_halfton(weight + weight / 2);
	    	
	    return weight;
	}

    SendError(tprintf("Error in #%d (%s) : No engine found!", mech->mynum,
	    Name(mech->mynum)));
    return 0;
}

static void calc_ints(MECH * mech, int *n, int *tot)
{
    int i;

    *n = 0;
    *tot = 0;
    for (i = 0; i < NUM_SECTIONS; i++) {
	*n += GetSectInt(mech, i);
	*tot += GetSectOInt(mech, i);
    }
    *tot = MAX(1, *tot);
}

static int ammo_weight(MECH * mech)
{
    int i, j, t, w = 0;

    for (i = 0; i < NUM_SECTIONS; i++)
	if (!SectIsDestroyed(mech, i))
	    for (j = 0; j < CritsInLoc(mech, i); j++)
		if (IsAmmo((t = GetPartType(mech, i, j))))
		    w += GetPartData(mech, i,
			j) * 1024 / MechWeapons[Ammo2I(GetPartType(mech, i,
				j))].ammoperton;
    return w;
}

#define MyGetSectOArmor(m,l) (interactive>=0?GetSectOArmor(m,l):GetSectArmor(m,l))
#define MyGetSectORArmor(m,l) (interactive>=0?GetSectORArmor(m,l):GetSectRArmor(m,l))
#define PLOC(a) if (interactive >= 0 || !SectIsDestroyed(mech,a))
#define MyMechNumOsinks(m) ((interactive >= 0) ? (MechNumOsinks(m)) : (MechRealNumsinks(m)))
int mech_weight_sub_mech(dbref player, MECH * mech, int interactive)
{
    int pile[NUM_ITEMS_M];
    int i, j, w, cl, id;
    int armor = 0, armor_o;
    int total = 0;
    coolmenu *c = NULL;
    int shs_size;
    int hs_eff;
    char buf[MBUF_SIZE];
    int ints_c, ints_tot;

    bzero(pile, sizeof(pile));
    if (interactive > 0) {
	addline();
	cent(tprintf("Weight totals for %s", GetMechID(mech)));
	addline();
    }
    calc_ints(mech, &ints_c, &ints_tot);
    for (i = 0; i < NUM_SECTIONS; i++) {
	if (!GetSectOInt(mech, i))
	    continue;
	armor += MyGetSectOArmor(mech, i);
	armor += MyGetSectORArmor(mech, i);
	PLOC(i)
	    for (j = 0; j < NUM_CRITICALS; j++)
	    if (interactive >= 0 || !IsAmmo(GetPartType(mech, i, j)))
		pile[GetPartType(mech, i, j)] += AmmoMod(mech, i, j);
    }
    shs_size = HS_Size(mech);
    hs_eff = HS_Efficiency(mech);
    cl = MechSpecials(mech) & CLAN_TECH;
#define ADDENTRY(text,weight) \
  if (weight) { if (interactive>0) { addmenu(text);addmenu(tprintf("      %6.1f", (float) (weight) / 1024.0));}; total += weight; }
#define ADDENTRY_C(text,count,weight) \
  if (weight) { if (interactive>0) { addmenu(text);addmenu(tprintf("%5d %6.1f", count, (float) (weight) / 1024.0)); }; total += weight; }
    sprintf(buf, "%-12s(%d rating)",
	MechSpecials(mech) & XL_TECH ? "Engine (XL)" : MechSpecials(mech) &
	XXL_TECH ? "Engine (XXL)" : MechSpecials(mech) & CE_TECH ?
	"Engine (Compact)" : MechSpecials(mech) & LE_TECH ?
	"Engine (Light)" : "Engine", MechEngineSize(mech));
    PLOC(CTORSO)
	ADDENTRY(buf, engine_weight(mech));
    PLOC(HEAD)
	ADDENTRY("Cockpit", 3 * 1024);
    PLOC(CTORSO)
	ADDENTRY("Gyro", (int) (ceil(MechEngineSize(mech) / 100.0) * 1024));
    ADDENTRY(MechSpecials(mech) & REINFI_TECH ? "Internals (Reinforced)" :
	MechSpecials(mech) & COMPI_TECH ? "Internals (Composite)" :
	MechSpecials(mech) & ES_TECH ? "Internals (ES)" : "Internals",
	round_to_halfton(MechTons(mech) * 1024 * (interactive >=
	    0 ? ints_tot : ints_c) / 5 / ints_tot /
	(MechSpecials(mech) & REINFI_TECH ? 1 : (MechSpecials(mech) &
		(ES_TECH | COMPI_TECH)) ? 4 : 2)));
    armor_o = armor;
    if (MechSpecials(mech) & FF_TECH)
	armor = armor * 50 / (cl ? 60 : 56);
    else if (MechSpecials2(mech) & HVY_FF_ARMOR_TECH)
	armor = armor * 50 / 62;
    else if (MechSpecials2(mech) & LT_FF_ARMOR_TECH)
	armor = armor * 50 / 53;

    ADDENTRY_C(MechSpecials2(mech) & STEALTH_ARMOR_TECH ? "Armor (Stealth)"
	: MechSpecials2(mech) & HVY_FF_ARMOR_TECH ? "Armor (Hvy FF)" :
	MechSpecials2(mech) & LT_FF_ARMOR_TECH ? "Armor (Lt FF)" :
	MechSpecials(mech) & HARDA_TECH ? "Armor (Hardened)" :
	MechSpecials(mech) & FF_TECH ? "Armor (FF)" : "Armor", armor_o,
	ceil(armor / (8. * (MechSpecials(mech) & HARDA_TECH ? 2 : 1))) * 512);

    if (MyMechNumOsinks(mech)) {
	pile[Special(HEAT_SINK)] =
	    MAX(0, MyMechNumOsinks(mech) * shs_size / hs_eff - 
		(MechSpecials(mech) & ICE_TECH ? 0 : 10) * shs_size);
    } else if (interactive > 0)
	cent(tprintf
	    ("WARNING: HS count may be off, due to certain odd things."));
    for (i = 1; i < NUM_ITEMS_M; i++)
	if (pile[i]) {
	    if (IsWeapon(i)) {
		id = Weapon2I(i);
		ADDENTRY_C(MechWeapons[id].name,
		    pile[i] / GetWeaponCrits(mech, id), crit_weight(mech,
			i) * pile[i]);
	    } else {
		if ((w = crit_weight(mech, i)))
		    ADDENTRY_C(get_parts_long_name(i, 0), pile[i],
			w * pile[i]);
	    }
	}
    ADDENTRY("Cargo/Infantry space", CargoSpace(mech));
    if (interactive > 0) {
	addline();
	vsi(tprintf("%%cgTotal: %s%.1f tons (offset: %.1f)%%cn",
		(total / 1024) > MechTons(mech) ? "%ch%cr" : "",
		(float) (total) / 1024.0,
		MechTons(mech) - (float) (total) / 1024.0));
	addline();
	ShowCoolMenu(player, c);
    }
    KillCoolMenu(c);
    if (interactive < 0)
	total += ammo_weight(mech);
    return MAX(1, total);
}

static int tank_in_pieces(MECH * mech)
{
    int i;

    for (i = 0; i < NUM_SECTIONS; i++)
	if (GetSectInt(mech, i))
	    return 0;
    return 1;
}

int mech_weight_sub_veh(dbref player, MECH * mech, int interactive)
{
    int pile[NUM_ITEMS_M];
    int i, j, w, cl, id, t;
    int armor = 0, armor_o;
    int total = 0;
    coolmenu *c = NULL;
    int shs_size;
    int hs_eff;
    char buf[MBUF_SIZE];
    int es;
    int turr_stuff = 0;
    int ints_c, ints_tot;

    bzero(pile, sizeof(pile));
    calc_ints(mech, &ints_c, &ints_tot);
    if (interactive > 0) {
	addline();
	cent(tprintf("Weight totals for %s", GetMechID(mech)));
	addline();
    }
    for (i = 0; i < NUM_SECTIONS; i++) {
    	if (!(GetSectOInt(mech, i)))
    	    continue;
	armor += MyGetSectOArmor(mech, i);
	armor += MyGetSectORArmor(mech, i);
	for (j = 0; j < CritsInLoc(mech, i); j++) {
	    if (!(t = GetPartType(mech, i, j)))
		continue;
	    if (interactive >= 0 || !SectIsDestroyed(mech, i)) {
		if (interactive >= 0 || !IsAmmo(t))
		    pile[t] += AmmoMod(mech, i, j);
		if (i == TURRET && (MechType(mech) == CLASS_VEH_GROUND ||
			MechType(mech) == CLASS_VEH_NAVAL))
		    if (IsWeapon(t))
			turr_stuff += crit_weight(mech, t);
	    }
	}
    }
    shs_size = HS_Size(mech);
    hs_eff = HS_Efficiency(mech);
    cl = MechSpecials(mech) & CLAN_TECH;
    es = susp_factor(mech);
    if (es)
	sprintf(buf, "%-12s(%d->%d eff/wt rat)",
	    MechSpecials(mech) & LE_TECH ? "Engine (Light)" :
	    MechSpecials(mech) & CE_TECH ? "Engine (Compact)" :
	    MechSpecials(mech) & XXL_TECH ? "Engine (XXL)" :
	    MechSpecials(mech) & XL_TECH ? "Engine (XL)" :
	    MechSpecials(mech) & ICE_TECH ? "Engine (ICE)" : "Engine",
	    MechEngineSize(mech),
	    MechEngineSize(mech) - susp_factor(mech));
    else
	sprintf(buf, "%-12s(%d rating)",
	    MechSpecials(mech) & LE_TECH ? "Engine (Light)" :
	    MechSpecials(mech) & CE_TECH ? "Engine (Compact)" :
	    MechSpecials(mech) & XXL_TECH ? "Engine (XXL)" :
	    MechSpecials(mech) & XL_TECH ? "Engine (XL)" :
	    MechSpecials(mech) & ICE_TECH ? "Engine (ICE)" : "Engine",
	    MechEngineSize(mech));
    if (!tank_in_pieces(mech)) {
	ADDENTRY(buf, (es = engine_weight(mech)));
	if (MechMove(mech) == MOVE_HOVER &&
	    es < (MechTons(mech) * 1024 / 5))
	    ADDENTRY("Engine size fix (-> 1/5 hover wt.)",
		MechTons(mech) * 1024 / 5 - es);
	ADDENTRY("Cockpit", round_to_halfton(MechTons(mech) * 1024 / 20));
	if (MechType(mech) == CLASS_VTOL ||
	    MechMove(mech) == MOVE_HOVER || MechMove(mech) == MOVE_HULL ||
	    MechMove(mech) == MOVE_SUB)
	    ADDENTRY("SpecialComponents",
		round_to_halfton(MechTons(mech) * 1024 / 10));
    }
    PLOC(TURRET)
	if (turr_stuff)
	ADDENTRY("Turret", round_to_halfton(turr_stuff / 10));
    ADDENTRY(MechSpecials(mech) & REINFI_TECH ? "Internals (Reinforced)" :
	MechSpecials(mech) & COMPI_TECH ? "Internals (Composite)" :
	MechSpecials(mech) & ES_TECH ? "Internals (ES)" : "Internals",
	round_to_halfton(MechTons(mech) * 1024 * (interactive >=
	    0 ? ints_tot : ints_c) / 5 / ints_tot /
	(MechSpecials(mech) & REINFI_TECH ? 1 : (MechSpecials(mech) &
		(ES_TECH | COMPI_TECH)) ? 4 : 2)));
    armor_o = armor;

    if (MechSpecials(mech) & FF_TECH)
	armor = armor * 50 / (cl ? 60 : 56);
    else if (MechSpecials2(mech) & HVY_FF_ARMOR_TECH)
	armor = armor * 50 / 62;
    else if (MechSpecials2(mech) & LT_FF_ARMOR_TECH)
	armor = armor * 50 / 53;
    else if (MechSpecials(mech) & HARDA_TECH)
    	armor *= 2;

    ADDENTRY_C(MechSpecials2(mech) & STEALTH_ARMOR_TECH ? "Armor (Stealth)"
	: MechSpecials2(mech) & HVY_FF_ARMOR_TECH ? "Armor (Hvy FF)" :
	MechSpecials2(mech) & LT_FF_ARMOR_TECH ? "Armor (Lt FF)" :
	MechSpecials(mech) & HARDA_TECH ? "Armor (Hardened)" :
	MechSpecials(mech) & FF_TECH ? "Armor (FF)" : "Armor", armor_o,
	round_to_halfton(armor * 1024 / 16));

    pile[Special(HEAT_SINK)] =
	MAX(0, MechRealNumsinks(mech) * shs_size / hs_eff - 
	    (MechSpecials(mech) & ICE_TECH ? 0 : 10) * shs_size);
    for (i = 1; i < NUM_ITEMS_M; i++)
	if (pile[i]) {
	    if (IsWeapon(i)) {
		id = Weapon2I(i);
		ADDENTRY_C(MechWeapons[id].name,
		    pile[i] / GetWeaponCrits(mech, id), crit_weight(mech,
			i) * pile[i]);
	    } else if ((w = crit_weight(mech, i)))
		ADDENTRY_C(get_parts_long_name(i, 0), pile[i],
		    w * pile[i]);
	}
    ADDENTRY("Cargo/Infantry space", CargoSpace(mech));
    if (interactive > 0) {
	addline();
	vsi(tprintf("%%cgTotal: %s%.1f tons (offset: %.1f)%%cn",
		(total / 1024) > MechTons(mech) ? "%ch%cr" : "",
		(float) (total) / 1024.0,
		MechTons(mech) - (float) (total) / 1024.0));
	addline();
	ShowCoolMenu(player, c);
    }
    KillCoolMenu(c);
    if (interactive < 0)
	total += ammo_weight(mech);
    return MAX(1, total);
}

/* Returns: 1024 * MechWeight(in tons) */
int mech_weight_sub(dbref player, MECH * mech, int interactive)
{
    if (MechType(mech) == CLASS_MECH)
	return mech_weight_sub_mech(player, mech, interactive);
    if (MechType(mech) == CLASS_VEH_GROUND ||
	MechType(mech) == CLASS_VTOL ||
	MechType(mech) == CLASS_VEH_NAVAL)
	return mech_weight_sub_veh(player, mech, interactive);
    if (interactive > 0)
	notify(player, "Invalid vehicle type!");
    return 1;
}

void mech_weight(dbref player, void *data, char *buffer)
{
    MECH *mech = (MECH *) data;

    mech_weight_sub(player, mech, 1);
}

#define Table(i,j) \
(MechIsQuad(mech) ? quad_loc_table[i][j] : mech_loc_table[i][j])

static int real_int(MECH * mech, int loc, int ti)
{
    int i;

    if (loc == HEAD)
	return 3;
    for (i = 0; Table(i, 0) >= 0; i++)
	if (loc == Table(i, 0))
	    break;
    if (Table(i, 0) < 0)
	return 0;
    return int_data[ti][Table(i, 1)];
}

#define tank_int(mech)	MAX((MechTons(mech) + 5) / 10, 1)

void vehicle_int_check(MECH * mech, int noisy)
{
    int i, j;

    j = tank_int(mech);
    for (i = 0; i < NUM_SECTIONS; i++)
	if (GetSectOInt(mech, i) && GetSectOInt(mech, i) != j) {
	    if (noisy)
		SendError(tprintf
		    ("Template %s / mech #%d: Invalid internals in loc %d (should be %d, are %d)",
			MechType_Ref(mech), mech->mynum, i, j,
			GetSectOInt(mech, i)));
	    SetSectOInt(mech, i, j);
	    SetSectInt(mech, i, j);
	}
}

void mech_int_check(MECH * mech, int noisy)
{
    int i, j, k;

    if (MechType(mech) != CLASS_MECH) {
	if (MechType(mech) == CLASS_VEH_GROUND ||
	    MechType(mech) == CLASS_VTOL ||
	    MechType(mech) == CLASS_VEH_NAVAL)
	    vehicle_int_check(mech, noisy);
	return;
    }
    for (i = 0; int_data[i][0] >= 0; i++)
	if (MechTons(mech) == int_data[i][0])
	    break;
    if (int_data[i][0] < 0) {
	if (noisy)
	    SendError(tprintf("VERY odd tonnage for #%d: %d.", mech->mynum,
		    MechTons(mech)));
	return;
    }
    k = i;
    for (i = 0; i < NUM_SECTIONS; i++) {
	if (GetSectOInt(mech, i) != (j = real_int(mech, i, k))) {
	    if (noisy)
		SendError(tprintf
		    ("Template %s / mech #%d: Invalid internals in loc %d (should be %d, are %d)",
			MechType_Ref(mech), mech->mynum, i, j,
			GetSectOInt(mech, i)));
	    SetSectOInt(mech, i, j);
	    SetSectInt(mech, i, j);
	}
    }
}
