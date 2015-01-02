
// BOXBOX This file contains entry point logic for players joining a server.

#include "cbase.h"
#include "MSS_player.h"
#include "MSS_gamerules.h"
#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "entitylist.h"
#include "physics.h"
#include "game.h"
#include "player_resource.h"
#include "engine/IEngineSound.h"
#include "team.h"
#include "viewport_panel_names.h"
#include <filesystem.h> // BOXBOX added

#include "tier0/vprof.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// BOXBOX removing teams
void Host_Say( edict_t *pEdict/*, bool teamonly*/ );

ConVar sv_motd_unload_on_dismissal( "sv_motd_unload_on_dismissal", "0", 0, "If enabled, the MOTD contents will be unloaded when the player closes the MOTD." );

extern CBaseEntity*	FindPickerEntityClass( CBasePlayer *pPlayer, char *classname );
extern bool			g_fGameOver;


void FinishClientPutInServer( CMSS_Player *pPlayer )
{
	pPlayer->InitialSpawn();
//	pPlayer->Spawn();

// BOXBOX adding these to set up the initial camera
		pPlayer->m_takedamage = DAMAGE_NO;
		pPlayer->pl.deadflag = true;
		pPlayer->m_lifeState = LIFE_DEAD;
		pPlayer->AddEffects( EF_NODRAW );
		pPlayer->SetThink( NULL );
		pPlayer->MoveToIntroCamera();
		pPlayer->SetMoveType( MOVETYPE_NONE );
// BOXBOX end

		char sName[128];
		Q_strncpy( sName, pPlayer->GetPlayerName(), sizeof( sName ) );
	
	// First parse the name and remove any %'s
		for ( char *pApersand = sName; pApersand != NULL && *pApersand != 0; pApersand++ )
		{
		// Replace it with a space
			if ( *pApersand == '%' )
					*pApersand = ' ';
		}

	// notify other clients of player joining the game
		UTIL_ClientPrintAll( HUD_PRINTNOTIFY, "#Game_connected", sName[0] != 0 ? sName : "<unconnected>" );

		const ConVar *hostname = cvar->FindVar( "hostname" );
		const char *title = (hostname) ? hostname->GetString() : "MESSAGE OF THE DAY";

		KeyValues *data = new KeyValues("data");
		data->SetString( "title", title );		// info panel title
		data->SetString( "type", "1" );			// BOXBOX type 0 is text, type 1 is INDEX, type 2 is URL, type 3 is FILE
		data->SetString( "msg",	"motd_text" );		// BOXBOX use "motd_text" stringtable entry
		data->SetBool( "unload", sv_motd_unload_on_dismissal.GetBool() );
		data->SetBool( "justjoined", true ); // BOXBOX added this, the player is just joining, so show the character select menu after this closes.

	// BOXBOX preload all three character slots since we just joined
		pPlayer->PreLoadChar( CHARSLOT_ONE );
		pPlayer->PreLoadChar( CHARSLOT_TWO );
		pPlayer->PreLoadChar( CHARSLOT_THREE );

// BOXBOX play music here
		pPlayer->EmitSound("MenuMusic.Intro");

// BOXBOX show the Join Marquis
		pPlayer->ShowViewPortPanel( PANEL_JOIN, true, data );
		data->deleteThis();
		pPlayer->m_bJustJoining = false; // BOXBOX fingers crossed, this should prevent all this from happening at map change
//	}

//	pPlayer->ShowViewPortPanel( PANEL_MAINMENU, false ); // BOXBOX TODO why is this on by default?

	pPlayer->ShowViewPortPanel( PANEL_STATSMENU, false ); // BOXBOX TODO why is this on by default?
	pPlayer->ShowViewPortPanel( PANEL_INVENTORYMENU, false ); // BOXBOX TODO why is this on by default?
	pPlayer->ShowViewPortPanel( PANEL_POWERUPMENU, false ); // BOXBOX TODO why is this on by default?





}


