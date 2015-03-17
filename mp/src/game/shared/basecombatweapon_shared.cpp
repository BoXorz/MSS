
#include "cbase.h"
#include "in_buttons.h"
#include "engine/IEngineSound.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "physics_saverestore.h"
#include "datacache/imdlcache.h"
#include "activitylist.h"
#include "haptics/haptic_utils.h"

#include "MSS_shareddefs.h"

#ifdef CLIENT_DLL
	#include "prediction.h"
#endif

#if !defined( CLIENT_DLL )
#include "soundent.h"
#include "eventqueue.h"
#include "fmtstr.h"
#include "gameweaponmanager.h"
#include "te_effect_dispatch.h"
#include "ilagcompensationmanager.h"
#include "MSS_gamerules.h"

#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/* BOXBOX removing
#define MIN_HUDHINT_DISPLAY_TIME 7.0f
*/

#define HIDEWEAPON_THINK_CONTEXT			"BaseCombatWeapon_HideThink"

extern bool UTIL_ItemCanBeTouchedByPlayer( CBaseEntity *pItem, CBasePlayer *pPlayer );

/* BOXBOX removing
#if defined ( TF_CLIENT_DLL ) || defined ( TF_DLL )
#ifdef _DEBUG
ConVar tf_weapon_criticals_force_random( "tf_weapon_criticals_force_random", "0", FCVAR_REPLICATED | FCVAR_CHEAT );
#endif // _DEBUG
ConVar tf_weapon_criticals_bucket_cap( "tf_weapon_criticals_bucket_cap", "1000.0", FCVAR_REPLICATED | FCVAR_CHEAT );
ConVar tf_weapon_criticals_bucket_bottom( "tf_weapon_criticals_bucket_bottom", "-250.0", FCVAR_REPLICATED | FCVAR_CHEAT );
ConVar tf_weapon_criticals_bucket_default( "tf_weapon_criticals_bucket_default", "300.0", FCVAR_REPLICATED | FCVAR_CHEAT );
#endif // TF
*/

LINK_ENTITY_TO_CLASS( weapon_unarmed, CBaseCombatWeapon ); // BOXBOX the always existing "weapon"

#ifndef CLIENT_DLL

acttable_t	CBaseCombatWeapon::m_acttable[] = 
{
	{ ACT_RANGE_ATTACK1,				ACT_RANGE_ATTACK_SLAM, true },
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_MELEE,					false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_MELEE,					false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_MELEE,			false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_MELEE,			false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_MELEE,			false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_MELEE,					false },
};

IMPLEMENT_ACTTABLE( CBaseCombatWeapon );

#endif

/*
acttable_t*	CBaseCombatWeapon::ActivityList( void )
{
	return NULL;
}

int	CBaseCombatWeapon::ActivityListCount( void )
{
	return 0;
}
*/

CBaseCombatWeapon::CBaseCombatWeapon()
{
	// Constructor must call this
	// CONSTRUCT_PREDICTABLE( CBaseCombatWeapon );

	// Some default values.  There should be set in the particular weapon classes
	m_fMinRange1		= 1;
	m_fMinRange2		= 1;
	m_fMaxRange1		= 64;
	m_fMaxRange2		= 64;

//	m_bReloadsSingly	= false;

	// Defaults to zero
	m_nViewModelIndex	= 0;

	m_bFlipViewModel	= false;

#if defined( CLIENT_DLL )
	m_iState = m_iOldState = WEAPON_NOT_CARRIED;
/* BOXBOX removing
	m_iClip1 = -1;
	m_iClip2 = -1;
	m_iPrimaryAmmoType = -1;
	m_iSecondaryAmmoType = -1;
*/
#endif

#if !defined( CLIENT_DLL )
	m_pConstraint = NULL;
	OnBaseCombatWeaponCreated( this );
#endif

	m_hItemFileInfo = GetInvalidItemInfoHandle();
/* BOXBOX removing
#if defined( TF_DLL )
	UseClientSideAnimation();
#endif

#if defined ( TF_CLIENT_DLL ) || defined ( TF_DLL )
	m_flCritTokenBucket = tf_weapon_criticals_bucket_default.GetFloat();
	m_nCritChecks = 1;
	m_nCritSeedRequests = 0;
#endif // TF
*/
}


CBaseCombatWeapon::~CBaseCombatWeapon( void )
{
#if !defined( CLIENT_DLL )
	//Remove our constraint, if we have one
	if ( m_pConstraint != NULL )
	{
		physenv->DestroyConstraint( m_pConstraint );
		m_pConstraint = NULL;
	}
	OnBaseCombatWeaponDestroyed( this );
#endif
}

void CBaseCombatWeapon::Activate( void )
{
	BaseClass::Activate();

#ifndef CLIENT_DLL
	if ( GetOwnerEntity() )
		return;
/* BOXBOX removing
	if ( g_pGameRules->IsAllowedToSpawn( this ) == false )
	{
		UTIL_Remove( this );
		return;
	}
*/
#endif
}

/* BOXBOX removing
void CBaseCombatWeapon::GiveDefaultAmmo( void )
{
	// If I use clips, set my clips to the default
	if ( UsesClipsForAmmo1() )
	{
		m_iClip1 = AutoFiresFullClip() ? 0 : GetDefaultClip1();
	}
	else
	{
		SetPrimaryAmmoCount( GetDefaultClip1() );
		m_iClip1 = WEAPON_NOCLIP;
	}
	if ( UsesClipsForAmmo2() )
	{
		m_iClip2 = GetDefaultClip2();
	}
	else
	{
		SetSecondaryAmmoCount( GetDefaultClip2() );
		m_iClip2 = WEAPON_NOCLIP;
	}
}
*/

//-----------------------------------------------------------------------------
// Purpose: Set mode to world model and start falling to the ground
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::Spawn( void )
{
	Precache();

	BaseClass::Spawn();

	SetSolid( SOLID_BBOX );
//	m_flNextEmptySoundTime = 0.0f;

	// Weapons won't show up in trace calls if they are being carried...
	RemoveEFlags( EFL_USE_PARTITION_WHEN_NOT_SOLID );

	m_iState = WEAPON_NOT_CARRIED;

	m_nViewModelIndex = 0;

//	GiveDefaultAmmo();

	if ( GetWorldModel() )
	{
		SetModel( GetWorldModel() );
	}

#if !defined( CLIENT_DLL )
/* BOXBOX removing
	if( IsX360() )
	{
		AddEffects( EF_ITEM_BLINK );
	}
*/
	FallInit();
	SetCollisionGroup( COLLISION_GROUP_WEAPON );
	m_takedamage = DAMAGE_EVENTS_ONLY;

	SetBlocksLOS( false );

	SetRemoveable( false );
#endif

	// BOXBOX removing, this line would bloat the box for player pickup
//	CollisionProp()->UseTriggerBounds( true, 36 );

	// Use more efficient bbox culling on the client. Otherwise, it'll setup bones for most
	// characters even when they're not in the frustum.
	AddEffects( EF_BONEMERGE_FASTCULL );
/* BOXBOX removing
	m_iReloadHudHintCount = 0;
	m_iAltFireHudHintCount = 0;
	m_flHudHintMinDisplayTime = 0;
*/
}

// BOXBOX get encryption key for decoding item kv files
const unsigned char *CBaseCombatWeapon::GetEncryptionKey( void ) 
{ 
	return g_pGameRules->GetEncryptionKey(); 
}


void CBaseCombatWeapon::Precache( void )
{
#if defined( CLIENT_DLL )
	Assert( Q_strlen( GetClassname() ) > 0 );
#endif

//	m_iPrimaryAmmoType = m_iSecondaryAmmoType = -1;

	// Add this weapon to the item registry, and get our index into it
	// Get item data from script file
	if ( ReadItemDataFromFile( filesystem, GetClassname(), &m_hItemFileInfo, GetEncryptionKey() ) )
	{

/* BOXBOX removing
		if ( GetWpnData().szAmmo1[0] )
		{
			m_iPrimaryAmmoType = GetAmmoDef()->Index( GetWpnData().szAmmo1 );
			if (m_iPrimaryAmmoType == -1)
			{
				Msg("ERROR: Weapon (%s) using undefined primary ammo type (%s)\n",GetClassname(), GetWpnData().szAmmo1);
			}
 #if defined ( TF_DLL ) || defined ( TF_CLIENT_DLL )
			// Ammo override
			int iModUseMetalOverride = 0;
			CALL_ATTRIB_HOOK_INT( iModUseMetalOverride, mod_use_metal_ammo_type );
			if ( iModUseMetalOverride )
			{
				m_iPrimaryAmmoType = (int)TF_AMMO_METAL;
			}
#endif
 		}
		if ( GetWpnData().szAmmo2[0] )
		{
			m_iSecondaryAmmoType = GetAmmoDef()->Index( GetWpnData().szAmmo2 );
			if (m_iSecondaryAmmoType == -1)
			{
				Msg("ERROR: Weapon (%s) using undefined secondary ammo type (%s)\n",GetClassname(),GetWpnData().szAmmo2);
			}
		}

#if defined( CLIENT_DLL )
		gWR.LoadWeaponSprites( GetItemFileInfoHandle() );
#endif
*/
		// Precache models (preload to avoid hitch)
		m_iViewModelIndex = 0;
		m_iWorldModelIndex = 0;
		if ( GetViewModel() && GetViewModel()[0] )
		{
			m_iViewModelIndex = CBaseEntity::PrecacheModel( GetViewModel() );
		}
		if ( GetWorldModel() && GetWorldModel()[0] )
		{
			m_iWorldModelIndex = CBaseEntity::PrecacheModel( GetWorldModel() );
		}

		// Precache sounds, too
		for ( int i = 0; i < NUM_WPN_SOUND_TYPES; ++i )
		{
			const char *wpnsound = GetWpnSound( i );
			if ( wpnsound && wpnsound[0] )
			{
				CBaseEntity::PrecacheScriptSound( wpnsound );
			}
		}
	}
	else
	{
		// Couldn't read data file, remove myself
		Warning( "Error reading item data file for: %s\n", GetClassname() );
	//	Remove( );	//don't remove, this gets released soon!
	}
}

//-----------------------------------------------------------------------------
// Purpose: Get my data in the file item info array
//-----------------------------------------------------------------------------
const FileItemInfo_t &CBaseCombatWeapon::GetItemData( void ) const
{
	return *GetFileItemInfoFromHandle( m_hItemFileInfo );
}

const char *CBaseCombatWeapon::GetName( void ) const
{
	return GetItemData().szClassName;
}

const char *CBaseCombatWeapon::GetPrintName( void ) const
{
	return GetItemData().szPrintName;
}

const char *CBaseCombatWeapon::GetWorldModel( void ) const
{
	return GetItemData().szWorldModel;
}

// BOXBOX TODO add MSS stuff here

const char *CBaseCombatWeapon::GetViewModel( int /*viewmodelindex = 0 -- this is ignored in the base class here*/ ) const
{
	return GetItemData().szViewModel;
}

const char *CBaseCombatWeapon::GetAnimPrefix( void ) const
{
	return GetItemData().szAnimationPrefix;
}

int CBaseCombatWeapon::GetItemType( void ) const
{
	return GetItemData().nItemType;
}

int CBaseCombatWeapon::GetWeaponType( void ) const
{
	return GetItemData().nWeaponType;
}

int CBaseCombatWeapon::GetItemLevel( void ) const
{
	return GetItemData().nItemLevel;
}

/* BOXBOX removing
int CBaseCombatWeapon::GetMaxClip1( void ) const
{
	return 0;
}

int CBaseCombatWeapon::GetMaxClip2( void ) const
{
	return 0;
}

int CBaseCombatWeapon::GetDefaultClip1( void ) const
{
	return 0;
}

int CBaseCombatWeapon::GetDefaultClip2( void ) const
{
	return 0;
}

bool CBaseCombatWeapon::UsesClipsForAmmo1( void ) const
{
	return false;
}

bool CBaseCombatWeapon::IsMeleeWeapon() const
{
	return true;
}

bool CBaseCombatWeapon::UsesClipsForAmmo2( void ) const
{
	return false;
}

int CBaseCombatWeapon::GetWeight( void ) const
{
	return 0;
}

bool CBaseCombatWeapon::AllowsAutoSwitchTo( void ) const
{
	return 0;
}

bool CBaseCombatWeapon::AllowsAutoSwitchFrom( void ) const
{
	return 0;
}

int CBaseCombatWeapon::GetWeaponFlags( void ) const
{
	return 0;
}

int CBaseCombatWeapon::GetSlot( void ) const
{
	return 0;
}

int CBaseCombatWeapon::GetPosition( void ) const
{
	return 0;
}
*/

