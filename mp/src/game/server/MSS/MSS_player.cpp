
// BOXBOX Player for MSS

#include "cbase.h"
#include "weapon_MSSbasecombatweapon.h"
#include "MSS_player.h"
#include "globalstate.h"
#include "game.h"
#include "gamerules.h"
#include "MSS_player_shared.h"
#include "predicted_viewmodel.h"
#include "in_buttons.h"
#include "MSS_gamerules.h"
#include "KeyValues.h"
//#include "team.h"
#include "weapon_MSSbase.h"
#include "grenade_satchel.h"
#include "eventqueue.h"
#include "gamestats.h"
#include "viewport_panel_names.h" // BOXBOX added

#include "engine/IEngineSound.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"

#include "ilagcompensationmanager.h"

#include <filesystem.h> // BOXBOX added for deleting character files ( see ClientCommand() )

/* BOXBOX removing stuff
int g_iLastCitizenModel = 0;
int g_iLastCombineModel = 0;

CBaseEntity	 *g_pLastCombineSpawn = NULL;
CBaseEntity	 *g_pLastRebelSpawn = NULL;
*/

extern CBaseEntity	*g_pLastSpawn;

#define MSS_COMMAND_MAX_RATE 0.3 // Limit frequency of entering console commands

// void DropPrimedFragGrenade( CMSS_Player *pPlayer, CBaseCombatWeapon *pGrenade ); // BOXBOX don't need this

LINK_ENTITY_TO_CLASS( player, CMSS_Player );

//LINK_ENTITY_TO_CLASS( info_player_combine, CPointEntity );
//LINK_ENTITY_TO_CLASS( info_player_rebel, CPointEntity );

IMPLEMENT_SERVERCLASS_ST(CMSS_Player, DT_MSS_Player)
	SendPropAngle( SENDINFO_VECTORELEM(m_angEyeAngles, 0), 11, SPROP_CHANGES_OFTEN ),
	SendPropAngle( SENDINFO_VECTORELEM(m_angEyeAngles, 1), 11, SPROP_CHANGES_OFTEN ),
	SendPropEHandle( SENDINFO( m_hRagdoll ) ),
	SendPropInt( SENDINFO( m_iSpawnInterpCounter), 4 ),
//	SendPropInt( SENDINFO( m_iPlayerSoundType), 3 ),

// BOXBOXBOX MSS STUFF
	SendPropInt( SENDINFO( m_nNumChars ), 3 ), // BOXBOX need 3 bits here
	SendPropString( SENDINFO( m_szCharName ) ),
	SendPropInt( SENDINFO( m_nGender ), 2 ), // BOXBOX need 2 bits here
	SendPropInt( SENDINFO( m_nRace ), 3 ), // BOXBOXBOX remember, only sending 3 bits limits races to 4 (0-3) so if we ever add more races than 4, more bit(s) will be needed!
	SendPropInt( SENDINFO( m_nTotalExp ), 21 ), // BOXBOX need 21 bits here because total exp. maxes out at 1 million

	//SendPropArray3( SENDINFO_ARRAY3( m_PreloadedCharInfo ), SendPropInt( SENDINFO_ARRAY(m_PreloadedCharInfo), 0, SendProxy_Preload ) ),
	//SendPropArray3( SENDINFO_ARRAY3( m_PreloadedCharInfo ), SendPropInt( SENDINFO_ARRAY(m_PreloadedCharInfo), 0, SendProxy_Preload ) ),
//	SendPropArray( SendPropString( SENDINFO_ARRAY( m_PreloadedCharInfo_Name ), 0, SendProxy_String_tToString ), m_PreloadedCharInfo_Name ),
	SendPropString( SENDINFO( m_szPreloadCharName0 ) ),
	SendPropString( SENDINFO( m_szPreloadCharName1 ) ),
	SendPropString( SENDINFO( m_szPreloadCharName2 ) ),

	SendPropArray( SendPropInt( SENDINFO_ARRAY( m_PreloadedCharInfo_Model ) ), m_PreloadedCharInfo_Model ),
	SendPropBool( SENDINFO( m_PreloadedCharInfo_DoneSending ) ),
	
	SendPropExclude( "DT_BaseAnimating", "m_flPoseParameter" ),
	SendPropExclude( "DT_BaseFlex", "m_viewtarget" ),

//	SendPropExclude( "DT_ServerAnimationData" , "m_flCycle" ),	
//	SendPropExclude( "DT_AnimTimeMustBeFirst" , "m_flAnimTime" ),
	
END_SEND_TABLE()

BEGIN_DATADESC( CMSS_Player )
END_DATADESC()


#define HL2MPPLAYER_PHYSDAMAGE_SCALE 4.0f

#pragma warning( disable : 4355 )

CMSS_Player::CMSS_Player() : m_PlayerAnimState( this )
{
	m_angEyeAngles.Init();

	m_iLastWeaponFireUsercmd = 0;

//	m_flNextModelChangeTime = 0.0f;
//	m_flNextTeamChangeTime = 0.0f;

	m_iSpawnInterpCounter = 0;

    m_bEnterObserver = false;
//	m_bReady = false;
//	m_bHasCharFile = false;

//	BaseClass::ChangeTeam( 0 );
	
//	UseClientSideAnimation();
}

CMSS_Player::~CMSS_Player( void )
{

}

void CMSS_Player::UpdateOnRemove( void )
{
	if ( m_hRagdoll )
	{
		UTIL_RemoveImmediate( m_hRagdoll );
		m_hRagdoll = NULL;
	}

	BaseClass::UpdateOnRemove();
}

void CMSS_Player::Precache( void )
{
	BaseClass::Precache();

	PrecacheModel ( "sprites/glow01.vmt" );

	PrecacheModel("models/player/humanmale.mdl"); // BOXBOX precache player models
//	PrecacheModel("models/player/humanfemale.mdl");

	PrecacheScriptSound( "Player_HumanMale.Die" );
	PrecacheScriptSound( "Player_HumanFemale.Die" );

}


void CMSS_Player::GiveAllItems( void )
{
	EquipSuit();

	GiveNamedItem( "weapon_rustyshortsword" );
	GiveNamedItem( "weapon_arozensword" );

/* BOXBOX removing old weapons
	CBasePlayer::GiveAmmo( 255,	"Pistol");
	CBasePlayer::GiveAmmo( 255,	"AR2" );
	CBasePlayer::GiveAmmo( 5,	"AR2AltFire" );
	CBasePlayer::GiveAmmo( 255,	"SMG1");
	CBasePlayer::GiveAmmo( 1,	"smg1_grenade");
	CBasePlayer::GiveAmmo( 255,	"Buckshot");
	CBasePlayer::GiveAmmo( 32,	"357" );
	CBasePlayer::GiveAmmo( 3,	"rpg_round");

	CBasePlayer::GiveAmmo( 1,	"grenade" );
	CBasePlayer::GiveAmmo( 2,	"slam" );

	GiveNamedItem( "weapon_crowbar" );
	GiveNamedItem( "weapon_stunstick" );
	GiveNamedItem( "weapon_pistol" );
	GiveNamedItem( "weapon_357" );

	GiveNamedItem( "weapon_smg1" );
	GiveNamedItem( "weapon_ar2" );
	
	GiveNamedItem( "weapon_shotgun" );
	GiveNamedItem( "weapon_frag" );
	
	GiveNamedItem( "weapon_crossbow" );
	
	GiveNamedItem( "weapon_rpg" );

	GiveNamedItem( "weapon_slam" );

	GiveNamedItem( "weapon_physcannon" );
*/	
}

