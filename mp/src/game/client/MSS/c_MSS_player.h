//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef MSS_PLAYER_H
#define HL2MP_PLAYER_H
#pragma once

class C_MSS_Player;
#include "c_basehlplayer.h"
#include "MSS_player_shared.h"
#include "MSS_shareddefs.h"
#include "beamdraw.h"

//=============================================================================
// >> HL2MP_Player
//=============================================================================
class C_MSS_Player : public C_BaseHLPlayer
{
public:
	DECLARE_CLASS( C_MSS_Player, C_BaseHLPlayer );

	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();


	C_MSS_Player();
	~C_MSS_Player( void );

	void ClientThink( void );

	static C_MSS_Player* GetLocalHL2MPPlayer();
	
	virtual int DrawModel( int flags );
	virtual void AddEntity( void );

	QAngle GetAnimEyeAngles( void ) { return m_angEyeAngles; }
	Vector GetAttackSpread( CBaseCombatWeapon *pWeapon, CBaseEntity *pTarget = NULL );


	// Should this object cast shadows?
	virtual ShadowType_t		ShadowCastType( void );
	virtual C_BaseAnimating *BecomeRagdollOnClient();
	virtual const QAngle& GetRenderAngles();
	virtual bool ShouldDraw( void );
	virtual void OnDataChanged( DataUpdateType_t type );
	virtual float GetFOV( void );
	virtual CStudioHdr *OnNewModel( void );
	virtual void TraceAttack( const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator );
	virtual void ItemPreFrame( void );
	virtual void ItemPostFrame( void );
	virtual float GetMinFOV()	const { return 5.0f; }
	virtual Vector GetAutoaimVector( float flDelta );
	virtual void NotifyShouldTransmit( ShouldTransmitState_t state );
	virtual void CreateLightEffects( void ) {}
	virtual bool ShouldReceiveProjectedTextures( int flags );
	virtual void PostDataUpdate( DataUpdateType_t updateType );
//	virtual void PlayStepSound( Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force ); // BOXBOX removing footstep override
	virtual void PreThink( void );
	virtual void DoImpactEffect( trace_t &tr, int nDamageType );
	IRagdoll* GetRepresentativeRagdoll() const;
	virtual void CalcView( Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov );
	virtual const QAngle& EyeAngles( void );

	
	bool	CanSprint( void );
	void	StartSprinting( void );
	void	StopSprinting( void );
	void	HandleSpeedChanges( void );
	void	UpdateLookAt( void );
	void	Initialize( void );
	int		GetIDTarget() const;
	void	UpdateIDTarget( void );
//	void	PrecacheFootStepSounds( void );
//	const char	*GetPlayerModelSoundPrefix( void );

	MSSPlayerState State_Get() const;

	// Walking
	void StartWalking( void );
	void StopWalking( void );
	bool IsWalking( void ) { return m_fIsWalking; }

	virtual void PostThink( void );

// BOXBOXBOX MSS STUFF

	int				GetNumChars( void ) { return m_bHasCharInSlot[1] + m_bHasCharInSlot[2] + m_bHasCharInSlot[3]; }

	/*struct preloaded_char_info
	{
		char Name[64];
		int model;
	};
	CNetworkArray( preloaded_char_info, m_PreloadedCharInfo, MAX_CHAR_SLOTS );*/
	//int m_PreloadedCharInfo[MAX_CHAR_SLOTS];
//	char m_PreloadedCharInfo_Name[MAX_CHAR_SLOTS][256];
	char m_szPreloadCharName1[ MAX_CHAR_NAME_LENGTH ];
	char m_szPreloadCharName2[ MAX_CHAR_NAME_LENGTH ];
	char m_szPreloadCharName3[ MAX_CHAR_NAME_LENGTH ];

	int m_nPlayerModelIndex;
	int m_nPreloadModelIndex[ MAX_CHAR_SLOTS + 1 ];
	bool m_bHasCharInSlot[ MAX_CHAR_SLOTS + 1 ];
	int m_nCharSlotToCreate; // BOXBOX needed so char create menu knows which slot to create character for

