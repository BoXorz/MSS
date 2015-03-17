
// BOXBOX Weakest sword in MSS! Bane of rats everywhere!

#include "cbase.h"
#include "npcevent.h"
//#include "weapon_MSSbasecombatweapon.h"
#include "weapon_MSSbasebasebludgeon.h"

/*
#include "gamerules.h"
#include "ammodef.h"
#include "mathlib/mathlib.h"
#include "in_buttons.h"
#include "vstdlib/random.h"

#if defined( CLIENT_DLL )
	#include "c_MSS_player.h"
#else
	#include "MSS_player.h"
	#include "ai_basenpc.h"
#endif
*/

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef CLIENT_DLL
#define CWeaponRustyShortSword C_WeaponRustyShortSword
#endif

class CWeaponRustyShortSword : public CBaseCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponRustyShortSword, CBaseCombatWeapon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif

	CWeaponRustyShortSword();

	float		GetRange( void );
//	float		GetFireRate( void );

//	void		AddViewKick( void ); // BOXBOX nope
//	float		GetDamageForActivity( Activity hitActivity );
//	void		SecondaryAttack( void )	{ return; }

	void		Drop( const Vector &vecVelocity );


	// Animation event
#ifndef CLIENT_DLL
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	void HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
//	int WeaponMeleeAttack1Condition( float flDot, float flDist );
#endif

	CWeaponRustyShortSword( const CWeaponRustyShortSword & );

private:
		
};

//#define RUSTYSHORTSWORD_DAMAGE	5.0f
#define	RUSTYSHORTSWORD_RANGE	60.0f
//#define	RUSTYSHORTSWORD_REFIRE	0.6f

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponRustyShortSword, DT_WeaponRustyShortSword )

BEGIN_NETWORK_TABLE( CWeaponRustyShortSword, DT_WeaponRustyShortSword )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponRustyShortSword )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_rustyshortsword, CWeaponRustyShortSword );
PRECACHE_WEAPON_REGISTER( weapon_rustyshortsword );

#ifndef CLIENT_DLL

acttable_t	CWeaponRustyShortSword::m_acttable[] = 
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

IMPLEMENT_ACTTABLE(CWeaponRustyShortSword);

#endif

CWeaponRustyShortSword::CWeaponRustyShortSword( void )
{
//	m_nMSSWeaponType = WEAPONTYPE_ONEHANDSLASHING;
}

/* BOXBOX removing
void CWeaponCrowbar::AddViewKick( void )
{
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	QAngle punchAng;

	punchAng.x = SharedRandomFloat( "crowbarpax", 1.0f, 2.0f );
	punchAng.y = SharedRandomFloat( "crowbarpay", -2.0f, -1.0f );
	punchAng.z = 0.0f;
	
	pPlayer->ViewPunch( punchAng ); 
}
*/

#ifndef CLIENT_DLL
//-----------------------------------------------------------------------------
// Animation event handlers
//-----------------------------------------------------------------------------
void CWeaponRustyShortSword::HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	// Trace up or down based on where the enemy is...
	// But only if we're basically facing that direction
	Vector vecDirection;
	AngleVectors( GetAbsAngles(), &vecDirection );

	Vector vecEnd;
	VectorMA( pOperator->Weapon_ShootPosition(), 50, vecDirection, vecEnd );
	CBaseEntity *pHurt = pOperator->CheckTraceHullAttack( pOperator->Weapon_ShootPosition(), vecEnd, 
		Vector(-16,-16,-16), Vector(36,36,36), GetDamageForActivity( GetActivity() ), DMG_CLUB, 0.75 );
	
	// did I hit someone?
	if ( pHurt )
	{
		// play sound
		WeaponSound( MELEE_HIT );

		// Fake a trace impact, so the effects work out like a player's crowbaw
		trace_t traceHit;
		UTIL_TraceLine( pOperator->Weapon_ShootPosition(), pHurt->GetAbsOrigin(), MASK_SHOT_HULL, pOperator, COLLISION_GROUP_NONE, &traceHit );
		ImpactEffect( traceHit );
	}
	else
	{
		WeaponSound( MELEE_MISS );
	}
}


//-----------------------------------------------------------------------------
// Animation event
//-----------------------------------------------------------------------------
void CWeaponRustyShortSword::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
	case EVENT_WEAPON_MELEE_HIT:
		HandleAnimEventMeleeHit( pEvent, pOperator );
		break;

	default:
		BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
		break;
	}
}

//ConVar sk_crowbar_lead_time( "sk_crowbar_lead_time", "0.9" ); // BOXBOX not right now

/* BOXBOX changing things
int CWeaponRustyShortSword::WeaponMeleeAttack1Condition( float flDot, float flDist )
{
	CAI_BaseNPC *pNPC	= GetOwner()->MyNPCPointer();
	CBaseEntity *pEnemy = pNPC->GetEnemy();
	if (!pEnemy)
		return COND_NONE;

	Vector vecVelocity;
	vecVelocity = pEnemy->GetSmoothedVelocity( );

	// Project where the enemy will be in a little while
	float dt = sk_crowbar_lead_time.GetFloat();
	dt += SharedRandomFloat( "crowbarmelee1", -0.3f, 0.2f );
	if ( dt < 0.0f )
		dt = 0.0f;

	Vector vecExtrapolatedPos;
	VectorMA( pEnemy->WorldSpaceCenter(), dt, vecVelocity, vecExtrapolatedPos );

	Vector vecDelta;
	VectorSubtract( vecExtrapolatedPos, pNPC->WorldSpaceCenter(), vecDelta );

	if ( fabs( vecDelta.z ) > 70 )
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	Vector vecForward = pNPC->BodyDirection2D( );
	vecDelta.z = 0.0f;
	float flExtrapolatedDist = Vector2DNormalize( vecDelta.AsVector2D() );
	if ((flDist > 64) && (flExtrapolatedDist > 64))
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	float flExtrapolatedDot = DotProduct2D( vecDelta.AsVector2D(), vecForward.AsVector2D() );
	if ((flDot < 0.7) && (flExtrapolatedDot < 0.7))
	{
		return COND_NOT_FACING_ATTACK;
	}

	return COND_CAN_MELEE_ATTACK1;
}
*/

#endif

void CWeaponRustyShortSword::Drop( const Vector &vecVelocity )
{
#ifndef CLIENT_DLL
//	UTIL_Remove( this ); // BOXBOX This is why weapons wouldn't drop!

	BaseClass::Drop( vecVelocity ); // BOXBOX replaced with this

	AddEffects( EF_ITEM_BLINK ); // BOXBOX TODO adding this, but maybe later change it to highlight using code from estranged?
#endif
}
/*
float CWeaponRustyShortSword::GetDamageForActivity( Activity hitActivity )
{
	return RUSTYSHORTSWORD_DAMAGE;
}
*/
float CWeaponRustyShortSword::GetRange( void )
{
	return	RUSTYSHORTSWORD_RANGE;	
}
/*
float CWeaponRustyShortSword::GetFireRate( void )
{
	return	RUSTYSHORTSWORD_REFIRE;	
}
*/