/* BOXBOX removing
CHudTexture const *CBaseCombatWeapon::GetSpriteActive( void ) const
{
	return GetWpnData().iconActive;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudTexture const *CBaseCombatWeapon::GetSpriteInactive( void ) const
{
	return GetWpnData().iconInactive;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudTexture const *CBaseCombatWeapon::GetSpriteAmmo( void ) const
{
	return GetWpnData().iconAmmo;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudTexture const *CBaseCombatWeapon::GetSpriteAmmo2( void ) const
{
	return GetWpnData().iconAmmo2;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudTexture const *CBaseCombatWeapon::GetSpriteCrosshair( void ) const
{
	return GetWpnData().iconCrosshair;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudTexture const *CBaseCombatWeapon::GetSpriteAutoaim( void ) const
{
	return GetWpnData().iconAutoaim;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudTexture const *CBaseCombatWeapon::GetSpriteZoomedCrosshair( void ) const
{
	return GetWpnData().iconZoomedCrosshair;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudTexture const *CBaseCombatWeapon::GetSpriteZoomedAutoaim( void ) const
{
	return GetWpnData().iconZoomedAutoaim;
}
*/


const char *CBaseCombatWeapon::GetWpnSound( int iIndex ) const
{
	return GetItemData().aWpnSounds[ iIndex ];
}

/*
int CBaseCombatWeapon::GetRumbleEffect() const
{
	return GetItemData().iRumbleEffect;
}
*/

CBaseCombatCharacter* CBaseCombatWeapon::GetOwner() const
{
	return ToBaseCombatCharacter( m_hOwner.Get() );
}	
CBasePlayer* CBaseCombatWeapon::GetPlayerOwner() const
{
	return dynamic_cast< CBasePlayer* >( GetOwner() );
}

/* BOXBOX ERROR C2680
CMSS_Player* CBaseCombatWeapon::GetMSSPlayerOwner() const
{
	return dynamic_cast< CMSS_Player* >( GetOwner() );
}
*/

void CBaseCombatWeapon::SetOwner( CBaseCombatCharacter *owner )
{
	if ( !owner )
	{ 
#ifndef CLIENT_DLL
		// Make sure the weapon updates its state when it's removed from the player
		// We have to force an active state change, because it's being dropped and won't call UpdateClientData()
		int iOldState = m_iState;
		m_iState = WEAPON_NOT_CARRIED;
		OnActiveStateChanged( iOldState );
#endif

		// make sure we clear out our HideThink if we have one pending
		SetContextThink( NULL, 0, HIDEWEAPON_THINK_CONTEXT );
	}

	m_hOwner = owner;
	
#ifndef CLIENT_DLL
	DispatchUpdateTransmitState();
#else
	UpdateVisibility();
#endif
}

/* BOXBOX removing
bool CBaseCombatWeapon::IsAllowedToSwitch( void )
{
	return true;
}

bool CBaseCombatWeapon::CanBeSelected( void )
{
	if ( !VisibleInWeaponSelection() )
		return false;

	return HasAmmo();
}

bool CBaseCombatWeapon::HasAmmo( void )
{
	// Weapons with no ammo types can always be selected
	if ( m_iPrimaryAmmoType == -1 && m_iSecondaryAmmoType == -1  )
		return true;
	if ( GetWeaponFlags() & ITEM_FLAG_SELECTONEMPTY )
		return true;

	CBasePlayer *player = ToBasePlayer( GetOwner() );
	if ( !player )
		return false;
	return ( m_iClip1 > 0 || player->GetAmmoCount( m_iPrimaryAmmoType ) || m_iClip2 > 0 || player->GetAmmoCount( m_iSecondaryAmmoType ) );
}

bool CBaseCombatWeapon::VisibleInWeaponSelection( void )
{
	return true;
}
*/

bool CBaseCombatWeapon::HasWeaponIdleTimeElapsed( void )
{
	if ( gpGlobals->curtime > m_flTimeWeaponIdle )
		return true;

	return false;
}

void CBaseCombatWeapon::SetWeaponIdleTime( float time )
{
	m_flTimeWeaponIdle = time;
}

float CBaseCombatWeapon::GetWeaponIdleTime( void )
{
	return m_flTimeWeaponIdle;
}

void CBaseCombatWeapon::Drop( const Vector &vecVelocity ) // BOXBOX TODO add Throw Weapon
{
#if !defined( CLIENT_DLL )


	SetRemoveable( false ); // BOXBOX changed to false
//	WeaponManager_AmmoMod( this ); // BOXBOX removed

	//If it was dropped then there's no need to respawn it.
	AddSpawnFlags( SF_NORESPAWN );

	StopAnimation();
	StopFollowingEntity( );
	SetMoveType( MOVETYPE_FLYGRAVITY );
	// clear follow stuff, setup for collision
	SetGravity(1.0);
	m_iState = WEAPON_NOT_CARRIED;
	RemoveEffects( EF_NODRAW );
	AddEffects( EF_ITEM_BLINK ); // BOXBOX TODO adding this, but maybe later change it to highlight using code from estranged?
	FallInit();
	SetGroundEntity( NULL );
	SetThink( &CBaseCombatWeapon::SetPickupTouch );
	SetTouch(NULL);

	if( hl2_episodic.GetBool() )
	{
		RemoveSpawnFlags( SF_WEAPON_NO_PLAYER_PICKUP );
	}

	IPhysicsObject *pObj = VPhysicsGetObject();
	if ( pObj != NULL )
	{
		AngularImpulse	angImp( 200, 200, 200 );
		pObj->AddVelocity( &vecVelocity, &angImp );
	}
	else
	{
		SetAbsVelocity( vecVelocity );
	}

//	CBaseEntity *pOwner = GetOwnerEntity();

	SetNextThink( gpGlobals->curtime + 1.0f );
	SetOwnerEntity( NULL );
	SetOwner( NULL );

	// If we're not allowing to spawn due to the gamerules,
	// remove myself when I'm dropped by an NPC.

/* BOXBOX removing this for MSS
	if ( pOwner && pOwner->IsNPC() )
	{
		if ( g_pGameRules->IsAllowedToSpawn( this ) == false )
		{
			UTIL_Remove( this );
			return;
		}
	}
*/

#endif
}


void CBaseCombatWeapon::OnPickedUp( CBaseCombatCharacter *pNewOwner ) // BOXBOX TODO if weapon is better than current, switch?
{
#if !defined( CLIENT_DLL )
	RemoveEffects( EF_ITEM_BLINK );

	if( pNewOwner->IsPlayer() )
	{
		m_OnPlayerPickup.FireOutput(pNewOwner, this);

		// Play the pickup sound for 1st-person observers
		CRecipientFilter filter;
		for ( int i=1; i <= gpGlobals->maxClients; ++i )
		{
			CBasePlayer *player = UTIL_PlayerByIndex(i);
			if ( player && !player->IsAlive() && player->GetObserverMode() == OBS_MODE_IN_EYE )
			{
				filter.AddRecipient( player );
			}
		}
		if ( filter.GetRecipientCount() )
		{
//			CBaseEntity::EmitSound( filter, pNewOwner->entindex(), "Player.PickupWeapon" ); // BOXBOX TODO set up pickup sounds based on item type!
		}

		SetName( NULL_STRING ); // BOXBOX TODO do we want to do this?
	}
	else
	{
		m_OnNPCPickup.FireOutput(pNewOwner, this);
	}

	SetRemoveable( false );

//	pNewOwner->SwitchToNextBestWeapon( pNewOwner->GetActiveWeapon() ); // BOXBOX TODO figure out how we want to deal with picking up better weapons

#endif
}




#if defined( CLIENT_DLL )

#define	HL2_BOB_CYCLE_MIN	1.0f
#define	HL2_BOB_CYCLE_MAX	0.45f
#define	HL2_BOB			0.002f
#define	HL2_BOB_UP		0.5f

//extern float	g_lateralBob;
//extern float	g_verticalBob;

float	g_lateralBob;
float	g_verticalBob;

static ConVar	cl_bobcycle( "cl_bobcycle","0.8" );
static ConVar	cl_bob( "cl_bob","0.002" );
static ConVar	cl_bobup( "cl_bobup","0.5" );

// Register these cvars if needed for easy tweaking
static ConVar	v_iyaw_cycle( "v_iyaw_cycle", "2", FCVAR_REPLICATED | FCVAR_CHEAT );
static ConVar	v_iroll_cycle( "v_iroll_cycle", "0.5", FCVAR_REPLICATED | FCVAR_CHEAT );
static ConVar	v_ipitch_cycle( "v_ipitch_cycle", "1", FCVAR_REPLICATED | FCVAR_CHEAT );
static ConVar	v_iyaw_level( "v_iyaw_level", "0.3", FCVAR_REPLICATED | FCVAR_CHEAT );
static ConVar	v_iroll_level( "v_iroll_level", "0.1", FCVAR_REPLICATED | FCVAR_CHEAT );
static ConVar	v_ipitch_level( "v_ipitch_level", "0.3", FCVAR_REPLICATED | FCVAR_CHEAT );


float CBaseCombatWeapon::CalcViewmodelBob( void )
{
	static	float bobtime;
	static	float lastbobtime;
	float	cycle;
	
	CBasePlayer *player = ToBasePlayer( GetOwner() );
	//Assert( player );

	//NOTENOTE: For now, let this cycle continue when in the air, because it snaps badly without it

	if ( ( !gpGlobals->frametime ) || ( player == NULL ) )
	{
		//NOTENOTE: We don't use this return value in our case (need to restructure the calculation function setup!)
		return 0.0f;// just use old value
	}

	//Find the speed of the player
	float speed = player->GetLocalVelocity().Length2D();

	//FIXME: This maximum speed value must come from the server.
	//		 MaxSpeed() is not sufficient for dealing with sprinting - jdw

	speed = clamp( speed, -320, 320 );

	float bob_offset = RemapVal( speed, 0, 320, 0.0f, 1.0f );
	
	bobtime += ( gpGlobals->curtime - lastbobtime ) * bob_offset;
	lastbobtime = gpGlobals->curtime;

	//Calculate the vertical bob
	cycle = bobtime - (int)(bobtime/HL2_BOB_CYCLE_MAX)*HL2_BOB_CYCLE_MAX;
	cycle /= HL2_BOB_CYCLE_MAX;

	if ( cycle < HL2_BOB_UP )
	{
		cycle = M_PI * cycle / HL2_BOB_UP;
	}
	else
	{
		cycle = M_PI + M_PI*(cycle-HL2_BOB_UP)/(1.0 - HL2_BOB_UP);
	}
	
	g_verticalBob = speed*0.005f;
	g_verticalBob = g_verticalBob*0.3 + g_verticalBob*0.7*sin(cycle);

	g_verticalBob = clamp( g_verticalBob, -7.0f, 4.0f );

	//Calculate the lateral bob
	cycle = bobtime - (int)(bobtime/HL2_BOB_CYCLE_MAX*2)*HL2_BOB_CYCLE_MAX*2;
	cycle /= HL2_BOB_CYCLE_MAX*2;

	if ( cycle < HL2_BOB_UP )
	{
		cycle = M_PI * cycle / HL2_BOB_UP;
	}
	else
	{
		cycle = M_PI + M_PI*(cycle-HL2_BOB_UP)/(1.0 - HL2_BOB_UP);
	}

	g_lateralBob = speed*0.005f;
	g_lateralBob = g_lateralBob*0.3 + g_lateralBob*0.7*sin(cycle);
	g_lateralBob = clamp( g_lateralBob, -7.0f, 4.0f );
	
	//NOTENOTE: We don't use this return value in our case (need to restructure the calculation function setup!)
	return 0.0f;
}

