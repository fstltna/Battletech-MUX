
/*
 * $Id: btech.c 1.1 02/01/03 01:01:31-00:00 twouters@ $
 *
 * Author: Markus Stenberg <fingon@iki.fi>
 *
 *  Copyright (c) 1996 Markus Stenberg
 *  Copyright (c) 1998-2002 Thomas Wouters
 *  Copyright (c) 2000-2002 Cord Awtry
 *       All rights reserved
 *
 * Created: Thu Sep 19 21:59:09 1996 fingon
 * Last modified: Tue Aug 12 19:39:55 1997 fingon
 *
 */

/*
   Local btech alike stuff for MUX.
   Work's based on MUSE's btechstats.c
 */

#include "interface.h"
#include "mech.h"
#include "create.h"
#include "btechstats_global.h"
#include "coolmenu.h"
#include "extern.h"
#include "mycool.h"
#include "p.mech.utils.h"
#include "p.glue.scode.h"

extern HASHTAB playervaluehash;
extern HASHTAB playervaluehash2;

void list_hashstat(dbref player, const char *tab_name, HASHTAB * htab);

void list_fhashstats(dbref player)
{
#if 0
    int i;

    for (i = 0; factions[i].name; i++)
	if (factions[i].factnewshash)
	    list_hashstat(player, tprintf("FNews:%s", factions[i].key),
		factions[i].factnewshash);
#endif
    list_hashstat(player, "PValues", &playervaluehash);
    list_hashstat(player, "PValues(S)", &playervaluehash2);
}

#define IsHCO(ref) WizP(ref)

void do_show(dbref player, dbref cause, int key, char *arg1, char *arg2)
{
    int i;
    enum {
	CHAVA, CHVAL, CHSKI, CHADV, CHATT,
	MECHVALUES
    };
    char *cmds[] = { "allvalues",
	"values", "skills", "advantages",
	"attributes", "xcodevalues", NULL
    };
    char *cmds_help[] = { "[char_]allvalues", "[char_]values",
	"[char_]skills", "[char_]advantages", "[char_]attributes",
	"xcodevalues [scode]", NULL
    };
    char buf[MBUF_SIZE];

    DOCHECK(!IsHCO(player), "You aren't cleared to know this stuff yet!");

    if (!arg1 || !*arg1) {
	strcpy(buf, "Valid arguments:");
	for (i = 0; cmds_help[i]; i++)
	    sprintf(buf + strlen(buf), "%c %s", i > 0 ? ',' : ' ',
		cmds_help[i]);
	notify(player, buf);
	return;
    }
    i = listmatch(cmds, arg1);
    /* Do da cmd */
    switch (i) {
    case MECHVALUES:
	list_xcodevalues(player);
	return;
    case CHAVA:
	list_charvaluestuff(player, -1);
	return;
    case CHVAL:
	list_charvaluestuff(player, CHAR_VALUE);
	return;
    case CHSKI:
	list_charvaluestuff(player, CHAR_SKILL);
	return;
    case CHADV:
	list_charvaluestuff(player, CHAR_ADVANTAGE);
	return;
    case CHATT:
	list_charvaluestuff(player, CHAR_ATTRIBUTE);
	return;
    }
    notify(player, "Invalid arguments to +show command!");
    return;
}
