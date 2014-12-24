
// The MasterSword: Source player

#ifndef MSS_PLAYER_H
#define MSS_PLAYER_H
#pragma once

class CMSS_Player;

#include "basemultiplayerplayer.h"
#include "hl2_playerlocaldata.h"
#include "hl2_player.h"
#include "simtimer.h"
#include "soundenvelope.h"
#include "MSS_player_shared.h"
#include "MSS_shareddefs.h"
#include "MSS_gamerules.h"
#include "MSS_charsave.h"

#include "utldict.h"

//=============================================================================
// >> MSS_Player
//=============================================================================
class CMSSPlayerStateInfo
{
public:
	MSSPlayerState m_iPlayerState;
	const char *m_pStateName;

	void (CMSS_Player::*pfnEnterState)();	// Init and deinit the state.
	void (CMSS_Player::*pfnLeaveState)();

	void (CMSS_Player::*pfnPreThink)();	// Do a PreThink() in this state.
};

class CMSS_Player : public CHL2_Player
{
public:
	DECLARE_CLASS( CMSS_Player, CHL2_Player );

	CMSS_Player();
	~CMSS_Player( void );
	
	static CMSS_Player *CreatePlayer( const char *className, edict_t *ed )
	{
		CMSS_Player::s_PlayerEdict = ed;
		return (CMSS_Player*)CreateEntityByName( className );
	}

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual void Precache( void );
	virtual void Spawn( void );
	virtual void PostThink( void );
	virtual void PreThink( void );
	virtual void PlayerDeathThink( void );
	virtual void SetAnimation( PLAYER_ANIM playerAnim );
//	virtual bool HandleCommand_JoinTeam( int team ); // BOXBOX replacing this to keep spectating without teams
	virtual void HandleCommand_Spectate( void ); // BOXBOX replaced it with this
	virtual bool ClientCommand( const CCommand &args );
	virtual void CreateViewModel( int viewmodelindex = 0 );
	virtual bool BecomeRagdollOnClient( const Vector &force );
	virtual void Event_Killed( const CTakeDamageInfo &info );
	virtual int OnTakeDamage( const CTakeDamageInfo &inputInfo );
	virtual bool WantsLagCompensationOnEntity( const CBasePlayer *pPlayer, const CUserCmd *pCmd, const CBitVec<MAX_EDICTS> *pEntityTransmitBits ) const;
	virtual void FireBullets ( const FireBulletsInfo_t &info );
	virtual bool Weapon_Switch( CBaseCombatWeapon *pWeapon, int viewmodelindex = 0);
	virtual bool BumpWeapon( CBaseCombatWeapon *pWeapon );
//	virtual void ChangeTeam( int iTeam ); // BOXBOX removing teams
	virtual void PickupObject ( CBaseEntity *pObject, bool bLimitMassAndSize );
//	virtual void PlayStepSound( Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force ); // BOXBOX removing footstep override
	virtual void Weapon_Drop( CBaseCombatWeapon *pWeapon, const Vector *pvecTarget = NULL, const Vector *pVelocity = NULL );
	virtual void UpdateOnRemove( void );
	virtual void DeathSound( const CTakeDamageInfo &info );
	virtual CBaseEntity* EntSelectSpawnPoint( void );
		
	int FlashlightIsOn( void );
	void FlashlightTurnOn( void );
	void FlashlightTurnOff( void );
//	void	PrecacheFootStepSounds( void );
//	bool	ValidatePlayerModel( const char *pModel ); // BOXBOX don't need this

	QAngle GetAnimEyeAngles( void ) { return m_angEyeAngles.Get(); }

	Vector GetAttackSpread( CBaseCombatWeapon *pWeapon, CBaseEntity *pTarget = NULL );

	void CheatImpulseCommands( int iImpulse );
	void CreateRagdollEntity( void );
	void GiveAllItems( void ); // BOXBOX TODO impulse 101, keeping for now - for development testing
//	void GiveDefaultItems( void ); // BOXBOX don't need this

	void NoteWeaponFired( void );

	void ResetAnimation( void );
//	void SetPlayerModel( void ); // BOXBOX don't need this
//	void SetPlayerTeamModel( void ); // BOXBOX removing teams
//	Activity TranslateTeamActivity( Activity ActToTranslate );
	
//	float GetNextModelChangeTime( void ) { return m_flNextModelChangeTime; } // BOXBOX removing these
//	float GetNextTeamChangeTime( void ) { return m_flNextTeamChangeTime; }
//	void  PickDefaultSpawnTeam( void );
//	void  SetupPlayerSoundsByModel( const char *pModelName );
//	const char *GetPlayerModelSoundPrefix( void );
//	int	  GetPlayerModelType( void ) { return m_iPlayerSoundType;	}
	
	void  DetonateTripmines( void );

	void Reset();

//	bool IsReady();
//	void SetReady( bool bReady );

	void CheckChatText( char *p, int bufsize );

	void State_Transition( MSSPlayerState newState );
	void State_Enter( MSSPlayerState newState );
	void State_Leave();
	void State_PreThink();
	CMSSPlayerStateInfo *State_LookupInfo( MSSPlayerState state );