/*  BOXBOX don't need this
void CMSS_Player::GiveDefaultItems( void )
{
	EquipSuit();

	CBasePlayer::GiveAmmo( 255,	"Pistol");
	CBasePlayer::GiveAmmo( 45,	"SMG1");
	CBasePlayer::GiveAmmo( 1,	"grenade" );
	CBasePlayer::GiveAmmo( 6,	"Buckshot");
	CBasePlayer::GiveAmmo( 6,	"357" );

	if ( GetPlayerModelType() == PLAYER_SOUNDS_METROPOLICE || GetPlayerModelType() == PLAYER_SOUNDS_COMBINESOLDIER )
	{
		GiveNamedItem( "weapon_stunstick" );
	}
	else if ( GetPlayerModelType() == PLAYER_SOUNDS_CITIZEN )
	{
		GiveNamedItem( "weapon_crowbar" );
	}
	
	GiveNamedItem( "weapon_pistol" );
	GiveNamedItem( "weapon_smg1" );
	GiveNamedItem( "weapon_frag" );
	GiveNamedItem( "weapon_physcannon" );

	const char *szDefaultWeaponName = engine->GetClientConVarValue( engine->IndexOfEdict( edict() ), "cl_defaultweapon" );

	CBaseCombatWeapon *pDefaultWeapon = Weapon_OwnsThisType( szDefaultWeaponName );

	if ( pDefaultWeapon )
	{
		Weapon_Switch( pDefaultWeapon );
	}
	else
	{
		Weapon_Switch( Weapon_OwnsThisType( "weapon_physcannon" ) );
	}

}
*/

/* BOXBOX removing teams

void CMSS_Player::PickDefaultSpawnTeam( void )
{
	/*
	if ( GetTeamNumber() == 0 )
	{
		if ( MSSRules()->IsTeamplay() == false )
		{
			if ( GetModelPtr() == NULL )
			{
				const char *szModelName = NULL;
				szModelName = engine->GetClientConVarValue( engine->IndexOfEdict( edict() ), "cl_playermodel" );

				if ( ValidatePlayerModel( szModelName ) == false )
				{
					char szReturnString[512];

					Q_snprintf( szReturnString, sizeof (szReturnString ), "cl_playermodel models/combine_soldier.mdl\n" );
					engine->ClientCommand ( edict(), szReturnString );
				}

				ChangeTeam( TEAM_UNASSIGNED );
			}
		}
		else
		{
			CTeam *pCombine = g_Teams[TEAM_COMBINE];
			CTeam *pRebels = g_Teams[TEAM_REBELS];

			if ( pCombine == NULL || pRebels == NULL )
			{
				ChangeTeam( random->RandomInt( TEAM_COMBINE, TEAM_REBELS ) );
			}
			else
			{
				if ( pCombine->GetNumPlayers() > pRebels->GetNumPlayers() )
				{
					ChangeTeam( TEAM_REBELS );
				}
				else if ( pCombine->GetNumPlayers() < pRebels->GetNumPlayers() )
				{
					ChangeTeam( TEAM_COMBINE );
				}
				else
				{
					ChangeTeam( random->RandomInt( TEAM_COMBINE, TEAM_REBELS ) );
				}
			}
		}
	}

}
*/


void CMSS_Player::MoveToIntroCamera() // BOXBOX here goes nothing
{
	EHANDLE hIntroCamera = gEntList.FindEntityByClassname( hIntroCamera, "info_player_start" );

	// if hIntroCamera is NULL we just were at end of list, start searching from start again
	if(!hIntroCamera)
		hIntroCamera = gEntList.FindEntityByClassname( hIntroCamera, "info_player_start");

	// BOXBOX remove target business
/*
	CBaseEntity *Target = NULL;
	
	if( hIntroCamera )
	{
		Target = gEntList.FindEntityByName( NULL, STRING(hIntroCamera->m_target) );
	}
*/
	// if we still couldn't find a camera, goto T spawn
//	if(!hIntroCamera)
//		hIntroCamera = gEntList.FindEntityByClassname(hIntroCamera, "info_player_terrorist");

	SetViewOffset( vec3_origin );	// no view offset
	UTIL_SetSize( this, vec3_origin, vec3_origin ); // no bbox

//	if( !Target ) //if there are no cameras(or the camera has no target, find a spawn point and black out the screen
//	{

//	if ( hIntroCamera.IsValid() )
	
	SetAbsOrigin( hIntroCamera->GetAbsOrigin() /*+ VEC_VIEW*/ );
	SnapEyeAngles( hIntroCamera->GetAbsAngles() );

//		SetAbsAngles( hIntroCamera->GetAbsAngles() );

//		hIntroCamera = NULL;  // never update again
//		return;
//	}
/*	
	Vector vCamera = Target->GetAbsOrigin() - hIntroCamera->GetAbsOrigin();
	Vector vIntroCamera = hIntroCamera->GetAbsOrigin();
	
	VectorNormalize( vCamera );
		
	QAngle CamAngles;
	VectorAngles( vCamera, CamAngles );

	SetAbsOrigin( vIntroCamera );
	SetAbsAngles( CamAngles );
	SnapEyeAngles( CamAngles );
//	m_fIntroCamTime = gpGlobals->curtime + 6;
*/
}


void CMSS_Player::Spawn(void)
{
//	m_flNextModelChangeTime = 0.0f;
//	m_flNextTeamChangeTime = 0.0f;

//	PickDefaultSpawnTeam(); // BOXBOX removing teams

	BaseClass::Spawn();
	
	if ( !IsObserver() )
	{
		pl.deadflag = false;
		RemoveSolidFlags( FSOLID_NOT_SOLID );

		RemoveEffects( EF_NODRAW );
		
//		GiveDefaultItems(); // BOXBOX don't need this
	}

	SetNumAnimOverlays( 3 );
	ResetAnimation();

	m_nRenderFX = kRenderNormal;

	m_Local.m_iHideHUD = 0;
	
	AddFlag(FL_ONGROUND); // set the player on the ground at the start of the round.

	m_impactEnergyScale = HL2MPPLAYER_PHYSDAMAGE_SCALE;

// BOXBOX changing this
//	if ( MSSRules()->IsIntermission() )
//	{
//		AddFlag( FL_FROZEN );
//	}
//	else
//	{
		RemoveFlag( FL_FROZEN );
//	}

	m_iSpawnInterpCounter = (m_iSpawnInterpCounter + 1) % 8;

	m_Local.m_bDucked = false;

	SetPlayerUnderwater(false);

//	m_bReady = false;
}

void CMSS_Player::PickupObject( CBaseEntity *pObject, bool bLimitMassAndSize )
{
	
}

/*
bool CMSS_Player::ValidatePlayerModel( const char *pModel )
{
	int iModels = ARRAYSIZE( g_ppszRandomCitizenModels );
	int i;	

	for ( i = 0; i < iModels; ++i )
	{
		if ( !Q_stricmp( g_ppszRandomCitizenModels[i], pModel ) )
		{
			return true;
		}
	}

	iModels = ARRAYSIZE( g_ppszRandomCombineModels );

	for ( i = 0; i < iModels; ++i )
	{
	   	if ( !Q_stricmp( g_ppszRandomCombineModels[i], pModel ) )
		{
			return true;
		}
	}

	return false;
}
*/