void CBaseCombatWeapon::AddViewmodelBob( CBaseViewModel *viewmodel, Vector &origin, QAngle &angles )
{
	Vector	forward, right;
	AngleVectors( angles, &forward, &right, NULL );

	CalcViewmodelBob();

	// Apply bob, but scaled down to 40%
	VectorMA( origin, g_verticalBob * 0.1f, forward, origin );
	
	// Z bob a bit more
	origin[2] += g_verticalBob * 0.1f;
	
	// bob the angles
	angles[ ROLL ]	+= g_verticalBob * 0.5f;
	angles[ PITCH ]	-= g_verticalBob * 0.4f;

	angles[ YAW ]	-= g_lateralBob  * 0.3f;

	VectorMA( origin, g_lateralBob * 0.8f, right, origin );
}

#else // Server stubs
float CBaseCombatWeapon::CalcViewmodelBob( void )
{
	return 0.0f;
}

void CBaseCombatWeapon::AddViewmodelBob( CBaseViewModel *viewmodel, Vector &origin, QAngle &angles )
{
}

#endif



/* BOXBOX removing
void CBaseCombatWeapon::MakeTracer( const Vector &vecTracerSrc, const trace_t &tr, int iTracerType )
{
	CBaseEntity *pOwner = GetOwner();

	if ( pOwner == NULL )
	{
		BaseClass::MakeTracer( vecTracerSrc, tr, iTracerType );
		return;
	}

	const char *pszTracerName = GetTracerType();

	Vector vNewSrc = vecTracerSrc;
	int iEntIndex = pOwner->entindex();

	if ( g_pGameRules->IsMultiplayer() )
	{
		iEntIndex = entindex();
	}

	int iAttachment = GetTracerAttachment();

	switch ( iTracerType )
	{
	case TRACER_LINE:
		UTIL_Tracer( vNewSrc, tr.endpos, iEntIndex, iAttachment, 0.0f, true, pszTracerName );
		break;

	case TRACER_LINE_AND_WHIZ:
		UTIL_Tracer( vNewSrc, tr.endpos, iEntIndex, iAttachment, 0.0f, true, pszTracerName );
		break;
	}
}
*/

void CBaseCombatWeapon::GiveTo( CBaseEntity *pOther )
{
	DefaultTouch( pOther );
}

void CBaseCombatWeapon::DefaultTouch( CBaseEntity *pOther )
{
#if !defined( CLIENT_DLL )
	// Can't pick up dissolving weapons
	if ( IsDissolving() )
		return;

	// if it's not a player, ignore
	CBasePlayer *pPlayer = ToBasePlayer(pOther);
	if ( !pPlayer )
		return;

	if( UTIL_ItemCanBeTouchedByPlayer(this, pPlayer) )
	{
		// This makes sure the player could potentially take the object
		// before firing the cache interaction output. That doesn't mean
		// the player WILL end up taking the object, but cache interactions
		// are fired as soon as you prove you have found the object, not
		// when you finally acquire it.
		m_OnCacheInteraction.FireOutput( pOther, this );
	}

	if( HasSpawnFlags(SF_WEAPON_NO_PLAYER_PICKUP) )
		return;

	if (pPlayer->BumpWeapon(this))
	{
		OnPickedUp( pPlayer );
	}
#endif
}

/* BOXBOX removing
bool CBaseCombatWeapon::ShouldDisplayAltFireHUDHint()
{
	if( m_iAltFireHudHintCount >= WEAPON_RELOAD_HUD_HINT_COUNT )
		return false;

	if( UsesSecondaryAmmo() && HasSecondaryAmmo() )
	{
		return true;
	}

	if( !UsesSecondaryAmmo() && HasPrimaryAmmo() )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::DisplayAltFireHudHint()
{
#if !defined( CLIENT_DLL )
	CFmtStr hint;
	hint.sprintf( "#valve_hint_alt_%s", GetClassname() );
	UTIL_HudHintText( GetOwner(), hint.Access() );
	m_iAltFireHudHintCount++;
	m_bAltFireHudHintDisplayed = true;
	m_flHudHintMinDisplayTime = gpGlobals->curtime + MIN_HUDHINT_DISPLAY_TIME;
#endif//CLIENT_DLL
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::RescindAltFireHudHint()
{
#if !defined( CLIENT_DLL )
	Assert(m_bAltFireHudHintDisplayed);
	
	UTIL_HudHintText( GetOwner(), "" );
	--m_iAltFireHudHintCount;
	m_bAltFireHudHintDisplayed = false;
#endif//CLIENT_DLL
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CBaseCombatWeapon::ShouldDisplayReloadHUDHint()
{
	if( m_iReloadHudHintCount >= WEAPON_RELOAD_HUD_HINT_COUNT )
		return false;

	CBaseCombatCharacter *pOwner = GetOwner();

	if( pOwner != NULL && pOwner->IsPlayer() && UsesClipsForAmmo1() && m_iClip1 < (GetMaxClip1() / 2) )
	{
		// I'm owned by a player, I use clips, I have less then half a clip loaded. Now, does the player have more ammo?
		if ( pOwner )
		{
			if ( pOwner->GetAmmoCount( m_iPrimaryAmmoType ) > 0 ) 
				return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::DisplayReloadHudHint()
{
#if !defined( CLIENT_DLL )
	UTIL_HudHintText( GetOwner(), "valve_hint_reload" );
	m_iReloadHudHintCount++;
	m_bReloadHudHintDisplayed = true;
	m_flHudHintMinDisplayTime = gpGlobals->curtime + MIN_HUDHINT_DISPLAY_TIME;
#endif//CLIENT_DLL
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::RescindReloadHudHint()
{
#if !defined( CLIENT_DLL )
	Assert(m_bReloadHudHintDisplayed);

	UTIL_HudHintText( GetOwner(), "" );
	--m_iReloadHudHintCount;
	m_bReloadHudHintDisplayed = false;
#endif//CLIENT_DLL
}
*/

void CBaseCombatWeapon::SetPickupTouch( void )
{
#if !defined( CLIENT_DLL )
	SetTouch(&CBaseCombatWeapon::DefaultTouch);
/* BOXBOX TODO make sure commenting this out stops weapons from dying after 30 seconds on ground
	if ( gpGlobals->maxClients > 1 )
	{
		if ( GetSpawnFlags() & SF_NORESPAWN )
		{
			SetThink( &CBaseEntity::SUB_Remove );
			SetNextThink( gpGlobals->curtime + 30.0f );
		}
	}
*/
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Become a child of the owner (MOVETYPE_FOLLOW)
//			disables collisions, touch functions, thinking
// Input  : *pOwner - new owner/operator
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::Equip( CBaseCombatCharacter *pOwner )
{
	// Attach the weapon to an owner
	SetAbsVelocity( vec3_origin );
	RemoveSolidFlags( FSOLID_TRIGGER );
	FollowEntity( pOwner );
	SetOwner( pOwner );
	SetOwnerEntity( pOwner );
	RemoveEffects( EF_ITEM_BLINK );

	// Break any constraint I might have to the world.
#if !defined( CLIENT_DLL )
	if ( m_pConstraint != NULL )
	{
		RemoveSpawnFlags( SF_WEAPON_START_CONSTRAINED );
		physenv->DestroyConstraint( m_pConstraint );
		m_pConstraint = NULL;
	}
#endif

	m_flNextPrimaryAttack		= gpGlobals->curtime;
	m_flNextSecondaryAttack		= gpGlobals->curtime;
	SetTouch( NULL );
	SetThink( NULL );
#if !defined( CLIENT_DLL )
	VPhysicsDestroyObject();
#endif

	if ( pOwner->IsPlayer() )
	{
		SetModel( GetViewModel() );
	}
	else
	{
		// Make the weapon ready as soon as any NPC picks it up.
		m_flNextPrimaryAttack = gpGlobals->curtime;
		m_flNextSecondaryAttack = gpGlobals->curtime;
		SetModel( GetWorldModel() );
	}
}

void CBaseCombatWeapon::SetActivity( Activity act, float duration ) 
{ 
#if !defined( CLIENT_DLL ) && (defined( MSS ) || defined( PORTAL ))
	SetModel( GetWorldModel() );
#endif
	
	int sequence = SelectWeightedSequence( act ); 
	
	// FORCE IDLE on sequences we don't have (which should be many)
	if ( sequence == ACTIVITY_NOT_AVAILABLE )
		sequence = SelectWeightedSequence( ACT_VM_IDLE );

#if !defined( CLIENT_DLL ) && (defined( MSS ) || defined( PORTAL ))
	SetModel( GetViewModel() );
#endif

	if ( sequence != ACTIVITY_NOT_AVAILABLE )
	{
		SetSequence( sequence );
		SetActivity( act ); 
		SetCycle( 0 );
		ResetSequenceInfo( );

		if ( duration > 0 )
		{
			// FIXME: does this even make sense in non-shoot animations?
			m_flPlaybackRate = SequenceDuration( sequence ) / duration;
			m_flPlaybackRate = MIN( m_flPlaybackRate, 12.0);  // FIXME; magic number!, network encoding range
		}
		else
		{
			m_flPlaybackRate = 1.0;
		}
	}
}

//====================================================================================
// WEAPON CLIENT HANDLING
//====================================================================================
int CBaseCombatWeapon::UpdateClientData( CBasePlayer *pPlayer )
{
	int iNewState = WEAPON_IS_CARRIED_BY_PLAYER;

	if ( pPlayer->GetActiveWeapon() == this )
	{
		if ( pPlayer->m_fOnTarget ) 
		{
			iNewState = WEAPON_IS_ONTARGET;
		}
		else
		{
			iNewState = WEAPON_IS_ACTIVE;
		}
	}
	else
	{
		iNewState = WEAPON_IS_CARRIED_BY_PLAYER;
	}

	if ( m_iState != iNewState )
	{
		int iOldState = m_iState;
		m_iState = iNewState;
		OnActiveStateChanged( iOldState );
	}
	return 1;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : index - 
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::SetViewModelIndex( int index )
{
	Assert( index >= 0 && index < MAX_VIEWMODELS );
	m_nViewModelIndex = index;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iActivity - 
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::SendViewModelAnim( int nSequence )
{
#if defined( CLIENT_DLL )
	if ( !IsPredicted() )
		return;
#endif
	
	if ( nSequence < 0 )
		return;

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;
	
	CBaseViewModel *vm = pOwner->GetViewModel( m_nViewModelIndex, false );
	
	if ( vm == NULL )
		return;

	SetViewModel();
	Assert( vm->ViewModelIndex() == m_nViewModelIndex );
	vm->SendViewModelMatchingSequence( nSequence );
}

float CBaseCombatWeapon::GetViewModelSequenceDuration()
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner == NULL )
	{
		Assert( false );
		return 0;
	}
	
	CBaseViewModel *vm = pOwner->GetViewModel( m_nViewModelIndex );
	if ( vm == NULL )
	{
		Assert( false );
		return 0;
	}

	SetViewModel();
	Assert( vm->ViewModelIndex() == m_nViewModelIndex );
	return vm->SequenceDuration();
}

bool CBaseCombatWeapon::IsViewModelSequenceFinished( void )
{
	// These are not valid activities and always complete immediately
	if ( GetActivity() == ACT_RESET || GetActivity() == ACT_INVALID )
		return true;

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner == NULL )
	{
		Assert( false );
		return false;
	}
	
	CBaseViewModel *vm = pOwner->GetViewModel( m_nViewModelIndex );
	if ( vm == NULL )
	{
		Assert( false );
		return false;
	}

	return vm->IsSequenceFinished();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::SetViewModel()
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner == NULL )
		return;
	CBaseViewModel *vm = pOwner->GetViewModel( m_nViewModelIndex, false );
	if ( vm == NULL )
		return;
	Assert( vm->ViewModelIndex() == m_nViewModelIndex );
	vm->SetWeaponModel( GetViewModel( m_nViewModelIndex ), this );
}