	void State_Enter_ACTIVE();
	void State_PreThink_ACTIVE();
	void State_Enter_OBSERVER_MODE();
	void State_PreThink_OBSERVER_MODE();


	virtual bool StartObserverMode( int mode );
	virtual void StopObserverMode( void );


	Vector m_vecTotalBulletForce;	//Accumulator for bullet force in a single frame

	// Tracks our ragdoll entity.
	CNetworkHandle( CBaseEntity, m_hRagdoll );	// networked entity handle 

	virtual bool	CanHearAndReadChatFrom( CBasePlayer *pPlayer );

	void MoveToIntroCamera(); // BOXBOX rip/modifying this from old sdk code

//	bool m_bHasCharFile; // BOXBOX does this player have a character file?	

// BOXBOX MSS STUFF FROM OLD DOGG/BRIAN CODE

	bool m_HasChoosenChar;			//True if the player has choosen a character
	int m_SelectedChar;				//Index of the character choosen

	void SaveChar( );
	void PreLoadChars( void ); // BOXBOX added this to get the character names and models for the Character Selection VGUI panel
//	void SetNumChars( void ); // BOXBOX added this, to set the number of characters this player has on server

	charloadstatus_e LoadChar( int charSlot );
	void GetCharacterSaveFileFields( CUtlVector<MSSaveProperty> &allPlayerData );
	float m_TimeLastSave;

//	CNetworkArray( string_t, m_PreloadedCharInfo_Name, MAX_CHAR_SLOTS ); // BOXBOX trying this a different way...
	CNetworkString( m_szPreloadCharName0, MAX_CHAR_NAME_LENGTH );
	CNetworkString( m_szPreloadCharName1, MAX_CHAR_NAME_LENGTH );
	CNetworkString( m_szPreloadCharName2, MAX_CHAR_NAME_LENGTH );

	CNetworkArray( int, m_PreloadedCharInfo_Model, MAX_CHAR_SLOTS );
	CNetworkVar( bool, m_PreloadedCharInfo_DoneSending );

	virtual void UpdateStats( void );

protected:
	CNetworkVar( int, m_nNumChars ); // BOXBOX added this, so client side (panels mostly) will know how many chars this player has on server

	CNetworkString( m_szCharName, MAX_CHAR_NAME_LENGTH );
	CNetworkVar(unsigned int, m_nGender);
	CNetworkVar(unsigned int, m_nRace);
	CNetworkVar(unsigned int, m_nTotalExp);


//	CNetworkVar(bool, ms_gender);
//	CNetworkVar(unsigned int, ms_race);

	CNetworkVar(unsigned int, ms_playerKills);
	CNetworkVar(unsigned int, ms_mana);
	CNetworkVar(unsigned int, ms_maxMana);
	CNetworkVar(unsigned int, ms_maxHealth);
	CNetworkVar(unsigned int, ms_gold);
	CNetworkVar(unsigned int, ms_alliance);

	CNetworkVar(unsigned int, ms_strength);
	CNetworkVar(unsigned int, ms_dexterity);
	CNetworkVar(unsigned int, ms_concentration);
	CNetworkVar(unsigned int, ms_awareness);
	CNetworkVar(unsigned int, ms_fitness);
	CNetworkVar(unsigned int, ms_level);
	CNetworkVar(unsigned int, ms_warriorSkills);
	CNetworkVar(unsigned int, ms_martialArts);
	CNetworkVar(unsigned int, ms_smallArms);
	CNetworkVar(unsigned int, ms_archery);
	CNetworkVar(unsigned int, ms_spellCasting);
	CNetworkVar(unsigned int, ms_parry);
	CNetworkVar(unsigned int, ms_warriorSkillsExpPercent);
	CNetworkVar(unsigned int, ms_martialArtsExpPercent);
	CNetworkVar(unsigned int, ms_smallArmsExpPercent);
	CNetworkVar(unsigned int, ms_archeryExpPercent);
	CNetworkVar(unsigned int, ms_spellCastingExpPercent);
	CNetworkVar(unsigned int, ms_parryExpPercent);


// BOXBOX end old MSS code

private:

	CNetworkQAngle( m_angEyeAngles );
	CPlayerAnimState   m_PlayerAnimState;

	int m_iLastWeaponFireUsercmd;
	int m_iModelType;
	CNetworkVar( int, m_iSpawnInterpCounter );
//	CNetworkVar( int, m_iPlayerSoundType );

//	float m_flNextModelChangeTime; // BOXBOX removing these
//	float m_flNextTeamChangeTime;

//	float m_flSlamProtectTime;	

	MSSPlayerState m_iPlayerState;
	CMSSPlayerStateInfo *m_pCurStateInfo;

	bool ShouldRunRateLimitedCommand( const CCommand &args );

	// This lets us rate limit the commands the players can execute so they don't overflow things like reliable buffers.
	CUtlDict<float,int>	m_RateLimitLastCommandTimes;

    bool m_bEnterObserver;
//	bool m_bReady;




};

inline CMSS_Player *ToMSSPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<CMSS_Player*>( pEntity );
}

#endif //MSS_PLAYER_H