/*
void CMSS_Player::SetPlayerTeamModel( void ) // BOXBOX removing teams
{

	const char *szModelName = NULL;
	szModelName = engine->GetClientConVarValue( engine->IndexOfEdict( edict() ), "cl_playermodel" );

	int modelIndex = modelinfo->GetModelIndex( szModelName );

	if ( modelIndex == -1 || ValidatePlayerModel( szModelName ) == false )
	{
		szModelName = "models/Combine_Soldier.mdl";
		m_iModelType = TEAM_COMBINE;

		char szReturnString[512];

		Q_snprintf( szReturnString, sizeof (szReturnString ), "cl_playermodel %s\n", szModelName );
		engine->ClientCommand ( edict(), szReturnString );
	}

	if ( GetTeamNumber() == TEAM_COMBINE )
	{
		if ( Q_stristr( szModelName, "models/human") )
		{
			int nHeads = ARRAYSIZE( g_ppszRandomCombineModels );
		
			g_iLastCombineModel = ( g_iLastCombineModel + 1 ) % nHeads;
			szModelName = g_ppszRandomCombineModels[g_iLastCombineModel];
		}

		m_iModelType = TEAM_COMBINE;
	}
	else if ( GetTeamNumber() == TEAM_REBELS )
	{
		if ( !Q_stristr( szModelName, "models/human") )
		{
			int nHeads = ARRAYSIZE( g_ppszRandomCitizenModels );

			g_iLastCitizenModel = ( g_iLastCitizenModel + 1 ) % nHeads;
			szModelName = g_ppszRandomCitizenModels[g_iLastCitizenModel];
		}

		m_iModelType = TEAM_REBELS;
	}
	
	SetModel( szModelName );
	SetupPlayerSoundsByModel( szModelName );

	m_flNextModelChangeTime = gpGlobals->curtime + MODEL_CHANGE_INTERVAL;
}


void CMSS_Player::SetPlayerModel( void ) // BOXBOX don't need this
{

	const char *szModelName = NULL;
	const char *pszCurrentModelName = modelinfo->GetModelName( GetModel());

	szModelName = engine->GetClientConVarValue( engine->IndexOfEdict( edict() ), "cl_playermodel" );

	if ( ValidatePlayerModel( szModelName ) == false )
	{
		char szReturnString[512];

		if ( ValidatePlayerModel( pszCurrentModelName ) == false )
		{
			pszCurrentModelName = "models/Combine_Soldier.mdl";
		}

		Q_snprintf( szReturnString, sizeof (szReturnString ), "cl_playermodel %s\n", pszCurrentModelName );
		engine->ClientCommand ( edict(), szReturnString );

		szModelName = pszCurrentModelName;
	}

	if ( GetTeamNumber() == TEAM_COMBINE )
	{
		int nHeads = ARRAYSIZE( g_ppszRandomCombineModels );
		
		g_iLastCombineModel = ( g_iLastCombineModel + 1 ) % nHeads;
		szModelName = g_ppszRandomCombineModels[g_iLastCombineModel];

		m_iModelType = TEAM_COMBINE;
	}
	else if ( GetTeamNumber() == TEAM_REBELS )
	{
		int nHeads = ARRAYSIZE( g_ppszRandomCitizenModels );

		g_iLastCitizenModel = ( g_iLastCitizenModel + 1 ) % nHeads;
		szModelName = g_ppszRandomCitizenModels[g_iLastCitizenModel];

		m_iModelType = TEAM_REBELS;
	}
	else
	{
		if ( Q_strlen( szModelName ) == 0 ) 
		{
			szModelName = g_ppszRandomCitizenModels[0];
		}

		if ( Q_stristr( szModelName, "models/human") )
		{
			m_iModelType = TEAM_REBELS;
		}
		else
		{
			m_iModelType = TEAM_COMBINE;
		}
	}

	int modelIndex = modelinfo->GetModelIndex( szModelName );

	if ( modelIndex == -1 )
	{
		szModelName = "models/Combine_Soldier.mdl";
		m_iModelType = TEAM_COMBINE;

		char szReturnString[512];

		Q_snprintf( szReturnString, sizeof (szReturnString ), "cl_playermodel %s\n", szModelName );
		engine->ClientCommand ( edict(), szReturnString );
	}

	SetModel( szModelName );
	SetupPlayerSoundsByModel( szModelName );

	m_flNextModelChangeTime = gpGlobals->curtime + MODEL_CHANGE_INTERVAL;
}
*/

/* // BOXBOX removing, may be useful in the future when multiple races come in to play
void CMSS_Player::SetupPlayerSoundsByModel( const char *pModelName )
{

	if ( Q_stristr( pModelName, "models/human") )
	{
		m_iPlayerSoundType = (int)PLAYER_SOUNDS_CITIZEN;
	}
	else if ( Q_stristr(pModelName, "police" ) )
	{
		m_iPlayerSoundType = (int)PLAYER_SOUNDS_METROPOLICE;
	}
	else if ( Q_stristr(pModelName, "combine" ) )
	{
		m_iPlayerSoundType = (int)PLAYER_SOUNDS_COMBINESOLDIER;
	}
}
*/

void CMSS_Player::ResetAnimation( void )
{
	if ( IsAlive() )
	{
		SetSequence ( -1 );
		SetActivity( ACT_INVALID );

		if (!GetAbsVelocity().x && !GetAbsVelocity().y)
			SetAnimation( PLAYER_IDLE );
		else if ((GetAbsVelocity().x || GetAbsVelocity().y) && ( GetFlags() & FL_ONGROUND ))
			SetAnimation( PLAYER_WALK );
		else if (GetWaterLevel() > 1)
			SetAnimation( PLAYER_WALK );
	}
}


bool CMSS_Player::Weapon_Switch( CBaseCombatWeapon *pWeapon, int viewmodelindex )
{
	bool bRet = BaseClass::Weapon_Switch( pWeapon, viewmodelindex );

	if ( bRet == true )
	{
		ResetAnimation();
	}

	return bRet;
}

void CMSS_Player::PreThink( void )
{
	QAngle vOldAngles = GetLocalAngles();
	QAngle vTempAngles = GetLocalAngles();

	vTempAngles = EyeAngles();

	if ( vTempAngles[PITCH] > 180.0f )
	{
		vTempAngles[PITCH] -= 360.0f;
	}

	SetLocalAngles( vTempAngles );

	BaseClass::PreThink();
	State_PreThink();

	//Reset bullet force accumulator, only lasts one frame
	m_vecTotalBulletForce = vec3_origin;
	SetLocalAngles( vOldAngles );
}

void CMSS_Player::PostThink( void )
{
	BaseClass::PostThink();
	
	if ( GetFlags() & FL_DUCKING )
	{
		SetCollisionBounds( VEC_CROUCH_TRACE_MIN, VEC_CROUCH_TRACE_MAX );
	}

	m_PlayerAnimState.Update();

	// Store the eye angles pitch so the client can compute its animation state correctly.
	m_angEyeAngles = EyeAngles();

	QAngle angles = GetLocalAngles();
	angles[PITCH] = 0;
	SetLocalAngles( angles );


// BOXBOXBOX MSS STUFF
	//Send the characters to the client
	//This code is in PostThink, because when you click "disconnect" and choose a new map,
	//the game marks your steam id as "STEAM_ID_PENDING" for a few seconds (Spawn() is called during this time).
//	if( !m_PreloadedCharInfo_DoneSending )
//	{
		//This is the check that should be running... but for some reason sv_lan is always true, even when running an internet game
		//if( cvar->FindVar( "sv_lan" )->GetBool() || Q_strcmp(this->GetNetworkIDString( ),"STEAM_ID_PENDING") )

/*		if( Q_strcmp(this->GetNetworkIDString( ),"STEAM_ID_PENDING") )
		{
			static char names[MAX_CHAR_SLOTS][100];		//It's absurd that I have to do this, but otherwise the m_PreloadedChar_Name won't store all three names
			for( int i = 0; i < MAX_CHAR_SLOTS; i++ )
			{
				charloadstatus_e status = CMSS_Player::LoadChar( i );
				if( status == CHARLOAD_STATUS_OK )
				{
					V_strncpy( names[i], m_szCharName.GetForModify(), sizeof(names[i]) );
					int model = 32;
//					m_PreloadedCharInfo_Name.Set( i, MAKE_STRING(names[i]) );
					m_PreloadedCharInfo_Model.Set( i, model );
				}
*/

//				m_PreloadedCharInfo_DoneSending = true;
//			}
//		}
//	}
}

void CMSS_Player::PlayerDeathThink()
{
	if( !IsObserver() )
	{
		BaseClass::PlayerDeathThink();
	}
}

void CMSS_Player::FireBullets ( const FireBulletsInfo_t &info )
{
	// Move other players back to history positions based on local player's lag
	lagcompensation->StartLagCompensation( this, this->GetCurrentCommand() );

	FireBulletsInfo_t modinfo = info;

	CWeaponMSSBase *pWeapon = dynamic_cast<CWeaponMSSBase *>( GetActiveWeapon() );

	if ( pWeapon )
	{
		modinfo.m_iPlayerDamage = modinfo.m_flDamage = pWeapon->GetHL2MPWpnData().m_iPlayerDamage;
	}

	NoteWeaponFired();

	BaseClass::FireBullets( modinfo );

	// Move other players back to history positions based on local player's lag
	lagcompensation->FinishLagCompensation( this );
}

void CMSS_Player::NoteWeaponFired( void )
{
	Assert( m_pCurrentCommand );
	if( m_pCurrentCommand )
	{
		m_iLastWeaponFireUsercmd = m_pCurrentCommand->command_number;
	}
}

extern ConVar sv_maxunlag;