//ClientPutInServer - called each time a player is spawned into the game
void ClientPutInServer( edict_t *pEdict, const char *playername )
{
	// Allocate a CMSS_Player for pev, and call spawn
	CMSS_Player *pPlayer = CMSS_Player::CreatePlayer( "player", pEdict );
	pPlayer->SetPlayerName( playername );


}


void ClientActive( edict_t *pEdict, bool bLoadGame )
{
	// Can't load games in CS!
	Assert( !bLoadGame );

	CMSS_Player *pPlayer = ToMSSPlayer( CBaseEntity::Instance( pEdict ) );
	FinishClientPutInServer( pPlayer );
}



const char *GetGameDescription()
{
	if ( g_pGameRules ) // this function may be called before the world has spawned, and the game rules initialized
		return g_pGameRules->GetGameDescription();
	else
		return "MasterSword: Source";
}


//-----------------------------------------------------------------------------
// Purpose: Given a player and optional name returns the entity of that 
//			classname that the player is nearest facing
//-----------------------------------------------------------------------------
CBaseEntity* FindEntity( edict_t *pEdict, char *classname)
{
	// If no name was given set bits based on the picked
	if (FStrEq(classname,"")) 
	{
		return (FindPickerEntityClass( static_cast<CBasePlayer*>(GetContainingEntity(pEdict)), classname ));
	}
	return NULL;
}


//-----------------------------------------------------------------------------
// Purpose: Precache game-specific models & sounds
//-----------------------------------------------------------------------------

void ClientGamePrecache( void ) // BOXBOX TODO redo all this
{
	CBaseEntity::PrecacheModel("models/player.mdl");
	CBaseEntity::PrecacheModel( "models/gibs/agibs.mdl" );
	CBaseEntity::PrecacheModel ("models/weapons/v_hands.mdl");

	CBaseEntity::PrecacheScriptSound( "MenuMusic.Intro" );
	CBaseEntity::PrecacheScriptSound( "MenuBook.Open" );
	CBaseEntity::PrecacheScriptSound( "MenuBook.Close" );

/*
	CBaseEntity::PrecacheScriptSound( "HUDQuickInfo.LowAmmo" );
	CBaseEntity::PrecacheScriptSound( "HUDQuickInfo.LowHealth" );

	CBaseEntity::PrecacheScriptSound( "FX_AntlionImpact.ShellImpact" );
	CBaseEntity::PrecacheScriptSound( "Missile.ShotDown" );
	CBaseEntity::PrecacheScriptSound( "Bullets.DefaultNearmiss" );
	CBaseEntity::PrecacheScriptSound( "Bullets.GunshipNearmiss" );
	CBaseEntity::PrecacheScriptSound( "Bullets.StriderNearmiss" );
	
	CBaseEntity::PrecacheScriptSound( "Geiger.BeepHigh" );
	CBaseEntity::PrecacheScriptSound( "Geiger.BeepLow" );
*/
}


// called by ClientKill and DeadThink
void respawn( CBaseEntity *pEdict, bool fCopyCorpse )
{
	CMSS_Player *pPlayer = ToMSSPlayer( pEdict );

	if ( pPlayer )
	{
		if ( gpGlobals->curtime > pPlayer->GetDeathTime() + DEATH_ANIMATION_TIME )
		{		
			// respawn player
			pPlayer->Spawn();			
		}
		else
		{
			pPlayer->SetNextThink( gpGlobals->curtime + 0.1f );
		}
	}
}

void GameStartFrame( void )
{
	VPROF("GameStartFrame()");
	if ( g_fGameOver )
		return;

// BOXBOX removing teams
//	gpGlobals->teamplay = (teamplay.GetInt() != 0);

#ifdef DEBUG
	extern void Bot_RunAll();
	Bot_RunAll();
#endif
}


void InstallGameRules()
{
	CreateGameRulesObject( "CMSSRules" ); // BOXBOX was "CHL2MPRules"
}