//-----------------------------------------------------------------------------
// Purpose: Set the desired activity for the weapon and its viewmodel counterpart
// Input  : iActivity - activity to play
//-----------------------------------------------------------------------------
bool CBaseCombatWeapon::SendWeaponAnim( int iActivity )
{
/* BOXBOX removing
#ifdef USES_ECON_ITEMS
	iActivity = TranslateViewmodelHandActivity( (Activity)iActivity );
#endif
*/
	// NVNT notify the haptics system of this weapons new activity
#ifdef WIN32
#ifdef CLIENT_DLL
	if ( prediction->InPrediction() && prediction->IsFirstTimePredicted() )
#endif
#ifndef _X360
		HapticSendWeaponAnim(this,iActivity);
#endif
#endif
	//For now, just set the ideal activity and be done with it
	return SetIdealActivity( (Activity) iActivity );
}

/* BOXBOX removing
bool CBaseCombatWeapon::HasAnyAmmo( void )
{
	// If I don't use ammo of any kind, I can always fire
	if ( !UsesPrimaryAmmo() && !UsesSecondaryAmmo() )
		return true;

	// Otherwise, I need ammo of either type
	return ( HasPrimaryAmmo() || HasSecondaryAmmo() );
}


bool CBaseCombatWeapon::HasPrimaryAmmo( void )
{
	// If I use a clip, and have some ammo in it, then I have ammo
	if ( UsesClipsForAmmo1() )
	{
		if ( m_iClip1 > 0 )
			return true;
	}

	// Otherwise, I have ammo if I have some in my ammo counts
	CBaseCombatCharacter		*pOwner = GetOwner();
	if ( pOwner )
	{
		if ( pOwner->GetAmmoCount( m_iPrimaryAmmoType ) > 0 ) 
			return true;
	}
	else
	{
		// No owner, so return how much primary ammo I have along with me.
		if( GetPrimaryAmmoCount() > 0 )
			return true;
	}

	return false;
}


bool CBaseCombatWeapon::HasSecondaryAmmo( void )
{
	// If I use a clip, and have some ammo in it, then I have ammo
	if ( UsesClipsForAmmo2() )
	{
		if ( m_iClip2 > 0 )
			return true;
	}

	// Otherwise, I have ammo if I have some in my ammo counts
	CBaseCombatCharacter		*pOwner = GetOwner();
	if ( pOwner )
	{
		if ( pOwner->GetAmmoCount( m_iSecondaryAmmoType ) > 0 ) 
			return true;
	}

	return false;
}


bool CBaseCombatWeapon::UsesPrimaryAmmo( void )
{
	if ( m_iPrimaryAmmoType < 0 )
		return false;
	return true;
}


bool CBaseCombatWeapon::UsesSecondaryAmmo( void )
{
	if ( m_iSecondaryAmmoType < 0 )
		return false;
	return true;
}
*/

