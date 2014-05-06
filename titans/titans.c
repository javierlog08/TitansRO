// Copyright (c) Hercules Dev Team, licensed under GNU GPL.
// See the LICENSE file
// Sample Hercules Plugin

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../common/HPMi.h"
#include "../common/mmo.h"
#include "../common/socket.h"
#include "../common/malloc.h"
#include "../map/script.h"
#include "../map/pc.h"
#include "../map/clif.h"
#include "../map/party.h"
#include "../common/strlib.h"

#include "../common/HPMDataCheck.h" /* should always be the last file included! (if you don't make it last, it'll intentionally break compile time) */

/********************************************************************************************************
* Plugin de titans
* Autor: Javierlog08
* Descripcion: 
*Plugin que contiene script commands y commands que trabajaban con algunos de los npcs propios de nuestro server
**********************************************************************************************************/
HPExport struct hplugin_info pinfo = {
	"Titans",		// Plugin name
	SERVER_TYPE_MAP,// Which server types this plugin works with?
	"0.1",			// Plugin version
	HPM_VERSION,	// HPM Version (don't change, macro is automatically updated)
};

/**
* Comando que limpia la pantalla del NPC tras haber finalizado una seleccion.
* Es una solucion a next sin obligar al usuario a presionar el boton [next] desde el cliente.
*/
BUILDIN(next2)
{
	TBL_PC* sd;
	int fd;

	sd = script->rid2sd(st);
	fd = sd->fd;
	if( sd == NULL )
		return true;
	clif->scriptclear(sd,sd->npc_id);
	//npc->scriptcont(sd,st->oid, false);
	return true;
}

BUILDIN(strcharinfo2)
{
	TBL_PC *sd;
	int num;
	struct guild* g;
	struct party_data* p;

	if( script_hasdata(st,3) )
		sd=map->nick2sd(script_getstr(st,3));
	else
		sd=script->rid2sd(st);

	if (!sd) { //Avoid crashing....
		script_pushconststr(st,"");
		return true;
	}
	num=script_getnum(st,2);
	switch(num) {
		case 0:
			script_pushstrcopy(st,sd->status.name);
			break;
		case 1:
			if( ( p = party->search(sd->status.party_id) ) != NULL ) {
				script_pushstrcopy(st,p->party.name);
			} else {
				script_pushconststr(st,"");
			}
			break;
		case 2:
			if( ( g = sd->guild ) != NULL ) {
				script_pushstrcopy(st,g->name);
			} else {
				script_pushconststr(st,"");
			}
			break;
		case 3:
			script_pushconststr(st,map->list[sd->bl.m].name);
			break;
		default:
			ShowWarning("buildin_strcharinfo: unknown parameter.\n");
			script_pushconststr(st,"");
			break;
	}

	return true;
}

/* run when server starts */
HPExport void plugin_init(void) {

	/* core interfaces */
	iMalloc = GET_SYMBOL("iMalloc");

	/* map-server interfaces */
	script = GET_SYMBOL("script");
	clif = GET_SYMBOL("clif");
	pc = GET_SYMBOL("pc");
	strlib = GET_SYMBOL("strlib");
	map = GET_SYMBOL("map");
	status = GET_SYMBOL("status");
	npc = GET_SYMBOL("npc");
	mob = GET_SYMBOL("mob");
	party = GET_SYMBOL("party");

	addScriptCommand("next2", "",next2);
	addScriptCommand("strcharinfo2","i?",strcharinfo2);
}
