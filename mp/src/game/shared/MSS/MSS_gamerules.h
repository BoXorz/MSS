
// BOXBOX MasterSword: Source - game rules

#ifndef MSS_GAMERULES_H
#define MSS_GAMERULES_H
#pragma once

#include "gamerules.h"
//#include "teamplay_gamerules.h" BOXBOX changing baseclass of the gamerules to CMultiplayRules
#include "multiplay_gamerules.h"
#include "gamevars_shared.h"

#ifndef CLIENT_DLL
#include "MSS_player.h"
#endif

#define VEC_CROUCH_TRACE_MIN	MSSRules()->GetMSSViewVectors()->m_vCrouchTraceMin
#define VEC_CROUCH_TRACE_MAX	MSSRules()->GetMSSViewVectors()->m_vCrouchTraceMax

/* BOXBOX commenting out
enum
{
	TEAM_COMBINE = 2,
	TEAM_REBELS,
};
*/

#ifdef CLIENT_DLL
	#define CMSSRules C_MSSRules
	#define CMSSGameRulesProxy C_MSSGameRulesProxy
#endif

/*
class CMSSGameRulesProxy : public CGameRulesProxy
{
public:
	DECLARE_CLASS( CMSSGameRulesProxy, CGameRulesProxy );
	DECLARE_NETWORKCLASS();
};
*/

class MSSViewVectors : public CViewVectors
{
public:
	MSSViewVectors( 
		Vector vView,
		Vector vHullMin,
		Vector vHullMax,
		Vector vDuckHullMin,
		Vector vDuckHullMax,
		Vector vDuckView,
		Vector vObsHullMin,
		Vector vObsHullMax,
		Vector vDeadViewHeight,
		Vector vCrouchTraceMin,
		Vector vCrouchTraceMax ) :
			CViewVectors( 
				vView,
				vHullMin,
				vHullMax,
				vDuckHullMin,
				vDuckHullMax,
				vDuckView,
				vObsHullMin,
				vObsHullMax,
				vDeadViewHeight )
	{
		m_vCrouchTraceMin = vCrouchTraceMin;
		m_vCrouchTraceMax = vCrouchTraceMax;
	}

	Vector m_vCrouchTraceMin;
	Vector m_vCrouchTraceMax;	
};


class CMSSRules : public CMultiplayRules // BOXBOX was based on CTeamplayRules
{
public:
	DECLARE_CLASS( CMSSRules, CMultiplayRules ); // BOXBOX again, was CTeamplayRules

#ifdef CLIENT_DLL

	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.

#else

	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.
#endif
	
	CMSSRules();
	virtual ~CMSSRules();

	virtual void Precache( void );
	virtual bool ShouldCollide( int collisionGroup0, int collisionGroup1 ); // BOXBOX TODO change this so players all collide or do not?
	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args );

//	virtual float FlWeaponRespawnTime( CBaseCombatWeapon *pWeapon ); // BOXBOX weapons will not respawn
//	virtual float FlWeaponTryRespawn( CBaseCombatWeapon *pWeapon );
//	virtual Vector VecWeaponRespawnSpot( CBaseCombatWeapon *pWeapon );
//	virtual int WeaponShouldRespawn( CBaseCombatWeapon *pWeapon );
	virtual void Think( void );
	virtual void CreateStandardEntities( void );
	virtual void ClientSettingsChanged( CBasePlayer *pPlayer );
//	virtual int PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget ); // BOXBOX player relationship is simple in MSS
//	virtual void GoToIntermission( void ); // BOXBOX removing intermission
	virtual void DeathNotice( CBasePlayer *pVictim, const CTakeDamageInfo &info );
	virtual const char *GetGameDescription( void );
	// derive this function if your mod uses encrypted weapon info files
	virtual const unsigned char *GetEncryptionKey( void ) { return (unsigned char *)"mS4tEw1N"; } // BOXBOX was "x9Ke0BY7"
	virtual const CViewVectors* GetViewVectors() const;
	const MSSViewVectors* GetMSSViewVectors() const;

//	float GetMapRemainingTime(); // BOXBOX removing timelimit
//	void CleanUpMap();
//	void CheckRestartGame(); // BOXBOX removing these
//	void RestartGame();
	
#ifndef CLIENT_DLL
//	virtual Vector VecItemRespawnSpot( CItem *pItem ); // BOXBOX weapons do not respawn
//	virtual QAngle VecItemRespawnAngles( CItem *pItem );
//	virtual float	FlItemRespawnTime( CItem *pItem );
//	virtual bool	CanHavePlayerItem( CBasePlayer *pPlayer, CBaseCombatWeapon *pItem ); // BOXBOX TODO change this ?
	virtual bool FShouldSwitchWeapon( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon );// BOXBOX TODO do we need this in it's current form?

//	void	AddLevelDesignerPlacedObject( CBaseEntity *pEntity );
//	void	RemoveLevelDesignerPlacedObject( CBaseEntity *pEntity );
//	void	ManageObjectRelocation( void );
//	void    CheckChatForReadySignal( CMSS_Player *pPlayer, const char *chatmsg ); // BOXBOX don't need this
	const char *GetChatFormat( /*bool bTeamOnly,*/ CBasePlayer *pPlayer ) { return "MSS_Chat_All"; }

#endif
	virtual void ClientDisconnected( edict_t *pClient );

//	bool CheckGameOver( void ); // BOXBOX removing this stuff
//	bool IsIntermission( void );

	void PlayerKilled( CBasePlayer *pVictim, const CTakeDamageInfo &info );

	
//	bool	IsTeamplay( void ) { return m_bTeamPlayEnabled;	} // BOXBOX removing teams
//	void	CheckAllPlayersReady( void ); // BOXBOX TODO see if we need this

//	virtual bool IsConnectedUserInfoChangeAllowed( CBasePlayer *pPlayer );
	
private:

#ifndef CLIENT_DLL
	virtual void	InitDefaultAIRelationships( void ); // BOXBOX copied from hl2_gamerules, to get monsters working!
#endif

//	CNetworkVar( bool, m_bTeamPlayEnabled ); // BOXBOX NETCODE removing teams
//	CNetworkVar( float, m_flGameStartTime ); // BOXBOX NETCODE don't need this
//	CUtlVector<EHANDLE> m_hRespawnableItemsAndWeapons;
//	float m_tmNextPeriodicThink;
//	float m_flRestartGameTime; // BOXBOX removing
//	bool m_bCompleteReset;
//	bool m_bAwaitingReadyRestart;
//	bool m_bHeardAllPlayersReady;

//#ifndef CLIENT_DLL
//	bool m_bChangelevelDone;
//#endif
};

inline CMSSRules* MSSRules()
{
	return static_cast<CMSSRules*>(g_pGameRules);
}

#endif //MSS_GAMERULES_H