//-----------------------------------------------------------------------------
// Purpose: Show/hide weapon and corresponding view model if any
// Input  : visible - 
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::SetWeaponVisible( bool visible )
{
	CBaseViewModel *vm = NULL;

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner )
	{
		vm = pOwner->GetViewModel( m_nViewModelIndex );
	}

	if ( visible )
	{
		RemoveEffects( EF_NODRAW );
		if ( vm )
		{
			vm->RemoveEffects( EF_NODRAW );
		}
	}
	else
	{
		AddEffects( EF_NODRAW );
		if ( vm )
		{
			vm->AddEffects( EF_NODRAW );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CBaseCombatWeapon::IsWeaponVisible( void )
{
	CBaseViewModel *vm = NULL;
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner )
	{
		vm = pOwner->GetViewModel( m_nViewModelIndex );
		if ( vm )
			return ( !vm->IsEffectActive(EF_NODRAW) );
	}

	return false;
}

/* BOXBOX removing
bool CBaseCombatWeapon::ReloadOrSwitchWeapons( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	Assert( pOwner );

	m_bFireOnEmpty = false;

	// If we don't have any ammo, switch to the next best weapon
	if ( !HasAnyAmmo() && m_flNextPrimaryAttack < gpGlobals->curtime && m_flNextSecondaryAttack < gpGlobals->curtime )
	{
		// weapon isn't useable, switch.
		if ( ( (GetWeaponFlags() & ITEM_FLAG_NOAUTOSWITCHEMPTY) == false ) && ( g_pGameRules->SwitchToNextBestWeapon( pOwner, this ) ) )
		{
			m_flNextPrimaryAttack = gpGlobals->curtime + 0.3;
			return true;
		}
	}
	else
	{
		// Weapon is useable. Reload if empty and weapon has waited as long as it has to after firing
		if ( UsesClipsForAmmo1() && !AutoFiresFullClip() && 
			 (m_iClip1 == 0) && 
			 (GetWeaponFlags() & ITEM_FLAG_NOAUTORELOAD) == false && 
			 m_flNextPrimaryAttack < gpGlobals->curtime && 
			 m_flNextSecondaryAttack < gpGlobals->curtime )
		{
			// if we're successfully reloading, we're done
			if ( Reload() )
				return true;
		}
	}

	return false;
}
*/


// Returns true on success, false on failure.

bool CBaseCombatWeapon::DefaultDeploy( char *szViewModel, char *szWeaponModel, int iActivity, char *szAnimExt )
{
	// Msg( "deploy %s at %f\n", GetClassname(), gpGlobals->curtime );

/* BOXBOX removing
	if ( !HasAnyAmmo() && AllowsAutoSwitchFrom() )
		return false;
*/
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner )
	{
		// Dead men deploy no weapons
		if ( pOwner->IsAlive() == false )
			return false;

		pOwner->SetAnimationExtension( szAnimExt );

		SetViewModel();
		SendWeaponAnim( iActivity );

		pOwner->SetNextAttack( gpGlobals->curtime + SequenceDuration() );
	}

	// Can't shoot again until we've finished deploying
	m_flNextPrimaryAttack	= gpGlobals->curtime + SequenceDuration();
	m_flNextSecondaryAttack	= gpGlobals->curtime + SequenceDuration();
/* BOXBOX removing
	m_flHudHintMinDisplayTime = 0;
	m_bAltFireHudHintDisplayed = false;
	m_bReloadHudHintDisplayed = false;
	m_flHudHintPollTime = gpGlobals->curtime + 5.0f;
*/	
	WeaponSound( DEPLOY );

	SetWeaponVisible( true );

	SetContextThink( NULL, 0, HIDEWEAPON_THINK_CONTEXT );

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CBaseCombatWeapon::Deploy( )
{
	MDLCACHE_CRITICAL_SECTION();
	return DefaultDeploy( (char*)GetViewModel(), (char*)GetWorldModel(), GetDrawActivity(), (char*)GetAnimPrefix() );
}

Activity CBaseCombatWeapon::GetDrawActivity( void )
{
	return ACT_VM_DRAW;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CBaseCombatWeapon::Holster( CBaseCombatWeapon *pSwitchingTo )
{ 
	MDLCACHE_CRITICAL_SECTION();

/* BOXBOX removing
	m_bInReload = false; 
	m_bFiringWholeClip = false;
*/

	// kill any think functions
	SetThink(NULL);

	// Send holster animation
	SendWeaponAnim( ACT_VM_HOLSTER );

	// Some weapon's don't have holster anims yet, so detect that
	float flSequenceDuration = 0;
	if ( GetActivity() == ACT_VM_HOLSTER )
	{
		flSequenceDuration = SequenceDuration();
	}

	CBaseCombatCharacter *pOwner = GetOwner();
	if (pOwner)
	{
		pOwner->SetNextAttack( gpGlobals->curtime + flSequenceDuration );
	}

	// If we don't have a holster anim, hide immediately to avoid timing issues
	if ( !flSequenceDuration )
	{
		SetWeaponVisible( false );
	}
	else
	{
		// Hide the weapon when the holster animation's finished
		SetContextThink( &CBaseCombatWeapon::HideThink, gpGlobals->curtime + flSequenceDuration, HIDEWEAPON_THINK_CONTEXT );
	}

/*
	if (m_flHudHintMinDisplayTime && gpGlobals->curtime < m_flHudHintMinDisplayTime)
	{
		if( m_bAltFireHudHintDisplayed )
			RescindAltFireHudHint();

		if( m_bReloadHudHintDisplayed )
			RescindReloadHudHint();
	}
*/
	return true;
}

#ifdef CLIENT_DLL

	void CBaseCombatWeapon::BoneMergeFastCullBloat( Vector &localMins, Vector &localMaxs, const Vector &thisEntityMins, const Vector &thisEntityMaxs ) const
	{
		// The default behavior pushes it out by BONEMERGE_FASTCULL_BBOX_EXPAND in all directions, but we can do better
		// since we know the weapon will never point behind him.

		localMaxs.x += 20;	// Leaves some space in front for long weapons.
		
		localMins.y -= 20;	// Fatten it to his left and right since he can rotate that way.
		localMaxs.y += 20;	

		localMaxs.z += 15;	// Leave some space at the top.
	}

#else
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::InputHideWeapon( inputdata_t &inputdata )
{
	// Only hide if we're still the active weapon. If we're not the active weapon
	if ( GetOwner() && GetOwner()->GetActiveWeapon() == this )
	{
		SetWeaponVisible( false );
	}
}
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::HideThink( void )
{
	// Only hide if we're still the active weapon.
	if ( GetOwner() && GetOwner()->GetActiveWeapon() == this )
	{
		SetWeaponVisible( false );
	}
}

/* BOXBOX removing
bool CBaseCombatWeapon::CanReload( void )
{
	if ( AutoFiresFullClip() && m_bFiringWholeClip )
	{
		return false;
	}

	return true;
}

#if defined ( TF_CLIENT_DLL ) || defined ( TF_DLL )
//-----------------------------------------------------------------------------
// Purpose: Anti-hack
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::AddToCritBucket( float flAmount )
{
	float flCap = tf_weapon_criticals_bucket_cap.GetFloat();

	// Regulate crit frequency to reduce client-side seed hacking
	if ( m_flCritTokenBucket < flCap )
	{
		// Treat raw damage as the resource by which we add or subtract from the bucket
		m_flCritTokenBucket += flAmount;
		m_flCritTokenBucket = Min( m_flCritTokenBucket, flCap );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Anti-hack
//-----------------------------------------------------------------------------
bool CBaseCombatWeapon::IsAllowedToWithdrawFromCritBucket( float flDamage )
{
	// Note: If we're in this block of code, the assumption is that the
	// seed said we should grant a random crit.  If allowed, the cost
	// will be deducted here.

	// Track each seed request - in cases where a player is hacking, we'll 
	// see a silly ratio.
	m_nCritSeedRequests++;

	// Adjust token cost based on the ratio of requests vs granted, except
	// melee, which crits much more than ranged (as high as 60% chance)
	float flMult = ( IsMeleeWeapon() ) ? 0.5f : RemapValClamped( ( (float)m_nCritSeedRequests / (float)m_nCritChecks ), 0.1f, 1.f, 1.f, 3.f );

	// Would this take us below our limit?
	float flCost = ( flDamage * TF_DAMAGE_CRIT_MULTIPLIER ) * flMult;
	if ( flCost > m_flCritTokenBucket )
		return false;

	// Withdraw
	RemoveFromCritBucket( flCost );

	float flBottom = tf_weapon_criticals_bucket_bottom.GetFloat();
	if ( m_flCritTokenBucket < flBottom )
		m_flCritTokenBucket = flBottom;

	return true;
}
#endif // TF_DLL
*/

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::ItemPreFrame( void )
{
	MaintainIdealActivity();

/* BOXBOX removing
#ifndef CLIENT_DLL
#ifndef HL2_EPISODIC
	if ( IsX360() )
#endif
	{
		// If we haven't displayed the hint enough times yet, it's time to try to 
		// display the hint, and the player is not standing still, try to show a hud hint.
		// If the player IS standing still, assume they could change away from this weapon at
		// any second.
		if( (!m_bAltFireHudHintDisplayed || !m_bReloadHudHintDisplayed) && gpGlobals->curtime > m_flHudHintMinDisplayTime && gpGlobals->curtime > m_flHudHintPollTime && GetOwner() && GetOwner()->IsPlayer() )
		{
			CBasePlayer *pPlayer = (CBasePlayer*)(GetOwner());

			if( pPlayer && pPlayer->GetStickDist() > 0.0f )
			{
				// If the player is moving, they're unlikely to switch away from the current weapon
				// the moment this weapon displays its HUD hint.
				if( ShouldDisplayReloadHUDHint() )
				{
					DisplayReloadHudHint();
				}
				else if( ShouldDisplayAltFireHUDHint() )
				{
					DisplayAltFireHudHint();
				}
			}
			else
			{
				m_flHudHintPollTime = gpGlobals->curtime + 2.0f;
			}
		}
	}
#endif
*/
}

//====================================================================================
// WEAPON BEHAVIOUR
//====================================================================================
void CBaseCombatWeapon::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	if ( (pOwner->m_nButtons & IN_ATTACK) && (m_flNextPrimaryAttack <= gpGlobals->curtime) )
	{
		PrimaryAttack();
	} 
	else if ( (pOwner->m_nButtons & IN_ATTACK2) && (m_flNextSecondaryAttack <= gpGlobals->curtime) )
	{
		SecondaryAttack();
	}
	else 
	{
		WeaponIdle();
		return;
	}
}

/* BOXBOX removing
void CBaseCombatWeapon::HandleFireOnEmpty()
{
	if ( m_bFireOnEmpty )
	{
		ReloadOrSwitchWeapons();
		m_fFireDuration = 0.0f;
	}
	else
	{
		if (m_flNextEmptySoundTime < gpGlobals->curtime)
		{
			WeaponSound(EMPTY);
			m_flNextEmptySoundTime = gpGlobals->curtime + 0.5;
		}
		m_bFireOnEmpty = true;
	}
}
*/

//-----------------------------------------------------------------------------
// Purpose: Called each frame by the player PostThink, if the player's not ready to attack yet
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::ItemBusyFrame( void )
{
//	UpdateAutoFire();
}

/* BOXBOX removing
int CBaseCombatWeapon::GetBulletType( void )
{
	return 0;
}

const Vector& CBaseCombatWeapon::GetBulletSpread( void )
{
	static Vector cone = VECTOR_CONE_15DEGREES;
	return cone;
}

const WeaponProficiencyInfo_t *CBaseCombatWeapon::GetProficiencyValues()
{
	static WeaponProficiencyInfo_t defaultWeaponProficiencyTable[] =
	{
		{ 1.0, 1.0	},
		{ 1.0, 1.0	},
		{ 1.0, 1.0	},
		{ 1.0, 1.0	},
		{ 1.0, 1.0	},
	};

	COMPILE_TIME_ASSERT( ARRAYSIZE(defaultWeaponProficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1);
	return defaultWeaponProficiencyTable;
}
*/

float CBaseCombatWeapon::GetSwingRate( void ) // BOXBOX recalculated this for MSS
{
#ifndef CLIENT_DLL

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if( !pPlayer ) return NULL;

	CMSS_Player *pMSSPlayer = ToMSSPlayer( pPlayer );
	if( !pPlayer ) return NULL;

	int wpntype = GetWeaponType();
	float skillmod = pMSSPlayer->GetWeaponSkill( wpntype );
	float mightmod = pMSSPlayer->GetStatMight();
	float agilitymod = pMSSPlayer->GetStatAgility();

	float baserate = 0.4f + ( wpntype * 0.1f );	// BOXBOX first, get base rate depending on weapon type
	baserate -= 0.01f * skillmod;				// BOXBOX take a tenth off per weapon skill level
	baserate -= 0.005f * mightmod;
	baserate -= 0.005f * agilitymod;

//	Warning("SKILLMOD: %f\n", skillmod );
//	Warning("BASERATE: %f\n", baserate );

	// BOXBOX TODO add considerations for magic buffs, skill buffs, etc.


	// BOXBOX add a little randomization (10%)

	float variance = baserate / 10;
	float final = RandomFloat(baserate - variance, baserate + variance);

	return final;

#else

	return NULL;

#endif
}


float CBaseCombatWeapon::GetDamageForActivity( Activity hitActivity ) // BOXBOX recalculated this for MSS
{
#ifndef CLIENT_DLL

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if( !pPlayer ) return NULL;

	CMSS_Player *pMSSPlayer = ToMSSPlayer( pPlayer );
	if( !pPlayer ) return NULL;

	int wpntype = GetWeaponType() + 1;

	float skillmod = pMSSPlayer->GetWeaponSkill( wpntype - 1 );
	float mightmod = pMSSPlayer->GetStatMight();
	float speedmod = pMSSPlayer->GetStatAgility();

	// BOXBOX first, get base damage depending on weapon type
	if( wpntype > 4 ) wpntype++; // BOXBOX 2 handed types
	if( wpntype == 9 ) wpntype = 5; // BOXBOX archery damage

	float basedmg = 1.0f;

	if( wpntype == 1 ) // unarmed
	{
		// BOXBOX TODO add damage mod for gloves' item level!
	}
	else
	{
		int wpnlvl = GetItemLevel();

		basedmg += (wpntype * 3) + (wpnlvl * 3);
//		basedmg += wpnlvl * 2;
	}

	basedmg += skillmod * skillmod - 1; // BOXBOX add weapon skill level

	mightmod *= wpntype * 0.5; // BOXBOX add might.  Might's influence increases with weapon size(type)
	basedmg += mightmod;

	speedmod *= (9 - wpntype) * 0.5; // BOXBOX add agility.  Agility's influence decreases with weapon size(type)
	basedmg += speedmod;

//	Warning("BASEDAMAGE: %f\n", basedmg );

	// BOXBOX TODO add considerations for magic buffs, skill buffs, etc.

	// BOXBOX TODO add crits!

	// BOXBOX add randomization
	float variance = basedmg * 0.25f;
	float finaldmg = RandomFloat(basedmg - variance, basedmg + variance);

	if( finaldmg < 1.0 )
		finaldmg = 1.0f;

	char dmg[8];	// BOXBOX TODO damage display this way for now
	Q_snprintf( dmg, sizeof(dmg), "%i", (int)finaldmg );
	ClientPrint( pPlayer, HUD_PRINTCENTER, "%s1", dmg );

	return finaldmg;

#else

	return NULL;

#endif
}

void CBaseCombatWeapon::WeaponSound( WeaponSound_t sound_type, float soundtime /* = 0.0f */ )
{
	// If we have some sounds from the script file, play a random one of them
	const char *wpnsound = GetWpnSound( sound_type );
	if ( !wpnsound || !wpnsound[0] )
		return;

	CSoundParameters params;
	
	if ( !GetParametersForSound( wpnsound, params, NULL ) )
		return;

	if ( params.play_to_owner_only )
	{
		// Am I only to play to my owner?
		if ( GetOwner() && GetOwner()->IsPlayer() )
		{
			CSingleUserRecipientFilter filter( ToBasePlayer( GetOwner() ) );
			if ( IsPredicted() && CBaseEntity::GetPredictionPlayer() )
			{
				filter.UsePredictionRules();
			}
			EmitSound( filter, GetOwner()->entindex(), wpnsound, NULL, soundtime );
		}
	}
	else
	{
		// Play weapon sound from the owner
		if ( GetOwner() )
		{
			CPASAttenuationFilter filter( GetOwner(), params.soundlevel );
			if ( IsPredicted() && CBaseEntity::GetPredictionPlayer() )
			{
				filter.UsePredictionRules();
			}
			EmitSound( filter, GetOwner()->entindex(), wpnsound, NULL, soundtime ); 

#if !defined( CLIENT_DLL )
			if( sound_type == 0 )
			{
				CSoundEnt::InsertSound( SOUND_COMBAT, GetOwner()->GetAbsOrigin(), SOUNDENT_VOLUME_EMPTY, 0.2, GetOwner() );
			}
#endif
		}
		// If no owner play from the weapon (this is used for thrown items)
		else
		{
			CPASAttenuationFilter filter( this, params.soundlevel );
			if ( IsPredicted() && CBaseEntity::GetPredictionPlayer() )
			{
				filter.UsePredictionRules();
			}
			EmitSound( filter, entindex(), wpnsound, NULL, soundtime ); 
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Stop a sound played by this weapon.
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::StopWeaponSound( WeaponSound_t sound_type )
{
	//if ( IsPredicted() )
	//	return;

	// If we have some sounds from the weapon classname.txt file, play a random one of them
	const char *wpnsound = GetWpnSound( sound_type );
	if ( !wpnsound || !wpnsound[0] )
		return;
	
	CSoundParameters params;
	if ( !GetParametersForSound( wpnsound, params, NULL ) )
		return;

	// Am I only to play to my owner?
	if ( params.play_to_owner_only )
	{
		if ( GetOwner() )
		{
			StopSound( GetOwner()->entindex(), wpnsound );
		}
	}
	else
	{
		// Play weapon sound from the owner
		if ( GetOwner() )
		{
			StopSound( GetOwner()->entindex(), wpnsound );
		}
		// If no owner play from the weapon (this is used for thrown items)
		else
		{
			StopSound( entindex(), wpnsound );
		}
	}
}

/* BOXBOX removing
bool CBaseCombatWeapon::DefaultReload( int iClipSize1, int iClipSize2, int iActivity )
{
	CBaseCombatCharacter *pOwner = GetOwner();
	if (!pOwner)
		return false;

	// If I don't have any spare ammo, I can't reload
	if ( pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0 )
		return false;

	bool bReload = false;

	// If you don't have clips, then don't try to reload them.
	if ( UsesClipsForAmmo1() )
	{
		// need to reload primary clip?
		int primary	= MIN(iClipSize1 - m_iClip1, pOwner->GetAmmoCount(m_iPrimaryAmmoType));
		if ( primary != 0 )
		{
			bReload = true;
		}
	}

	if ( UsesClipsForAmmo2() )
	{
		// need to reload secondary clip?
		int secondary = MIN(iClipSize2 - m_iClip2, pOwner->GetAmmoCount(m_iSecondaryAmmoType));
		if ( secondary != 0 )
		{
			bReload = true;
		}
	}

	if ( !bReload )
		return false;

#ifdef CLIENT_DLL
	// Play reload
	WeaponSound( RELOAD );
#endif
	SendWeaponAnim( iActivity );

	// Play the player's reload animation
	if ( pOwner->IsPlayer() )
	{
		( ( CBasePlayer * )pOwner)->SetAnimation( PLAYER_RELOAD );
	}

	MDLCACHE_CRITICAL_SECTION();
	float flSequenceEndTime = gpGlobals->curtime + SequenceDuration();
	pOwner->SetNextAttack( flSequenceEndTime );
	m_flNextPrimaryAttack = m_flNextSecondaryAttack = flSequenceEndTime;

	m_bInReload = true;

	return true;
}

bool CBaseCombatWeapon::ReloadsSingly( void ) const
{

#if defined ( TF_DLL ) || defined ( TF_CLIENT_DLL )
	float fHasReload = 1.0f;
	CALL_ATTRIB_HOOK_FLOAT( fHasReload, mod_no_reload_display_only );
	if ( fHasReload != 1.0f )
	{
		return false;
	}

	int iWeaponMod = 0;
	CALL_ATTRIB_HOOK_INT( iWeaponMod, set_scattergun_no_reload_single );
	if ( iWeaponMod == 1 )
	{
		return false;
	}
#endif // TF_DLL || TF_CLIENT_DLL

	return m_bReloadsSingly;
}

bool CBaseCombatWeapon::Reload( void )
{
	return DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD );
}
*/

//=========================================================
void CBaseCombatWeapon::WeaponIdle( void )
{
	//Idle again if we've finished
	if ( HasWeaponIdleTimeElapsed() )
	{
		SendWeaponAnim( ACT_VM_IDLE );
	}
}


//=========================================================
Activity CBaseCombatWeapon::GetPrimaryAttackActivity( void )
{
	return ACT_VM_PRIMARYATTACK;
}

//=========================================================
Activity CBaseCombatWeapon::GetSecondaryAttackActivity( void )
{
	return ACT_VM_SECONDARYATTACK;
}

/*
void CBaseCombatWeapon::AddViewKick( void )
{
}
*/

//-----------------------------------------------------------------------------
// Purpose: Get the string to print death notices with
//-----------------------------------------------------------------------------
char *CBaseCombatWeapon::GetDeathNoticeName( void )
{
#if !defined( CLIENT_DLL )
	return (char*)STRING( m_iszName );
#else
	return "GetDeathNoticeName not implemented on client yet";
#endif
}

/* BOXBOX removing
void CBaseCombatWeapon::CheckReload( void )
{
	if ( m_bReloadsSingly )
	{
		CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
		if ( !pOwner )
			return;

		if ((m_bInReload) && (m_flNextPrimaryAttack <= gpGlobals->curtime))
		{
			if ( pOwner->m_nButtons & (IN_ATTACK | IN_ATTACK2) && m_iClip1 > 0 )
			{
				m_bInReload = false;
				return;
			}

			// If out of ammo end reload
			if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) <=0)
			{
				FinishReload();
				return;
			}
			// If clip not full reload again
			else if (m_iClip1 < GetMaxClip1())
			{
				// Add them to the clip
				m_iClip1 += 1;
				pOwner->RemoveAmmo( 1, m_iPrimaryAmmoType );

				Reload();
				return;
			}
			// Clip full, stop reloading
			else
			{
				FinishReload();
				m_flNextPrimaryAttack	= gpGlobals->curtime;
				m_flNextSecondaryAttack = gpGlobals->curtime;
				return;
			}
		}
	}
	else
	{
		if ( (m_bInReload) && (m_flNextPrimaryAttack <= gpGlobals->curtime))
		{
			FinishReload();
			m_flNextPrimaryAttack	= gpGlobals->curtime;
			m_flNextSecondaryAttack = gpGlobals->curtime;
			m_bInReload = false;
		}
	}
}

void CBaseCombatWeapon::FinishReload( void )
{
	CBaseCombatCharacter *pOwner = GetOwner();

	if (pOwner)
	{
		// If I use primary clips, reload primary
		if ( UsesClipsForAmmo1() )
		{
			int primary	= MIN( GetMaxClip1() - m_iClip1, pOwner->GetAmmoCount(m_iPrimaryAmmoType));	
			m_iClip1 += primary;
			pOwner->RemoveAmmo( primary, m_iPrimaryAmmoType);
		}

		// If I use secondary clips, reload secondary
		if ( UsesClipsForAmmo2() )
		{
			int secondary = MIN( GetMaxClip2() - m_iClip2, pOwner->GetAmmoCount(m_iSecondaryAmmoType));
			m_iClip2 += secondary;
			pOwner->RemoveAmmo( secondary, m_iSecondaryAmmoType );
		}

		if ( m_bReloadsSingly )
		{
			m_bInReload = false;
		}
	}
}

void CBaseCombatWeapon::AbortReload( void )
{

#ifdef CLIENT_DLL
	StopWeaponSound( RELOAD ); 
#endif

	m_bInReload = false;
}

void CBaseCombatWeapon::UpdateAutoFire( void )
{
	if ( !AutoFiresFullClip() )
		return;

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return;

	if ( m_iClip1 == 0 )
	{
		// Ready to reload again
		m_bFiringWholeClip = false;
	}

	if ( m_bFiringWholeClip )
	{
		// If it's firing the clip don't let them repress attack to reload
		pOwner->m_nButtons &= ~IN_ATTACK;
	}

	// Don't use the regular reload key
	if ( pOwner->m_nButtons & IN_RELOAD )
	{
		pOwner->m_nButtons &= ~IN_RELOAD;
	}

	// Try to fire if there's ammo in the clip and we're not holding the button
	bool bReleaseClip = m_iClip1 > 0 && !( pOwner->m_nButtons & IN_ATTACK );

	if ( !bReleaseClip )
	{
		if ( CanReload() && ( pOwner->m_nButtons & IN_ATTACK ) )
		{
			// Convert the attack key into the reload key
			pOwner->m_nButtons |= IN_RELOAD;
		}

		// Don't allow attack button if we're not attacking
		pOwner->m_nButtons &= ~IN_ATTACK;
	}
	else
	{
		// Fake the attack key
		pOwner->m_nButtons |= IN_ATTACK;
	}
}
*/


void CBaseCombatWeapon::PrimaryAttack( void ) // BOXBOX TODO ongoing WIP on-the-fly kinda thang
{
//	CMSS_Player *pPlayer = ToMSSPlayer( GetPlayerOwner() );
//	if (!pPlayer) return;

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if (!pPlayer) return;

#ifndef CLIENT_DLL
	// Move other players back to history positions based on local player's lag
	lagcompensation->StartLagCompensation( pPlayer, pPlayer->GetCurrentCommand() );
#endif

	Swing( false );

#ifndef CLIENT_DLL
	// Move other players back to history positions based on local player's lag
	lagcompensation->FinishLagCompensation( pPlayer );
#endif

/* BOXBOX removing
	if ( UsesClipsForAmmo1() && !m_iClip1 ) 
	{
		Reload();
		return;
	}
*/
	// Only the player fires this way so we can cast
//	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
//	if (!pPlayer) return;

//	pPlayer->DoMuzzleFlash();

	SendWeaponAnim( GetPrimaryAttackActivity() );

	// player attack animation
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

/* BOXBOX removing
	FireBulletsInfo_t info;
	info.m_vecSrc	 = pPlayer->Weapon_ShootPosition( );
	
	info.m_vecDirShooting = pPlayer->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT );

	info.m_iShots = 0;

	float fireRate = GetSwingRate();

	while ( m_flNextPrimaryAttack <= gpGlobals->curtime )
	{
		WeaponSound(MELEE_MISS, m_flNextPrimaryAttack);
		m_flNextPrimaryAttack = m_flNextPrimaryAttack + fireRate;
//		info.m_iShots++;
		if ( !fireRate )
			break;
	}


	// Make sure we don't fire more than the amount in the clip
	if ( UsesClipsForAmmo1() )
	{
		info.m_iShots = MIN( info.m_iShots, m_iClip1 );
		m_iClip1 -= info.m_iShots;
	}
	else
	{
		info.m_iShots = MIN( info.m_iShots, pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) );
		pPlayer->RemoveAmmo( info.m_iShots, m_iPrimaryAmmoType );
	}

	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_iTracerFreq = 2;

#if !defined( CLIENT_DLL )
	// Fire the bullets
	info.m_vecSpread = pPlayer->GetAttackSpread( this );
#else
	//!!!HACKHACK - what does the client want this function for? 
	info.m_vecSpread = GetActiveWeapon()->GetBulletSpread();
#endif // CLIENT_DLL

	pPlayer->FireBullets( info );

	if (!m_iClip1 && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0); 
	}

	//Add our view kick in
	AddViewKick();
*/
}

void CBaseCombatWeapon::SecondaryAttack()
{
	Swing( true );
}


void CBaseCombatWeapon::ImpactEffect( trace_t &traceHit )
{
	// See if we hit water (we don't do the other impact effects in this case)
	if ( ImpactWater( traceHit.startpos, traceHit.endpos ) )
		return;

	//FIXME: need new decals
	UTIL_ImpactTrace( &traceHit, DMG_CLUB );
}

bool CBaseCombatWeapon::ImpactWater( const Vector &start, const Vector &end )
{
	//FIXME: This doesn't handle the case of trying to splash while being underwater, but that's not going to look good
	//		 right now anyway...
	
	// We must start outside the water
	if ( UTIL_PointContents( start ) & (CONTENTS_WATER|CONTENTS_SLIME))
		return false;

	// We must end inside of water
	if ( !(UTIL_PointContents( end ) & (CONTENTS_WATER|CONTENTS_SLIME)))
		return false;

	trace_t	waterTrace;

	UTIL_TraceLine( start, end, (CONTENTS_WATER|CONTENTS_SLIME), GetOwner(), COLLISION_GROUP_NONE, &waterTrace );

	if ( waterTrace.fraction < 1.0f )
	{
#ifndef CLIENT_DLL
		CEffectData	data;

		data.m_fFlags  = 0;
		data.m_vOrigin = waterTrace.endpos;
		data.m_vNormal = waterTrace.plane.normal;
		data.m_flScale = 8.0f;

		// See if we hit slime
		if ( waterTrace.contents & CONTENTS_SLIME )
		{
			data.m_fFlags |= FX_WATER_IN_SLIME;
		}

		DispatchEffect( "watersplash", data );			
#endif
	}

	return true;
}

#define WEAPON_HULL_DIM		16

static const Vector g_weaponMins(-WEAPON_HULL_DIM,-WEAPON_HULL_DIM,-WEAPON_HULL_DIM);
static const Vector g_weaponMaxs(WEAPON_HULL_DIM,WEAPON_HULL_DIM,WEAPON_HULL_DIM);

void CBaseCombatWeapon::Swing( int bIsSecondary )
{
	trace_t traceHit;

	// Try a ray
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return;

	Vector swingStart = pOwner->Weapon_ShootPosition( );
	Vector forward;

	pOwner->EyeVectors( &forward, NULL, NULL );

	Vector swingEnd = swingStart + forward * GetRange();
	UTIL_TraceLine( swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );
	Activity nHitActivity = ACT_VM_HITCENTER;

#ifndef CLIENT_DLL
	//  traces have to trace against triggers.
	CTakeDamageInfo triggerInfo( GetOwner(), GetOwner(), GetDamageForActivity( nHitActivity ), DMG_CLUB );
	TraceAttackToTriggers( triggerInfo, traceHit.startpos, traceHit.endpos, vec3_origin );
#endif

	if ( traceHit.fraction == 1.0 )
	{
		float bludgeonHullRadius = 1.732f * WEAPON_HULL_DIM;  // hull is +/- 16, so use cuberoot of 2 to determine how big the hull is from center to the corner point

		// Back off by hull "radius"
		swingEnd -= forward * bludgeonHullRadius;

		UTIL_TraceHull( swingStart, swingEnd, g_weaponMins, g_weaponMaxs, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );
		if ( traceHit.fraction < 1.0 && traceHit.m_pEnt )
		{
			Vector vecToTarget = traceHit.m_pEnt->GetAbsOrigin() - swingStart;
			VectorNormalize( vecToTarget );

			float dot = vecToTarget.Dot( forward );

			// YWB:  Make sure they are sort of facing the guy at least...
			if ( dot < 0.70721f )
			{
				// Force a miss
				traceHit.fraction = 1.0f;
			}
			else
			{
				nHitActivity = ChooseIntersectionPointAndActivity( traceHit, g_weaponMins, g_weaponMaxs, pOwner );
			}
		}
	}

	WeaponSound( MELEE_MISS );

	// -------------------------
	//	Miss
	// -------------------------
	if ( traceHit.fraction == 1.0f )
	{
		nHitActivity = bIsSecondary ? ACT_VM_MISSCENTER2 : ACT_VM_MISSCENTER;

		// We want to test the first swing again
		Vector testEnd = swingStart + forward * GetRange();
		
		// See if we happened to hit water
		ImpactWater( swingStart, testEnd );
	}
	else
	{
		Hit( traceHit, nHitActivity );
	}

	// Send the anim
	SendWeaponAnim( nHitActivity );

	pOwner->SetAnimation( PLAYER_ATTACK1 );

	//Setup our next attack times
	m_flNextPrimaryAttack = gpGlobals->curtime + GetSwingRate();
	m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration();
}

void CBaseCombatWeapon::Hit( trace_t &traceHit, Activity nHitActivity )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	//Do view kick
//	AddViewKick();

	CBaseEntity	*pHitEntity = traceHit.m_pEnt;

	//Apply damage to a hit target
	if ( pHitEntity != NULL )
	{
		Vector hitDirection;
		pPlayer->EyeVectors( &hitDirection, NULL, NULL );
		VectorNormalize( hitDirection );

#ifndef CLIENT_DLL
		CTakeDamageInfo info( GetOwner(), GetOwner(), GetDamageForActivity( nHitActivity ), DMG_CLUB );

		if( pPlayer && pHitEntity->IsNPC() )
		{
			// If bonking an NPC, adjust damage.
			info.AdjustPlayerDamageInflictedForSkillLevel();
		}

		CalculateMeleeDamageForce( &info, hitDirection, traceHit.endpos );

		pHitEntity->DispatchTraceAttack( info, hitDirection, &traceHit ); 
		ApplyMultiDamage();

		// Now hit all triggers along the ray that... 
		TraceAttackToTriggers( info, traceHit.startpos, traceHit.endpos, hitDirection );
#endif
		WeaponSound( MELEE_HIT );
	}

	// Apply an impact effect
	ImpactEffect( traceHit );
}

Activity CBaseCombatWeapon::ChooseIntersectionPointAndActivity( trace_t &hitTrace, const Vector &mins, const Vector &maxs, CBasePlayer *pOwner )
{
	int			i, j, k;
	float		distance;
	const float	*minmaxs[2] = {mins.Base(), maxs.Base()};
	trace_t		tmpTrace;
	Vector		vecHullEnd = hitTrace.endpos;
	Vector		vecEnd;

	distance = 1e6f;
	Vector vecSrc = hitTrace.startpos;

	vecHullEnd = vecSrc + ((vecHullEnd - vecSrc)*2);
	UTIL_TraceLine( vecSrc, vecHullEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &tmpTrace );
	if ( tmpTrace.fraction == 1.0 )
	{
		for ( i = 0; i < 2; i++ )
		{
			for ( j = 0; j < 2; j++ )
			{
				for ( k = 0; k < 2; k++ )
				{
					vecEnd.x = vecHullEnd.x + minmaxs[i][0];
					vecEnd.y = vecHullEnd.y + minmaxs[j][1];
					vecEnd.z = vecHullEnd.z + minmaxs[k][2];

					UTIL_TraceLine( vecSrc, vecEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &tmpTrace );
					if ( tmpTrace.fraction < 1.0 )
					{
						float thisDistance = (tmpTrace.endpos - vecSrc).Length();
						if ( thisDistance < distance )
						{
							hitTrace = tmpTrace;
							distance = thisDistance;
						}
					}
				}
			}
		}
	}
	else
	{
		hitTrace = tmpTrace;
	}


	return ACT_VM_HITCENTER;
}


//-----------------------------------------------------------------------------
// Purpose: Called every frame to check if the weapon is going through transition animations
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::MaintainIdealActivity( void )
{
	// Must be transitioning
	if ( GetActivity() != ACT_TRANSITION )
		return;

	// Must not be at our ideal already 
	if ( ( GetActivity() == m_IdealActivity ) && ( GetSequence() == m_nIdealSequence ) )
		return;
	
	// Must be finished with the current animation
	if ( IsViewModelSequenceFinished() == false )
		return;

	// Move to the next animation towards our ideal
	SendWeaponAnim( m_IdealActivity );
}

//-----------------------------------------------------------------------------
// Purpose: Sets the ideal activity for the weapon to be in, allowing for transitional animations inbetween
// Input  : ideal - activity to end up at, ideally
//-----------------------------------------------------------------------------
bool CBaseCombatWeapon::SetIdealActivity( Activity ideal )
{
	MDLCACHE_CRITICAL_SECTION();
	int	idealSequence = SelectWeightedSequence( ideal );

	if ( idealSequence == -1 )
		return false;

	//Take the new activity
	m_IdealActivity	 = ideal;
	m_nIdealSequence = idealSequence;

	//Find the next sequence in the potential chain of sequences leading to our ideal one
	int nextSequence = FindTransitionSequence( GetSequence(), m_nIdealSequence, NULL );

	// Don't use transitions when we're deploying
	if ( ideal != ACT_VM_DRAW && IsWeaponVisible() && nextSequence != m_nIdealSequence )
	{
		//Set our activity to the next transitional animation
		SetActivity( ACT_TRANSITION );
		SetSequence( nextSequence );	
		SendViewModelAnim( nextSequence );
	}
	else
	{
		//Set our activity to the ideal
		SetActivity( m_IdealActivity );
		SetSequence( m_nIdealSequence );	
		SendViewModelAnim( m_nIdealSequence );
	}

	//Set the next time the weapon will idle
	SetWeaponIdleTime( gpGlobals->curtime + SequenceDuration() );
	return true;
}

//-----------------------------------------------------------------------------
// Returns information about the various control panels
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::GetControlPanelInfo( int nPanelIndex, const char *&pPanelName )
{
	pPanelName = NULL;
}

//-----------------------------------------------------------------------------
// Returns information about the various control panels
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::GetControlPanelClassName( int nPanelIndex, const char *&pPanelName )
{
	pPanelName = "vgui_screen";
}


//-----------------------------------------------------------------------------
// Locking a weapon is an exclusive action. If you lock a weapon, that means 
// you are preventing others from doing so for themselves.
//-----------------------------------------------------------------------------
void CBaseCombatWeapon::Lock( float lockTime, CBaseEntity *pLocker )
{
	m_flUnlockTime = gpGlobals->curtime + lockTime;
	m_hLocker.Set( pLocker );
}

//-----------------------------------------------------------------------------
// If I'm still locked for a period of time, tell everyone except the person
// that locked me that I'm not available. 
//-----------------------------------------------------------------------------
bool CBaseCombatWeapon::IsLocked( CBaseEntity *pAsker )
{
	return ( m_flUnlockTime > gpGlobals->curtime && m_hLocker != pAsker );
}

#if !defined( CLIENT_DLL )
Activity CBaseCombatWeapon::ActivityOverride( Activity baseAct, bool *pRequired )
{
	acttable_t *pTable = ActivityList();
	int actCount = ActivityListCount();

	for ( int i = 0; i < actCount; i++, pTable++ )
	{
		if ( baseAct == pTable->baseAct )
		{
			if (pRequired)
			{
				*pRequired = pTable->required;
			}
			return (Activity)pTable->weaponAct;
		}
	}
	return baseAct;
}
#endif

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CDmgAccumulator::CDmgAccumulator( void )
{
#ifdef GAME_DLL
	SetDefLessFunc( m_TargetsDmgInfo );
#endif // GAME_DLL

	m_bActive = false;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CDmgAccumulator::~CDmgAccumulator()
{
	// Did a weapon get deleted while aggregating CTakeDamageInfo events?
	Assert( !m_bActive );
}

#ifdef GAME_DLL
//-----------------------------------------------------------------------------
// Collect trace attacks for weapons that fire multiple bullets per attack that also penetrate
//-----------------------------------------------------------------------------
void CDmgAccumulator::AccumulateMultiDamage( const CTakeDamageInfo &info, CBaseEntity *pEntity )
{
	if ( !pEntity )
		return;

	Assert( m_bActive );

#if defined( GAME_DLL )
	int iIndex = m_TargetsDmgInfo.Find( pEntity->entindex() );
	if ( iIndex == m_TargetsDmgInfo.InvalidIndex() )
	{
		m_TargetsDmgInfo.Insert( pEntity->entindex(), info );
	}
	else
	{
		CTakeDamageInfo *pInfo = &m_TargetsDmgInfo[iIndex];
		if ( pInfo )
		{
			// Update
			m_TargetsDmgInfo[iIndex].AddDamageType( info.GetDamageType() );
			m_TargetsDmgInfo[iIndex].SetDamage( pInfo->GetDamage() + info.GetDamage() );
			m_TargetsDmgInfo[iIndex].SetDamageForce( pInfo->GetDamageForce() + info.GetDamageForce() );
			m_TargetsDmgInfo[iIndex].SetDamagePosition( info.GetDamagePosition() );
			m_TargetsDmgInfo[iIndex].SetReportedPosition( info.GetReportedPosition() );
			m_TargetsDmgInfo[iIndex].SetMaxDamage( MAX( pInfo->GetMaxDamage(), info.GetDamage() ) );
			m_TargetsDmgInfo[iIndex].SetAmmoType( info.GetAmmoType() );
		}

	}
#endif	// GAME_DLL
}

//-----------------------------------------------------------------------------
// Purpose: Send aggregate info
//-----------------------------------------------------------------------------
void CDmgAccumulator::Process( void )
{
	FOR_EACH_MAP( m_TargetsDmgInfo, i )
	{
		CBaseEntity *pEntity = UTIL_EntityByIndex( m_TargetsDmgInfo.Key( i ) );
		if ( pEntity )
		{
			AddMultiDamage( m_TargetsDmgInfo[i], pEntity );
		}
	}

	m_bActive = false;
	m_TargetsDmgInfo.Purge();
}
#endif // GAME_DLL

#if defined( CLIENT_DLL )

BEGIN_PREDICTION_DATA( CBaseCombatWeapon )

	DEFINE_PRED_FIELD( m_nNextThinkTick, FIELD_INTEGER, FTYPEDESC_INSENDTABLE ),
	// Networked
	DEFINE_PRED_FIELD( m_hOwner, FIELD_EHANDLE, FTYPEDESC_INSENDTABLE ),
	// DEFINE_FIELD( m_hItemFileInfo, FIELD_SHORT ),
	DEFINE_PRED_FIELD( m_iState, FIELD_INTEGER, FTYPEDESC_INSENDTABLE ),			 
	DEFINE_PRED_FIELD( m_iViewModelIndex, FIELD_INTEGER, FTYPEDESC_INSENDTABLE | FTYPEDESC_MODELINDEX ),
	DEFINE_PRED_FIELD( m_iWorldModelIndex, FIELD_INTEGER, FTYPEDESC_INSENDTABLE | FTYPEDESC_MODELINDEX ),
	DEFINE_PRED_FIELD_TOL( m_flNextPrimaryAttack, FIELD_FLOAT, FTYPEDESC_INSENDTABLE, TD_MSECTOLERANCE ),	
	DEFINE_PRED_FIELD_TOL( m_flNextSecondaryAttack, FIELD_FLOAT, FTYPEDESC_INSENDTABLE, TD_MSECTOLERANCE ),
	DEFINE_PRED_FIELD_TOL( m_flTimeWeaponIdle, FIELD_FLOAT, FTYPEDESC_INSENDTABLE, TD_MSECTOLERANCE ),
/* BOXBOX removing
	DEFINE_PRED_FIELD( m_iPrimaryAmmoType, FIELD_INTEGER, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_iSecondaryAmmoType, FIELD_INTEGER, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_iClip1, FIELD_INTEGER, FTYPEDESC_INSENDTABLE ),			
	DEFINE_PRED_FIELD( m_iClip2, FIELD_INTEGER, FTYPEDESC_INSENDTABLE ),			
*/
	DEFINE_PRED_FIELD( m_nViewModelIndex, FIELD_INTEGER, FTYPEDESC_INSENDTABLE ),

	// Not networked

	DEFINE_PRED_FIELD( m_flTimeWeaponIdle, FIELD_FLOAT, FTYPEDESC_INSENDTABLE ),
/* BOXBOX removing
	DEFINE_FIELD( m_bInReload, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bFireOnEmpty, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bFiringWholeClip, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flNextEmptySoundTime, FIELD_FLOAT ),
*/
	DEFINE_FIELD( m_Activity, FIELD_INTEGER ),
//	DEFINE_FIELD( m_fFireDuration, FIELD_FLOAT ),
	DEFINE_FIELD( m_iszName, FIELD_INTEGER ),		
//	DEFINE_FIELD( m_bFiresUnderwater, FIELD_BOOLEAN ),
//	DEFINE_FIELD( m_bAltFiresUnderwater, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_fMinRange1, FIELD_FLOAT ),		
	DEFINE_FIELD( m_fMinRange2, FIELD_FLOAT ),		
	DEFINE_FIELD( m_fMaxRange1, FIELD_FLOAT ),		
	DEFINE_FIELD( m_fMaxRange2, FIELD_FLOAT ),		
//	DEFINE_FIELD( m_bReloadsSingly, FIELD_BOOLEAN ),	
	DEFINE_FIELD( m_bRemoveable, FIELD_BOOLEAN ),
//	DEFINE_FIELD( m_iPrimaryAmmoCount, FIELD_INTEGER ),
//	DEFINE_FIELD( m_iSecondaryAmmoCount, FIELD_INTEGER ),

	//DEFINE_PHYSPTR( m_pConstraint ),

	// DEFINE_FIELD( m_iOldState, FIELD_INTEGER ),
	// DEFINE_FIELD( m_bJustRestored, FIELD_BOOLEAN ),

	// DEFINE_FIELD( m_OnPlayerPickup, COutputEvent ),
	// DEFINE_FIELD( m_pConstraint, FIELD_INTEGER ),

END_PREDICTION_DATA()

#endif	// ! CLIENT_DLL

// Special hack since we're aliasing the name C_BaseCombatWeapon with a macro on the client
IMPLEMENT_NETWORKCLASS_ALIASED( BaseCombatWeapon, DT_BaseCombatWeapon )

#if !defined( CLIENT_DLL )
//-----------------------------------------------------------------------------
// Purpose: Save Data for Base Weapon object
//-----------------------------------------------------------------------------// 
BEGIN_DATADESC( CBaseCombatWeapon )

	DEFINE_FIELD( m_flNextPrimaryAttack, FIELD_TIME ),
	DEFINE_FIELD( m_flNextSecondaryAttack, FIELD_TIME ),
	DEFINE_FIELD( m_flTimeWeaponIdle, FIELD_TIME ),

//	DEFINE_FIELD( m_bInReload, FIELD_BOOLEAN ),
//	DEFINE_FIELD( m_bFireOnEmpty, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_hOwner, FIELD_EHANDLE ),

	DEFINE_FIELD( m_iState, FIELD_INTEGER ),
	DEFINE_FIELD( m_iszName, FIELD_STRING ),
/* BOXBOX removing
	DEFINE_FIELD( m_iPrimaryAmmoType, FIELD_INTEGER ),
	DEFINE_FIELD( m_iSecondaryAmmoType, FIELD_INTEGER ),
	DEFINE_FIELD( m_iClip1, FIELD_INTEGER ),
	DEFINE_FIELD( m_iClip2, FIELD_INTEGER ),
	DEFINE_FIELD( m_bFiresUnderwater, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bAltFiresUnderwater, FIELD_BOOLEAN ),
*/
	DEFINE_FIELD( m_fMinRange1, FIELD_FLOAT ),
	DEFINE_FIELD( m_fMinRange2, FIELD_FLOAT ),
	DEFINE_FIELD( m_fMaxRange1, FIELD_FLOAT ),
	DEFINE_FIELD( m_fMaxRange2, FIELD_FLOAT ),
/* BOXBOX removing
	DEFINE_FIELD( m_iPrimaryAmmoCount, FIELD_INTEGER ),
	DEFINE_FIELD( m_iSecondaryAmmoCount, FIELD_INTEGER ),
*/
	DEFINE_FIELD( m_nViewModelIndex, FIELD_INTEGER ),

// don't save these, init to 0 and regenerate
//	DEFINE_FIELD( m_flNextEmptySoundTime, FIELD_TIME ),
//	DEFINE_FIELD( m_Activity, FIELD_INTEGER ),
 	DEFINE_FIELD( m_nIdealSequence, FIELD_INTEGER ),
	DEFINE_FIELD( m_IdealActivity, FIELD_INTEGER ),
/* BOXBOX removing
	DEFINE_FIELD( m_fFireDuration, FIELD_FLOAT ),

	DEFINE_FIELD( m_bReloadsSingly, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_iSubType, FIELD_INTEGER ),
*/
 	DEFINE_FIELD( m_bRemoveable, FIELD_BOOLEAN ),

	DEFINE_FIELD( m_flUnlockTime,		FIELD_TIME ),
	DEFINE_FIELD( m_hLocker,			FIELD_EHANDLE ),

	//	DEFINE_FIELD( m_iViewModelIndex, FIELD_INTEGER ),
	//	DEFINE_FIELD( m_iWorldModelIndex, FIELD_INTEGER ),
	//  DEFINE_FIELD( m_hItemFileInfo, ???? ),

	DEFINE_PHYSPTR( m_pConstraint ),
/* BOXBOX removing
	DEFINE_FIELD( m_iReloadHudHintCount,	FIELD_INTEGER ),
	DEFINE_FIELD( m_iAltFireHudHintCount,	FIELD_INTEGER ),
	DEFINE_FIELD( m_bReloadHudHintDisplayed, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bAltFireHudHintDisplayed, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flHudHintPollTime, FIELD_TIME ),
	DEFINE_FIELD( m_flHudHintMinDisplayTime, FIELD_TIME ),
*/
	// Just to quiet classcheck.. this field exists only on the client
//	DEFINE_FIELD( m_iOldState, FIELD_INTEGER ),
//	DEFINE_FIELD( m_bJustRestored, FIELD_BOOLEAN ),

	// Function pointers
	DEFINE_ENTITYFUNC( DefaultTouch ),
	DEFINE_THINKFUNC( FallThink ),
	DEFINE_THINKFUNC( Materialize ),
//	DEFINE_THINKFUNC( AttemptToMaterialize ),
	DEFINE_THINKFUNC( DestroyItem ),
	DEFINE_THINKFUNC( SetPickupTouch ),

	DEFINE_THINKFUNC( HideThink ),
	DEFINE_INPUTFUNC( FIELD_VOID, "HideWeapon", InputHideWeapon ),

	// Outputs
	DEFINE_OUTPUT( m_OnPlayerUse, "OnPlayerUse"),
	DEFINE_OUTPUT( m_OnPlayerPickup, "OnPlayerPickup"),
	DEFINE_OUTPUT( m_OnNPCPickup, "OnNPCPickup"),
	DEFINE_OUTPUT( m_OnCacheInteraction, "OnCacheInteraction" ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Only send to local player if this weapon is the active weapon
// Input  : *pStruct - 
//			*pVarData - 
//			*pRecipients - 
//			objectID - 
// Output : void*
//-----------------------------------------------------------------------------
void* SendProxy_SendActiveLocalWeaponDataTable( const SendProp *pProp, const void *pStruct, const void *pVarData, CSendProxyRecipients *pRecipients, int objectID )
{
	// Get the weapon entity
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)pVarData;
	if ( pWeapon )
	{
		// Only send this chunk of data to the player carrying this weapon
		CBasePlayer *pPlayer = ToBasePlayer( pWeapon->GetOwner() );
		if ( pPlayer /*&& pPlayer->GetActiveWeapon() == pWeapon*/ )
		{
			pRecipients->SetOnly( pPlayer->GetClientIndex() );
			return (void*)pVarData;
		}
	}
	
	return NULL;
}
REGISTER_SEND_PROXY_NON_MODIFIED_POINTER( SendProxy_SendActiveLocalWeaponDataTable );

//-----------------------------------------------------------------------------
// Purpose: Only send the LocalWeaponData to the player carrying the weapon
//-----------------------------------------------------------------------------
void* SendProxy_SendLocalWeaponDataTable( const SendProp *pProp, const void *pStruct, const void *pVarData, CSendProxyRecipients *pRecipients, int objectID )
{
	// Get the weapon entity
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)pVarData;
	if ( pWeapon )
	{
		// Only send this chunk of data to the player carrying this weapon
		CBasePlayer *pPlayer = ToBasePlayer( pWeapon->GetOwner() );
		if ( pPlayer )
		{
			pRecipients->SetOnly( pPlayer->GetClientIndex() );
			return (void*)pVarData;
		}
	}
	
	return NULL;
}
REGISTER_SEND_PROXY_NON_MODIFIED_POINTER( SendProxy_SendLocalWeaponDataTable );

//-----------------------------------------------------------------------------
// Purpose: Only send to non-local players
//-----------------------------------------------------------------------------
void* SendProxy_SendNonLocalWeaponDataTable( const SendProp *pProp, const void *pStruct, const void *pVarData, CSendProxyRecipients *pRecipients, int objectID )
{
	pRecipients->SetAllRecipients();

	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)pVarData;
	if ( pWeapon )
	{
		CBasePlayer *pPlayer = ToBasePlayer( pWeapon->GetOwner() );
		if ( pPlayer )
		{
			pRecipients->ClearRecipient( pPlayer->GetClientIndex() );
			return ( void * )pVarData;
		}
	}

	return NULL;
}
REGISTER_SEND_PROXY_NON_MODIFIED_POINTER( SendProxy_SendNonLocalWeaponDataTable );

#endif

#if PREDICTION_ERROR_CHECK_LEVEL > 1
#define SendPropTime SendPropFloat
#define RecvPropTime RecvPropFloat
#endif

//-----------------------------------------------------------------------------
// Purpose: Propagation data for weapons. Only sent when a player's holding it.
//-----------------------------------------------------------------------------
BEGIN_NETWORK_TABLE_NOBASE( CBaseCombatWeapon, DT_LocalActiveWeaponData )
#if !defined( CLIENT_DLL )
	SendPropTime( SENDINFO( m_flNextPrimaryAttack ) ),
	SendPropTime( SENDINFO( m_flNextSecondaryAttack ) ),
	SendPropInt( SENDINFO( m_nNextThinkTick ) ),
	SendPropTime( SENDINFO( m_flTimeWeaponIdle ) ),

/* BOXBOX removing
#if defined( TF_DLL )
	SendPropExclude( "DT_AnimTimeMustBeFirst" , "m_flAnimTime" ),
#endif
*/

#else
	RecvPropTime( RECVINFO( m_flNextPrimaryAttack ) ),
	RecvPropTime( RECVINFO( m_flNextSecondaryAttack ) ),
	RecvPropInt( RECVINFO( m_nNextThinkTick ) ),
	RecvPropTime( RECVINFO( m_flTimeWeaponIdle ) ),
#endif
END_NETWORK_TABLE()

//-----------------------------------------------------------------------------
// Purpose: Propagation data for weapons. Only sent when a player's holding it.
//-----------------------------------------------------------------------------
BEGIN_NETWORK_TABLE_NOBASE( CBaseCombatWeapon, DT_LocalWeaponData )
#if !defined( CLIENT_DLL )

/* BOXBOX removing
	SendPropIntWithMinusOneFlag( SENDINFO(m_iClip1 ), 8 ),
	SendPropIntWithMinusOneFlag( SENDINFO(m_iClip2 ), 8 ),
	SendPropInt( SENDINFO(m_iPrimaryAmmoType ), 8 ),
	SendPropInt( SENDINFO(m_iSecondaryAmmoType ), 8 ),
*/
	SendPropInt( SENDINFO( m_nViewModelIndex ), VIEWMODEL_INDEX_BITS, SPROP_UNSIGNED ),

	SendPropInt( SENDINFO( m_bFlipViewModel ) ),
/* BOXBOX removing
#if defined( TF_DLL )
	SendPropExclude( "DT_AnimTimeMustBeFirst" , "m_flAnimTime" ),
#endif
*/
#else
/* BOXBOX removing
	RecvPropIntWithMinusOneFlag( RECVINFO(m_iClip1 )),
	RecvPropIntWithMinusOneFlag( RECVINFO(m_iClip2 )),
	RecvPropInt( RECVINFO(m_iPrimaryAmmoType )),
	RecvPropInt( RECVINFO(m_iSecondaryAmmoType )),
*/
	RecvPropInt( RECVINFO( m_nViewModelIndex ) ),

	RecvPropBool( RECVINFO( m_bFlipViewModel ) ),

#endif
END_NETWORK_TABLE()

BEGIN_NETWORK_TABLE(CBaseCombatWeapon, DT_BaseCombatWeapon)
#if !defined( CLIENT_DLL )
	SendPropDataTable("LocalWeaponData", 0, &REFERENCE_SEND_TABLE(DT_LocalWeaponData), SendProxy_SendLocalWeaponDataTable ),
	SendPropDataTable("LocalActiveWeaponData", 0, &REFERENCE_SEND_TABLE(DT_LocalActiveWeaponData), SendProxy_SendActiveLocalWeaponDataTable ),
	SendPropModelIndex( SENDINFO(m_iViewModelIndex) ),
	SendPropModelIndex( SENDINFO(m_iWorldModelIndex) ),
	SendPropInt( SENDINFO(m_iState ), 8, SPROP_UNSIGNED ),
	SendPropEHandle( SENDINFO(m_hOwner) ),
#else
	RecvPropDataTable("LocalWeaponData", 0, 0, &REFERENCE_RECV_TABLE(DT_LocalWeaponData)),
	RecvPropDataTable("LocalActiveWeaponData", 0, 0, &REFERENCE_RECV_TABLE(DT_LocalActiveWeaponData)),
	RecvPropInt( RECVINFO(m_iViewModelIndex)),
	RecvPropInt( RECVINFO(m_iWorldModelIndex)),
	RecvPropInt( RECVINFO(m_iState )),
	RecvPropEHandle( RECVINFO(m_hOwner ) ),
#endif
END_NETWORK_TABLE()