bool CMSS_Player::WantsLagCompensationOnEntity( const CBasePlayer *pPlayer, const CUserCmd *pCmd, const CBitVec<MAX_EDICTS> *pEntityTransmitBits ) const
{
	// No need to lag compensate at all if we're not attacking in this command and
	// we haven't attacked recently.
	if ( !( pCmd->buttons & IN_ATTACK ) && (pCmd->command_number - m_iLastWeaponFireUsercmd > 5) )
		return false;

	// If this entity hasn't been transmitted to us and acked, then don't bother lag compensating it.
	if ( pEntityTransmitBits && !pEntityTransmitBits->Get( pPlayer->entindex() ) )
		return false;

	const Vector &vMyOrigin = GetAbsOrigin();
	const Vector &vHisOrigin = pPlayer->GetAbsOrigin();

	// get max distance player could have moved within max lag compensation time, 
	// multiply by 1.5 to to avoid "dead zones"  (sqrt(2) would be the exact value)
	float maxDistance = 1.5 * pPlayer->MaxSpeed() * sv_maxunlag.GetFloat();

	// If the player is within this distance, lag compensate them in case they're running past us.
	if ( vHisOrigin.DistTo( vMyOrigin ) < maxDistance )
		return true;

	// If their origin is not within a 45 degree cone in front of us, no need to lag compensate.
	Vector vForward;
	AngleVectors( pCmd->viewangles, &vForward );
	
	Vector vDiff = vHisOrigin - vMyOrigin;
	VectorNormalize( vDiff );

	float flCosAngle = 0.707107f;	// 45 degree angle
	if ( vForward.Dot( vDiff ) < flCosAngle )
		return false;

	return true;
}

/* // BOXBOX removing teams
Activity CMSS_Player::TranslateTeamActivity( Activity ActToTranslate )
{

	if ( m_iModelType == TEAM_COMBINE )
		 return ActToTranslate;
	
	if ( ActToTranslate == ACT_RUN )
		 return ACT_RUN_AIM_AGITATED;

	if ( ActToTranslate == ACT_IDLE )
		 return ACT_IDLE_AIM_AGITATED;

	if ( ActToTranslate == ACT_WALK )
		 return ACT_WALK_AIM_AGITATED;

	return ActToTranslate;
}
*/

extern ConVar hl2_normspeed;

// Set the activity based on an event or current state
void CMSS_Player::SetAnimation( PLAYER_ANIM playerAnim )
{
	int animDesired;

	float speed;

	speed = GetAbsVelocity().Length2D();

	
	// bool bRunning = true;

	//Revisit!
/*	if ( ( m_nButtons & ( IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT ) ) )
	{
		if ( speed > 1.0f && speed < hl2_normspeed.GetFloat() - 20.0f )
		{
			bRunning = false;
		}
	}*/

	if ( GetFlags() & ( FL_FROZEN | FL_ATCONTROLS ) )
	{
		speed = 0;
		playerAnim = PLAYER_IDLE;
	}

	Activity idealActivity = ACT_HL2MP_RUN;

	// This could stand to be redone. Why is playerAnim abstracted from activity? (sjb)
	if ( playerAnim == PLAYER_JUMP )
	{
		idealActivity = ACT_HL2MP_JUMP;
	}
	else if ( playerAnim == PLAYER_DIE )
	{
		if ( m_lifeState == LIFE_ALIVE )
		{
			return;
		}
	}
	else if ( playerAnim == PLAYER_ATTACK1 )
	{
		if ( GetActivity( ) == ACT_HOVER	|| 
			 GetActivity( ) == ACT_SWIM		||
			 GetActivity( ) == ACT_HOP		||
			 GetActivity( ) == ACT_LEAP		||
			 GetActivity( ) == ACT_DIESIMPLE )
		{
			idealActivity = GetActivity( );
		}
		else
		{
			idealActivity = ACT_HL2MP_GESTURE_RANGE_ATTACK;
		}
	}
	else if ( playerAnim == PLAYER_RELOAD )
	{
		idealActivity = ACT_HL2MP_GESTURE_RELOAD;
	}
	else if ( playerAnim == PLAYER_IDLE || playerAnim == PLAYER_WALK )
	{
		if ( !( GetFlags() & FL_ONGROUND ) && GetActivity( ) == ACT_HL2MP_JUMP )	// Still jumping
		{
			idealActivity = GetActivity( );
		}
		/*
		else if ( GetWaterLevel() > 1 )
		{
			if ( speed == 0 )
				idealActivity = ACT_HOVER;
			else
				idealActivity = ACT_SWIM;
		}
		*/
		else
		{
			if ( GetFlags() & FL_DUCKING )
			{
				if ( speed > 0 )
				{
					idealActivity = ACT_HL2MP_WALK_CROUCH;
				}
				else
				{
					idealActivity = ACT_HL2MP_IDLE_CROUCH;
				}
			}
			else
			{
				if ( speed > 0 )
				{
					/*
					if ( bRunning == false )
					{
						idealActivity = ACT_WALK;
					}
					else
					*/
					{
						idealActivity = ACT_HL2MP_RUN;
					}
				}
				else
				{
					idealActivity = ACT_HL2MP_IDLE;
				}
			}
		}

//		idealActivity = TranslateTeamActivity( idealActivity ); // BOXBOX not needed
	}
	
	if ( idealActivity == ACT_HL2MP_GESTURE_RANGE_ATTACK )
	{
		RestartGesture( Weapon_TranslateActivity( idealActivity ) );

		// FIXME: this seems a bit wacked
		Weapon_SetActivity( Weapon_TranslateActivity( ACT_RANGE_ATTACK1 ), 0 );

		return;
	}
	else if ( idealActivity == ACT_HL2MP_GESTURE_RELOAD )
	{
		RestartGesture( Weapon_TranslateActivity( idealActivity ) );
		return;
	}
	else
	{
		SetActivity( idealActivity );

		animDesired = SelectWeightedSequence( Weapon_TranslateActivity ( idealActivity ) );

		if (animDesired == -1)
		{
			animDesired = SelectWeightedSequence( idealActivity );

			if ( animDesired == -1 )
			{
				animDesired = 0;
			}
		}
	
		// Already using the desired animation?
		if ( GetSequence() == animDesired )
			return;

		m_flPlaybackRate = 1.0;
		ResetSequence( animDesired );
		SetCycle( 0 );
		return;
	}

	// Already using the desired animation?
	if ( GetSequence() == animDesired )
		return;

	//Msg( "Set animation to %d\n", animDesired );
	// Reset to first frame of desired animation
	ResetSequence( animDesired );
	SetCycle( 0 );
}


extern int	gEvilImpulse101;
//-----------------------------------------------------------------------------
// Purpose: Player reacts to bumping a weapon. 
// Input  : pWeapon - the weapon that the player bumped into.
// Output : Returns true if player picked up the weapon
//-----------------------------------------------------------------------------
bool CMSS_Player::BumpWeapon( CBaseCombatWeapon *pWeapon )
{
	CBaseCombatCharacter *pOwner = pWeapon->GetOwner();

	// Can I have this weapon type?
	if ( !IsAllowedToPickupWeapons() )
		return false;

	if ( pOwner || !Weapon_CanUse( pWeapon ) || !g_pGameRules->CanHavePlayerItem( this, pWeapon ) )
	{
		if ( gEvilImpulse101 )
		{
			UTIL_Remove( pWeapon );
		}
		return false;
	}

	// Don't let the player fetch weapons through walls (use MASK_SOLID so that you can't pickup through windows)
	if( !pWeapon->FVisible( this, MASK_SOLID ) && !(GetFlags() & FL_NOTARGET) )
	{
		return false;
	}

	bool bOwnsWeaponAlready = !!Weapon_OwnsThisType( pWeapon->GetClassname(), pWeapon->GetSubType());

	if ( bOwnsWeaponAlready == true ) 
	{
		//If we have room for the ammo, then "take" the weapon too.
		 if ( Weapon_EquipAmmoOnly( pWeapon ) )
		 {
			 pWeapon->CheckRespawn();

			 UTIL_Remove( pWeapon );
			 return true;
		 }
		 else
		 {
			 return false;
		 }
	}

	pWeapon->CheckRespawn();
	Weapon_Equip( pWeapon );

	return true;
}