	char			m_szCharName[ MAX_CHAR_NAME_LENGTH ];
	const char		*m_pszCharName;
	unsigned int	m_nGender;
	unsigned int	m_nRace;
	unsigned int	m_nTotalExp;

	unsigned int	m_nGold;

	int m_nMight;
	int m_nAgility;
	int m_nStamina;
	int m_nIntellect;
	int m_nWit;
	int m_nFortitude;
	int m_nLuck;

//	int		m_nWeaponExp[ MAX_WEAPON_SKILLS ];		// BOXBOX these are the experience totals (0-100,000)
	int		m_nWeaponSkills[ MAX_WEAPON_SKILLS ];	// BOXBOX and these are the levels(1-10)
	int		m_nUnarmed;			
	int		m_nOneHandPiercing;
	int		m_nOneHandSlashing;
	int		m_nOneHandBashing;
	int		m_nTwoHandPiercing;
	int		m_nTwoHandSlashing;
	int		m_nTwoHandBashing;
	int		m_nArchery;
	int		m_nThrowingWeapons;

	int		m_nCraftSkills[ MAX_CRAFT_SKILLS ];
	int		m_nAlchemy;
	int		m_nClothwork;
	int		m_nWoodwork;
	int		m_nStonework;
	int		m_nMetalwork;

	int		m_nBackpackSize;

	void	TabulateStats( void ); // BOXBOX Here is where we calculate all the player stats that we don't need to save in the character files



private:
	
	C_MSS_Player( const C_MSS_Player & );

	CPlayerAnimState m_PlayerAnimState;

	QAngle	m_angEyeAngles;

	CInterpolatedVar< QAngle >	m_iv_angEyeAngles;

	EHANDLE	m_hRagdoll;

	int	m_headYawPoseParam;
	int	m_headPitchPoseParam;
	float m_headYawMin;
	float m_headYawMax;
	float m_headPitchMin;
	float m_headPitchMax;

	bool m_isInit;
	Vector m_vLookAtTarget;

	float m_flLastBodyYaw;
	float m_flCurrentHeadYaw;
	float m_flCurrentHeadPitch;

	int	  m_iIDEntIndex;

	CountdownTimer m_blinkTimer;

	int	  m_iSpawnInterpCounter;
	int	  m_iSpawnInterpCounterCache;

//	int	  m_iPlayerSoundType;

	void ReleaseFlashlight( void );
	Beam_t	*m_pFlashlightBeam;

	CNetworkVar( MSSPlayerState, m_iPlayerState );	

	bool m_fIsWalking;
};

inline C_MSS_Player *ToMSSPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<C_MSS_Player*>( pEntity );
}


class C_HL2MPRagdoll : public C_BaseAnimatingOverlay
{
public:
	DECLARE_CLASS( C_HL2MPRagdoll, C_BaseAnimatingOverlay );
	DECLARE_CLIENTCLASS();
	
	C_HL2MPRagdoll();
	~C_HL2MPRagdoll();

	virtual void OnDataChanged( DataUpdateType_t type );

	int GetPlayerEntIndex() const;
	IRagdoll* GetIRagdoll() const;

	void ImpactTrace( trace_t *pTrace, int iDamageType, const char *pCustomImpactName );
	void UpdateOnRemove( void );
	virtual void SetupWeights( const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights );
	
private:
	
	C_HL2MPRagdoll( const C_HL2MPRagdoll & ) {}

	void Interp_Copy( C_BaseAnimatingOverlay *pDestinationEntity );
	void CreateHL2MPRagdoll( void );

private:

	EHANDLE	m_hPlayer;
	CNetworkVector( m_vecRagdollVelocity );
	CNetworkVector( m_vecRagdollOrigin );
};

#endif //HL2MP_PLAYER_H
