
#ifndef WEAPON_MSSBASE_H
#define WEAPON_MSSBASE_H
#ifdef _WIN32
#pragma once
#endif

#include "MSS_player_shared.h"
#include "basecombatweapon_shared.h"
#include "MSS_item_parse.h"

#if defined( CLIENT_DLL )
	#define CWeaponMSSBase C_WeaponMSSBase
	void UTIL_ClipPunchAngleOffset( QAngle &in, const QAngle &punch, const QAngle &clip );
#endif

class CMSS_Player;

// bool IsAmmoType( int iAmmoType, const char *pAmmoName ); // BOXBOX removing ammo

class CWeaponMSSBase : public CBaseCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponMSSBase, CBaseCombatWeapon );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CWeaponMSSBase();

	#ifdef GAME_DLL
		DECLARE_DATADESC();
	
//		void SendReloadSoundEvent( void );

//		void Materialize( void );
		virtual	int	ObjectCaps( void );
	#endif

	// All predicted weapons need to implement and return true
	virtual bool	IsPredicted() const;

	CBasePlayer* GetPlayerOwner() const;
	CMSS_Player* GetMSSPlayerOwner() const;

	void WeaponSound( WeaponSound_t sound_type, float soundtime = 0.0f );
	
//	CHL2MPSWeaponInfo const	&GetHL2MPWpnData() const;
//	FileItemInfo_t const &GetWpnData() const;

	virtual void FireBullets( const FireBulletsInfo_t &info );
	virtual void FallInit( void );
	
public:
	#if defined( CLIENT_DLL )
		
		virtual bool	ShouldPredict();
		virtual void	OnDataChanged( DataUpdateType_t type );

		virtual bool	OnFireEvent( C_BaseViewModel *pViewModel, const Vector& origin, const QAngle& angles, int event, const char *options );

	#else

		virtual void	Spawn();

	#endif

	float		m_flPrevAnimTime;
	float  m_flNextResetCheckTime;

	Vector	GetOriginalSpawnOrigin( void ) { return m_vOriginalSpawnOrigin;	}
	QAngle	GetOriginalSpawnAngles( void ) { return m_vOriginalSpawnAngles;	}

private:

	CWeaponMSSBase( const CWeaponMSSBase & );

	Vector m_vOriginalSpawnOrigin;
	QAngle m_vOriginalSpawnAngles;
};


#endif // WEAPON_MSSBASE_H