/*
void CMSS_Player::ChangeTeam( int iTeam )
{
	if ( GetNextTeamChangeTime() >= gpGlobals->curtime )
	{
		char szReturnString[128];
		Q_snprintf( szReturnString, sizeof( szReturnString ), "Please wait %d more seconds before trying to switch teams again.\n", (int)(GetNextTeamChangeTime() - gpGlobals->curtime) );

		ClientPrint( this, HUD_PRINTTALK, szReturnString );
		return;
	}


	bool bKill = false;

	if ( MSSRules()->IsTeamplay() != true && iTeam != TEAM_SPECTATOR )
	{
		//don't let them try to join combine or rebels during deathmatch.
		iTeam = TEAM_UNASSIGNED;
	}

	if ( MSSRules()->IsTeamplay() == true )
	{
		if ( iTeam != GetTeamNumber() && GetTeamNumber() != TEAM_UNASSIGNED )
		{
			bKill = true;
		}
	}

	BaseClass::ChangeTeam( iTeam );

	m_flNextTeamChangeTime = gpGlobals->curtime + TEAM_CHANGE_INTERVAL;

	if ( MSSRules()->IsTeamplay() == true )
	{
		SetPlayerTeamModel();
	}
	else
	{
		SetPlayerModel();
	}

	if ( iTeam == TEAM_SPECTATOR )
	{
		RemoveAllItems( true );

		State_Transition( STATE_OBSERVER_MODE );
	}

	if ( bKill == true )
	{
		CommitSuicide();
	}
}
*/

/*  BOXBOX replacing this function with one below to allow spectating without teams

bool CMSS_Player::HandleCommand_JoinTeam( int team )
{

	if ( !GetGlobalTeam( team ) || team == 0 )
	{
		Warning( "HandleCommand_JoinTeam( %d ) - invalid team index.\n", team );
		return false;
	}

	if ( team == TEAM_SPECTATOR )
	{
		// Prevent this is the cvar is set
		if ( !mp_allowspectators.GetInt() )
		{
			ClientPrint( this, HUD_PRINTCENTER, "#Cannot_Be_Spectator" );
			return false;
		}

		if ( GetTeamNumber() != TEAM_UNASSIGNED && !IsDead() )
		{
			m_fNextSuicideTime = gpGlobals->curtime;	// allow the suicide to work

			CommitSuicide();

			// add 1 to frags to balance out the 1 subtracted for killing yourself
			IncrementFragCount( 1 );
		}

		ChangeTeam( TEAM_SPECTATOR );

		return true;
	}
	else
	{
		StopObserverMode();
		State_Transition(STATE_ACTIVE);
	}

	// Switch their actual team...
	ChangeTeam( team );

	return true;
}
*/


bool CMSS_Player::ClientCommand( const CCommand &args )
{
	if ( FStrEq( args[0], "spectate" ) )
	{
		if ( ShouldRunRateLimitedCommand( args ) ) // Stop players from spamming console commands
		{
//			HandleCommand_JoinTeam( TEAM_SPECTATOR ); // BOXBOX replacing this with line below
			HandleCommand_Spectate(); // BOXBOX adding this function to keep spectating without teams
		}
		return true;
	}

	// BOXBOX adding this
	else if ( !Q_stricmp( args[0], "dropweapon" ) )
	{
		CBaseCombatWeapon *pWeapon = GetActiveWeapon();
		if( pWeapon )
		{
			Msg("Dropping %s\n", pWeapon->GetName() );
		}
		else
		{
			Msg("No weapon in hand!\n");
		}
		Weapon_Drop( GetActiveWeapon() );
		return true;
	}

	// BOXBOX adding this
	else if ( !Q_stricmp( args[0], "currentmap" ) )
	{
		Msg("Current map is: %s\n", STRING(gpGlobals->mapname) );
		return true;
	}
	else if ( FStrEq( args[0], "joingame" ) )
	{
		return true;
	}

	//BOXBOX MSS stuff
	else if ( FStrEq( args[0], "getchars" ) )
	{
		if ( args.ArgC() == 1 )
		{
			PreLoadChars();
		}
	}
	else if ( FStrEq( args[0], "choosechar" ) )
	{
		if ( args.ArgC() == 2 )
		{
			int charSlot = atoi( args[1] );
			charSlot = min( charSlot, (MAX_CHAR_SLOTS-1) );
			charSlot = max( charSlot, 0 );

			Msg(UTIL_VarArgs("charSlot = %i\n",charSlot)); //So it's right so far...

			charloadstatus_e LoadStatus = LoadChar( charSlot );
			if( LoadStatus == CHARLOAD_STATUS_OK
				/*|| LoadStatus == CHARLOAD_STATUS_FILE_NOT_FOUND*/ )
			{
				m_HasChoosenChar = true;
				m_SelectedChar = charSlot;

				SetViewEntity( NULL );
				ChangeTeam( TEAM_UNASSIGNED );

				Spawn( );
			}
			else
			{
				const int userid = GetUserID();
				const char *networkID = GetNetworkIDString();
				const char *playerName = GetPlayerName(); 

				char filePath[MAX_PATH];
				CharacterSave::GetSaveFileNameForPlayer( this, charSlot, filePath );
				UTIL_LogPrintf( "\"%s<%i><%s>\" Attempted to load hacked file! \"%s\"\n", playerName, userid, networkID, filePath );
			}
			StopSound("Music.Intro"); // BOXBOX added
			return true;
		}
	}
	else if ( FStrEq( args[0], "createchar" ) )
	{
		if( args.ArgC() == 4 )
		{
			Warning("Received command %s %s %i %i\n", args[0], args[1], atoi( args[2] ), atoi( args[3] ) );

			//Check for open slot
			int openSlot = -1;

			for( int i = 0; i < MAX_CHAR_SLOTS; i++ )
			{
				charloadstatus_e status = CMSS_Player::LoadChar( i );
				if( status == CHARLOAD_STATUS_FILE_NOT_FOUND )
				{
					openSlot = i;
					break;
				}
			}

			if( openSlot > -1 )
			{
				//Found open slot
				m_HasChoosenChar = true;
				m_SelectedChar = openSlot;

				//Put New Character initial values here
				V_strncpy( m_szCharName.GetForModify(), args[1], MAX_CHAR_NAME_LENGTH );
				m_nGender = atoi( args[2] );
				m_nRace = atoi( args[3] );

				SetViewEntity( NULL );

				Spawn( );
	//			SetPlayerName( m_szCharName.Get( ) );

				//Setting everything to 0 to avoid it saving former characters' stats
				ms_warriorSkills = 0;
				ms_martialArts = 0;
				ms_smallArms = 0;
				ms_archery = 0;
				ms_spellCasting = 0;
				ms_parry = 0;

				SaveChar( );
				StopSound("Music.Intro"); // BOXBOX added
				PreLoadChars(); // BOXBOX number of characters just changed
			}
			else	
				//No slots open!
				ShowViewPortPanel( PANEL_CHARSELECT , true );
		}

		return true;
	}
	else if ( FStrEq( args[0], "deletechar" ) ) // BOXBOX added
	{
		if ( args.ArgC() == 2 )
		{
			int charSlot = atoi( args[1] );
			charSlot = min( charSlot, (MAX_CHAR_SLOTS-1) );
			charSlot = max( charSlot, 0 );

			char filePath[MAX_PATH];
			CharacterSave::GetSaveFileNameForPlayer( this, charSlot, filePath );

/*
	BOXBOX TODO Set this up so instead of deleting the file, it renames the file with "_deleted" at the end, in case a player changes their mind and wants the character back
	in the future, in which case a server op will have to go into the characters folder on the server and remove the "_deleted" from the filename (which will override any
	character currently in that slot, make sure the player is aware of this!)
*/
			g_pFullFileSystem->RemoveFile( filePath ); // feel the power

			PreLoadChars(); // BOXBOX so character menus update on character deletion

//			Warning("Received command  deletechar %i\n", atoi( args[1] ));
//			Warning( "*** Filepath received is: %s ***\n", filePath );
			return true;
		}
	}
	else if ( FStrEq( args[0], "charselect" ) ) // BOXBOX open the Character Selection menu in-game.
	{
		if( args.ArgC() == 1 )
		{
			ShowViewPortPanel( PANEL_CHARSELECT );
			return true;
		}
	}
	else if ( FStrEq( args[0], "mainmenu" ) ) // BOXBOX toggle the main MSS menu
	{
		if( args.ArgC() == 1 )
		{
			ShowViewPortPanel( PANEL_MAINMENU );
			return true;
		}
	}

	return BaseClass::ClientCommand( args );
}

void CMSS_Player::CheatImpulseCommands( int iImpulse )
{
	switch ( iImpulse )
	{
		case 101:
			{
				if( sv_cheats->GetBool() )
				{
					GiveAllItems();
				}
			}
			break;

		default:
			BaseClass::CheatImpulseCommands( iImpulse );
	}
}

bool CMSS_Player::ShouldRunRateLimitedCommand( const CCommand &args )
{
	int i = m_RateLimitLastCommandTimes.Find( args[0] );
	if ( i == m_RateLimitLastCommandTimes.InvalidIndex() )
	{
		m_RateLimitLastCommandTimes.Insert( args[0], gpGlobals->curtime );
		return true;
	}
	else if ( (gpGlobals->curtime - m_RateLimitLastCommandTimes[i]) < MSS_COMMAND_MAX_RATE )
	{
		// Too fast.
		return false;
	}
	else
	{
		m_RateLimitLastCommandTimes[i] = gpGlobals->curtime;
		return true;
	}
}

void CMSS_Player::CreateViewModel( int index /*=0*/ )
{
	Assert( index >= 0 && index < MAX_VIEWMODELS );

	if ( GetViewModel( index ) )
		return;

	CPredictedViewModel *vm = ( CPredictedViewModel * )CreateEntityByName( "predicted_viewmodel" );
	if ( vm )
	{
		vm->SetAbsOrigin( GetAbsOrigin() );
		vm->SetOwner( this );
		vm->SetIndex( index );
		DispatchSpawn( vm );
		vm->FollowEntity( this, false );
		m_hViewModel.Set( index, vm );
	}
}

bool CMSS_Player::BecomeRagdollOnClient( const Vector &force )
{
	return true;
}

// -------------------------------------------------------------------------------- //
// Ragdoll entities.
// -------------------------------------------------------------------------------- //

class CHL2MPRagdoll : public CBaseAnimatingOverlay
{
public:
	DECLARE_CLASS( CHL2MPRagdoll, CBaseAnimatingOverlay );
	DECLARE_SERVERCLASS();

	// Transmit ragdolls to everyone.
	virtual int UpdateTransmitState()
	{
		return SetTransmitState( FL_EDICT_ALWAYS );
	}

public:
	// In case the client has the player entity, we transmit the player index.
	// In case the client doesn't have it, we transmit the player's model index, origin, and angles
	// so they can create a ragdoll in the right place.
	CNetworkHandle( CBaseEntity, m_hPlayer );	// networked entity handle 
	CNetworkVector( m_vecRagdollVelocity );
	CNetworkVector( m_vecRagdollOrigin );
};

LINK_ENTITY_TO_CLASS( hl2mp_ragdoll, CHL2MPRagdoll );

IMPLEMENT_SERVERCLASS_ST_NOBASE( CHL2MPRagdoll, DT_HL2MPRagdoll )
	SendPropVector( SENDINFO(m_vecRagdollOrigin), -1,  SPROP_COORD ),
	SendPropEHandle( SENDINFO( m_hPlayer ) ),
	SendPropModelIndex( SENDINFO( m_nModelIndex ) ),
	SendPropInt		( SENDINFO(m_nForceBone), 8, 0 ),
	SendPropVector	( SENDINFO(m_vecForce), -1, SPROP_NOSCALE ),
	SendPropVector( SENDINFO( m_vecRagdollVelocity ) )
END_SEND_TABLE()


void CMSS_Player::CreateRagdollEntity( void )
{
	if ( m_hRagdoll )
	{
		UTIL_RemoveImmediate( m_hRagdoll );
		m_hRagdoll = NULL;
	}

	// If we already have a ragdoll, don't make another one.
	CHL2MPRagdoll *pRagdoll = dynamic_cast< CHL2MPRagdoll* >( m_hRagdoll.Get() );
	
	if ( !pRagdoll )
	{
		// create a new one
		pRagdoll = dynamic_cast< CHL2MPRagdoll* >( CreateEntityByName( "hl2mp_ragdoll" ) );
	}

	if ( pRagdoll )
	{
		pRagdoll->m_hPlayer = this;
		pRagdoll->m_vecRagdollOrigin = GetAbsOrigin();
		pRagdoll->m_vecRagdollVelocity = GetAbsVelocity();
		pRagdoll->m_nModelIndex = m_nModelIndex;
		pRagdoll->m_nForceBone = m_nForceBone;
		pRagdoll->m_vecForce = m_vecTotalBulletForce;
		pRagdoll->SetAbsOrigin( GetAbsOrigin() );
	}

	// ragdolls will be removed on round restart automatically
	m_hRagdoll = pRagdoll;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CMSS_Player::FlashlightIsOn( void )
{
	return IsEffectActive( EF_DIMLIGHT );
}

extern ConVar flashlight;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CMSS_Player::FlashlightTurnOn( void )
{
	if( flashlight.GetInt() > 0 && IsAlive() )
	{
		AddEffects( EF_DIMLIGHT );
		EmitSound( "HL2Player.FlashlightOn" );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CMSS_Player::FlashlightTurnOff( void )
{
	RemoveEffects( EF_DIMLIGHT );
	
	if( IsAlive() )
	{
		EmitSound( "HL2Player.FlashlightOff" );
	}
}

void CMSS_Player::Weapon_Drop( CBaseCombatWeapon *pWeapon, const Vector *pvecTarget, const Vector *pVelocity )
{
// BOXBOX TODO why do melee weapons not drop?

/*
	if ( GetActiveWeapon() )
	{
		CBaseCombatWeapon *pGrenade = Weapon_OwnsThisType("weapon_frag");

		if ( GetActiveWeapon() == pGrenade )
		{
			if ( ( m_nButtons & IN_ATTACK ) || (m_nButtons & IN_ATTACK2) )
			{
				DropPrimedFragGrenade( this, pGrenade );
				return;
			}
		}
	}
*/
	BaseClass::Weapon_Drop( pWeapon, pvecTarget, pVelocity );

}


void CMSS_Player::DetonateTripmines( void )
{
	CBaseEntity *pEntity = NULL;

	while ((pEntity = gEntList.FindEntityByClassname( pEntity, "npc_satchel" )) != NULL)
	{
		CSatchelCharge *pSatchel = dynamic_cast<CSatchelCharge *>(pEntity);
		if (pSatchel->m_bIsLive && pSatchel->GetThrower() == this )
		{
			g_EventQueue.AddEvent( pSatchel, "Explode", 0.20, this, this );
		}
	}

	// Play sound for pressing the detonator
	EmitSound( "Weapon_SLAM.SatchelDetonate" );
}

void CMSS_Player::Event_Killed( const CTakeDamageInfo &info )
{
	//update damage info with our accumulated physics force
	CTakeDamageInfo subinfo = info;
	subinfo.SetDamageForce( m_vecTotalBulletForce );

	SetNumAnimOverlays( 0 );

	// Note: since we're dead, it won't draw us on the client, but we don't set EF_NODRAW
	// because we still want to transmit to the clients in our PVS.
	CreateRagdollEntity();

	DetonateTripmines();

	BaseClass::Event_Killed( subinfo );


	if ( info.GetDamageType() & DMG_DISSOLVE )
	{
		if ( m_hRagdoll )
		{
			m_hRagdoll->GetBaseAnimating()->Dissolve( NULL, gpGlobals->curtime, false, ENTITY_DISSOLVE_NORMAL );
		}
	}

	CBaseEntity *pAttacker = info.GetAttacker();

	if ( pAttacker )
	{
		int iScoreToAdd = 1;

		if ( pAttacker == this )
		{
			iScoreToAdd = -1;
		}

// BOXBOX removing teams
//		GetGlobalTeam( pAttacker->GetTeamNumber() )->AddScore( iScoreToAdd );
	}

	FlashlightTurnOff();

	m_lifeState = LIFE_DEAD;

	RemoveEffects( EF_NODRAW );	// still draw player body
	StopZooming();

}

int CMSS_Player::OnTakeDamage( const CTakeDamageInfo &inputInfo )
{
	//return here if the player is in the respawn grace period vs. slams.
//	if ( gpGlobals->curtime < m_flSlamProtectTime &&  (inputInfo.GetDamageType() == DMG_BLAST ) ) // BOXBOX don't need this
//		return 0;

	m_vecTotalBulletForce += inputInfo.GetDamageForce();
	
	gamestats->Event_PlayerDamage( this, inputInfo );

	return BaseClass::OnTakeDamage( inputInfo );
}

void CMSS_Player::DeathSound( const CTakeDamageInfo &info ) // BOXBOX redoing death sounds
{
	if ( m_hRagdoll && m_hRagdoll->GetBaseAnimating()->IsDissolving() )
		 return;

	char szDeathSound[128];
	const char *pModelName = STRING( GetModelName() );

	if ( pModelName )
		if ( !stricmp( pModelName, "models/player/humanmale.mdl" ) )
		{
			Q_snprintf( szDeathSound, sizeof( szDeathSound ), "Player_HumanMale.Die" );
		}
		else if ( !stricmp( pModelName, "models/player/humanfemale.mdl" ) )
		{
			Q_snprintf( szDeathSound, sizeof( szDeathSound ), "Player_HumanFemale.Die" );
		}

	CSoundParameters params;
	if ( GetParametersForSound( szDeathSound, params, pModelName ) == false )
		return;

	Vector vecOrigin = GetAbsOrigin();
	
	CRecipientFilter filter;
	filter.AddRecipientsByPAS( vecOrigin );

	EmitSound_t ep;
	ep.m_nChannel = params.channel;
	ep.m_pSoundName = params.soundname;
	ep.m_flVolume = params.volume;
	ep.m_SoundLevel = params.soundlevel;
	ep.m_nFlags = 0;
	ep.m_nPitch = params.pitch;
	ep.m_pOrigin = &vecOrigin;

	EmitSound( filter, entindex(), ep );
}

CBaseEntity* CMSS_Player::EntSelectSpawnPoint( void ) // BOXBOX removing teams
{
	CBaseEntity *pSpot = NULL;
	CBaseEntity *pLastSpawnPoint = g_pLastSpawn;
	edict_t		*player = edict();
	const char *pSpawnpointName = "mss_spawnpoint"; // BOXBOX was "info_player_deathmatch"
/*
	if ( MSSRules()->IsTeamplay() == true )
	{
		if ( GetTeamNumber() == TEAM_COMBINE )
		{
			pSpawnpointName = "info_player_combine";
			pLastSpawnPoint = g_pLastCombineSpawn;
		}
		else if ( GetTeamNumber() == TEAM_REBELS )
		{
			pSpawnpointName = "info_player_rebel";
			pLastSpawnPoint = g_pLastRebelSpawn;
		}

		if ( gEntList.FindEntityByClassname( NULL, pSpawnpointName ) == NULL )
		{
			pSpawnpointName = "info_player_deathmatch";
			pLastSpawnPoint = g_pLastSpawn;
		}
	}
*/
	pSpot = pLastSpawnPoint;
	// Randomize the start spot
	for ( int i = random->RandomInt(1,5); i > 0; i-- )
		pSpot = gEntList.FindEntityByClassname( pSpot, pSpawnpointName );
	if ( !pSpot )  // skip over the null point
		pSpot = gEntList.FindEntityByClassname( pSpot, pSpawnpointName );

	CBaseEntity *pFirstSpot = pSpot;

	do 
	{
		if ( pSpot )
		{
			// check if pSpot is valid
			if ( g_pGameRules->IsSpawnPointValid( pSpot, this ) )
			{
				if ( pSpot->GetLocalOrigin() == vec3_origin )
				{
					pSpot = gEntList.FindEntityByClassname( pSpot, pSpawnpointName );
					continue;
				}

				// if so, go to pSpot
				goto ReturnSpot;
			}
		}
		// increment pSpot
		pSpot = gEntList.FindEntityByClassname( pSpot, pSpawnpointName );
	} while ( pSpot != pFirstSpot ); // loop if we're not back to the start

	// we haven't found a place to spawn yet,  so kill any guy at the first spawn point and spawn there
	if ( pSpot )
	{
		CBaseEntity *ent = NULL;
		for ( CEntitySphereQuery sphere( pSpot->GetAbsOrigin(), 128 ); (ent = sphere.GetCurrentEntity()) != NULL; sphere.NextEntity() )
		{
			// if ent is a client, kill em (unless they are ourselves)
			if ( ent->IsPlayer() && !(ent->edict() == player) )
				ent->TakeDamage( CTakeDamageInfo( GetContainingEntity(INDEXENT(0)), GetContainingEntity(INDEXENT(0)), 300, DMG_GENERIC ) );
		}
		goto ReturnSpot;
	}

	if ( !pSpot  )
	{
		pSpot = gEntList.FindEntityByClassname( pSpot, "info_player_start" );

		if ( pSpot )
			goto ReturnSpot;
	}

ReturnSpot:
/*
	if ( MSSRules()->IsTeamplay() == true )
	{
		if ( GetTeamNumber() == TEAM_COMBINE )
		{
			g_pLastCombineSpawn = pSpot;
		}
		else if ( GetTeamNumber() == TEAM_REBELS ) 
		{
			g_pLastRebelSpawn = pSpot;
		}
	}
*/
	g_pLastSpawn = pSpot;

//	m_flSlamProtectTime = gpGlobals->curtime + 0.5; // BOXBOX commented out

	return pSpot;
} 


/* BOXBOX don't need this

CON_COMMAND( timeleft, "prints the time remaining in the match" )
{
	CMSS_Player *pPlayer = ToMSSPlayer( UTIL_GetCommandClient() );

	int iTimeRemaining = (int)MSSRules()->GetMapRemainingTime();
    
	if ( iTimeRemaining == 0 )
	{
		if ( pPlayer )
		{
			ClientPrint( pPlayer, HUD_PRINTTALK, "This game has no timelimit." );
		}
		else
		{
			Msg( "* No Time Limit *\n" );
		}
	}
	else
	{
		int iMinutes, iSeconds;
		iMinutes = iTimeRemaining / 60;
		iSeconds = iTimeRemaining % 60;

		char minutes[8];
		char seconds[8];

		Q_snprintf( minutes, sizeof(minutes), "%d", iMinutes );
		Q_snprintf( seconds, sizeof(seconds), "%2.2d", iSeconds );

		if ( pPlayer )
		{
			ClientPrint( pPlayer, HUD_PRINTTALK, "Time left in map: %s1:%s2", minutes, seconds );
		}
		else
		{
			Msg( "Time Remaining:  %s:%s\n", minutes, seconds );
		}
	}	
}
*/

void CMSS_Player::Reset()
{	
	ResetDeathCount();
	ResetFragCount();
}
/*
bool CMSS_Player::IsReady()
{
	return m_bReady;
}

void CMSS_Player::SetReady( bool bReady )
{
	m_bReady = bReady;
}
*/
void CMSS_Player::CheckChatText( char *p, int bufsize )
{
	//Look for escape sequences and replace

	char *buf = new char[bufsize];
	int pos = 0;

	// Parse say text for escape sequences
	for ( char *pSrc = p; pSrc != NULL && *pSrc != 0 && pos < bufsize-1; pSrc++ )
	{
		// copy each char across
		buf[pos] = *pSrc;
		pos++;
	}

	buf[pos] = '\0';

	// copy buf back into p
	Q_strncpy( p, buf, bufsize );

	delete[] buf;	

//	const char *pReadyCheck = p;

//	MSSRules()->CheckChatForReadySignal( this, pReadyCheck );
}

void CMSS_Player::State_Transition( MSSPlayerState newState )
{
	State_Leave();
	State_Enter( newState );
}


void CMSS_Player::State_Enter( MSSPlayerState newState )
{
	m_iPlayerState = newState;
	m_pCurStateInfo = State_LookupInfo( newState );

	// Initialize the new state.
	if ( m_pCurStateInfo && m_pCurStateInfo->pfnEnterState )
		(this->*m_pCurStateInfo->pfnEnterState)();
}


void CMSS_Player::State_Leave()
{
	if ( m_pCurStateInfo && m_pCurStateInfo->pfnLeaveState )
	{
		(this->*m_pCurStateInfo->pfnLeaveState)();
	}
}


void CMSS_Player::State_PreThink()
{
	if ( m_pCurStateInfo && m_pCurStateInfo->pfnPreThink )
	{
		(this->*m_pCurStateInfo->pfnPreThink)();
	}
}


CMSSPlayerStateInfo *CMSS_Player::State_LookupInfo( MSSPlayerState state )
{
	// This table MUST match the 
	static CMSSPlayerStateInfo playerStateInfos[] =
	{
		{ STATE_ACTIVE,			"STATE_ACTIVE",			&CMSS_Player::State_Enter_ACTIVE, NULL, &CMSS_Player::State_PreThink_ACTIVE },
		{ STATE_OBSERVER_MODE,	"STATE_OBSERVER_MODE",	&CMSS_Player::State_Enter_OBSERVER_MODE,	NULL, &CMSS_Player::State_PreThink_OBSERVER_MODE }
	};

	for ( int i=0; i < ARRAYSIZE( playerStateInfos ); i++ )
	{
		if ( playerStateInfos[i].m_iPlayerState == state )
			return &playerStateInfos[i];
	}

	return NULL;
}

bool CMSS_Player::StartObserverMode(int mode)
{
	//we only want to go into observer mode if the player asked to, not on a death timeout
	if ( m_bEnterObserver == true )
	{
		VPhysicsDestroyObject();
		return BaseClass::StartObserverMode( mode );
	}
	return false;
}

void CMSS_Player::StopObserverMode()
{
	m_bEnterObserver = false;
	BaseClass::StopObserverMode();
}

void CMSS_Player::State_Enter_OBSERVER_MODE()
{
	int observerMode = m_iObserverLastMode;
	if ( IsNetClient() )
	{
		const char *pIdealMode = engine->GetClientConVarValue( engine->IndexOfEdict( edict() ), "cl_spec_mode" );
		if ( pIdealMode )
		{
			observerMode = atoi( pIdealMode );
			if ( observerMode <= OBS_MODE_FIXED || observerMode > OBS_MODE_ROAMING )
			{
				observerMode = m_iObserverLastMode;
			}
		}
	}
	m_bEnterObserver = true;
	StartObserverMode( observerMode );
}

void CMSS_Player::State_PreThink_OBSERVER_MODE()
{
	// Make sure nobody has changed any of our state.
	//	Assert( GetMoveType() == MOVETYPE_FLY );
	Assert( m_takedamage == DAMAGE_NO );
	Assert( IsSolidFlagSet( FSOLID_NOT_SOLID ) );
	//	Assert( IsEffectActive( EF_NODRAW ) );

	// Must be dead.
	Assert( m_lifeState == LIFE_DEAD );
	Assert( pl.deadflag );
}


void CMSS_Player::State_Enter_ACTIVE()
{
	SetMoveType( MOVETYPE_WALK );
	
	// md 8/15/07 - They'll get set back to solid when they actually respawn. If we set them solid now and mp_forcerespawn
	// is false, then they'll be spectating but blocking live players from moving.
	// RemoveSolidFlags( FSOLID_NOT_SOLID );
	
	m_Local.m_iHideHUD = 0;
}


void CMSS_Player::State_PreThink_ACTIVE()
{
	//we don't really need to do anything here. 
	//This state_prethink structure came over from CS:S and was doing an assert check that fails the way hl2dm handles death
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CMSS_Player::CanHearAndReadChatFrom( CBasePlayer *pPlayer )
{
	// can always hear the console unless we're ignoring all chat
	if ( !pPlayer )
		return false;

	return true;
}



///////////////////////////////////////////////////////////////////
//
// BOXBOX New MSS functions at bottom of file for finding easier
//
///////////////////////////////////////////////////////////////////

void CMSS_Player::HandleCommand_Spectate( void ) // BOXBOX user has entered 'spectate' in console
{
	if ( !IsObserver() ) // BOXBOX If you're not already spectating, turn spectator mode on
	{
		if ( !mp_allowspectators.GetInt() ) // BOXBOX unless server doesn't allow spectators
		{
			ClientPrint( this, HUD_PRINTCENTER, "#Cannot_Be_Spectator" );
			return;
		}

		State_Transition( STATE_OBSERVER_MODE ); // BOXBOX ok to enter spectator mode
	}
	else // BOXBOX if you are already spectating, toggle spectator mode off
	{
		StopObserverMode();
		State_Transition(STATE_ACTIVE);
	}
}

void CMSS_Player::PreLoadChars( void )
{
	m_nNumChars = 0;

	charloadstatus_e status = CMSS_Player::LoadChar( 0 );
	if( status == CHARLOAD_STATUS_OK )
	{
		V_strncpy( m_szPreloadCharName0.GetForModify(), m_szCharName.GetForModify(), MAX_CHAR_NAME_LENGTH );
		m_PreloadedCharInfo_Model.Set( 0, (m_nRace * 2) + m_nGender + 1 );
		m_nNumChars++;
	}
	else
	{
		V_strncpy( m_szPreloadCharName0.GetForModify(), "", MAX_CHAR_NAME_LENGTH );
		m_PreloadedCharInfo_Model.Set( 0, MODEL_NOCHAR );
	}

	status = CMSS_Player::LoadChar( 1 );
	if( status == CHARLOAD_STATUS_OK )
	{
		V_strncpy( m_szPreloadCharName1.GetForModify(), m_szCharName.GetForModify(), MAX_CHAR_NAME_LENGTH );
		m_PreloadedCharInfo_Model.Set( 1, (m_nRace * 2) + m_nGender + 1 );
		m_nNumChars++;
	}
	else
	{
		V_strncpy( m_szPreloadCharName1.GetForModify(), "", MAX_CHAR_NAME_LENGTH );
		m_PreloadedCharInfo_Model.Set( 1, MODEL_NOCHAR );
	}

	status = CMSS_Player::LoadChar( 2 );
	if( status == CHARLOAD_STATUS_OK )
	{
		V_strncpy( m_szPreloadCharName2.GetForModify(), m_szCharName.GetForModify(), MAX_CHAR_NAME_LENGTH );
		m_PreloadedCharInfo_Model.Set( 2, (m_nRace * 2) + m_nGender + 1 );
		m_nNumChars++;
	}
	else
	{
		V_strncpy( m_szPreloadCharName2.GetForModify(), "", MAX_CHAR_NAME_LENGTH );
		m_PreloadedCharInfo_Model.Set( 2, MODEL_NOCHAR );
	}
}

/* BOXBOX obsoleted by incorporating into function above
void CMSS_Player::SetNumChars( void )
{
	m_nNumChars = 0;

	for( int i = 0; i < MAX_CHAR_SLOTS; i++ )
	{
		char filePath[MAX_PATH];
		CharacterSave::GetSaveFileNameForPlayer( this, i, filePath );
		if( g_pFullFileSystem->FileExists( filePath ) )
			m_nNumChars++;
	}
}
*/
void CMSS_Player::UpdateStats( void )
{
	//Strength formula
	ms_strength = 2 * ms_warriorSkills + ms_martialArts + .25 * ( ms_smallArms + ms_archery + ms_spellCasting );

	if ( (ms_archery > ms_martialArts && ms_martialArts > ms_warriorSkills && ms_martialArts > ms_smallArms && ms_martialArts > ms_spellCasting ) || ( ms_martialArts > ms_archery && ms_archery > ms_warriorSkills && ms_archery > ms_smallArms && ms_archery > ms_spellCasting ) )
		//Dexterity formula when the highest skills are Archery and Martial Arts
		ms_dexterity = .5 * ( ms_martialArts +  ms_archery );
	else
		//Dexterity formula if archery and martial arts arn't the highest skills, (drkill wants to redo this, comment for now)
		ms_dexterity = 2 * ms_smallArms + ms_martialArts + ms_archery + .25 * ( ms_smallArms + ms_spellCasting );

	//Concentration formula
	ms_concentration = 2 * ms_spellCasting + ms_archery + .25 * (ms_warriorSkills + ms_smallArms) + ms_martialArts;

	//Fitness formula
	ms_fitness = 1.25 * ms_warriorSkills + ms_martialArts + .75 * ms_smallArms + .5 * ms_archery + .25 * ms_spellCasting;

	//Awareness formula
	ms_awareness = .25 * ms_warriorSkills + .5 * ms_martialArts + .75 * ms_smallArms + ms_archery + 1.25 * ms_spellCasting;

	//HP formula
	ms_maxHealth = 5 * ms_fitness + 2 * ms_awareness + 29;

	//Mana formula
	ms_maxMana = 7 * ms_awareness + 29;

	//Determine the two highest skill numbers
	int skillArray[5] = { ms_warriorSkills, ms_martialArts, ms_smallArms, ms_archery, ms_spellCasting };
//	std::sort( skillArray, skillArray+5 );

	//Main Level formula
	ms_level = (skillArray[3] + skillArray[4]) / 2;

}