
/* BOXBOX well here goes nothing, rebuilding the entire item/weapon code base!

Here's the current(old) class structure, using arozen sword as an example

	CBaseAnimating  - the top of the pyramid in both old and new code structure
		CBaseCombatWeapon
			CWeaponMSSBase
				CBaseMSSCombatWeapon		
					CBaseMSSBludgeonWeapon
						CWeaponArozenSword

damn, that's crazy, ok now here's the structure we want in the restructuring

	CBaseAnimating
		CMSSBaseItem  - this base class will hold the ItemFileInfo_t for all items/weapons, and everything common to all item types.
			CMSSBaseWeapon  - all melee weapons
				CMSSArcheryWeapon

*/

#pragma once

#include "cbase.h"

#if defined( CLIENT_DLL )
#define CMSSBaseItem C_MSSBaseItem
#endif


class CMSSBaseItem : public CBaseAnimating // BOXBOXBOX separating this from weapon stuff
{
public:
	DECLARE_CLASS( CMSSBaseItem, CBaseAnimating );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

/* BOXBOX weapon only code
#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif
*/

	CMSSBaseItem();
	~CMSSBaseItem();

	string_t				m_iszName;	// Classname of this item

	virtual CMSSBaseItem	*MyItemPointer( void ) { return this; }

	virtual bool			IsPredicted( void ) const { return false; } // BOXBOX made this false(only weapons need it right?)

	virtual void			Spawn( void );
	virtual void			Precache( void );

	// BOXBOX TODO set up the +use dynamic for picking up items!
	virtual void			SetPickupTouch( void );
	virtual void 			DefaultTouch( CBaseEntity *pOther );
	virtual void			GiveTo( CBaseEntity *pOther );

	CBaseCombatCharacter	*GetOwner() const;
	CBasePlayer				*GetPlayerOwner() const;
	CMSS_Player				*GetMSSPlayerOwner() const;

	void					SetOwner( CBaseCombatCharacter *owner );
	virtual void			OnPickedUp( CBaseCombatCharacter *pNewOwner );

	virtual void			Drop( const Vector &vecVelocity );

// BOXBOX item script file stuff

	ITEM_FILE_INFO_HANDLE	GetItemFileInfoHandle() { return m_hItemFileInfo; }
	ITEM_FILE_INFO_HANDLE	m_hItemFileInfo;

	const FileItemInfo_t	&GetItemData( void ) const;
	virtual char const		*GetName( void ) const;
	virtual char const		*GetPrintName( void ) const;
	virtual const char		*GetWorldModel( void ) const;
	virtual int				GetItemType( void ) const;
	virtual int				GetItemSubType( void ) const;
	virtual int				GetItemLevel( void ) const;

	virtual bool			CanBePickedUpByNPCs( void ) { return true;	}

	virtual int				GetSkinOverride() const { return -1; }

	virtual const unsigned char *GetEncryptionKey( void );

	virtual void			Activate( void );

#if !defined( CLIENT_DLL )	// Server Only

	DECLARE_DATADESC();

	virtual void			FallInit( void );						// prepare to fall to the ground
	virtual void			FallThink( void );						// make the item fall to the ground after spawning

	bool					IsConstrained() { return m_pConstraint != NULL; }
	bool					IsInBadPosition ( void );				// Is item in bad position to pickup?
	bool					RepositionItem ( void );				// Attempts to reposition the item where it can be gotted
	virtual void			Materialize( void );					// make an item visible and tangible

	void					DestroyItem( void );
	virtual void			Kill( void );

	virtual int				CapabilitiesGet( void ) { return 0; }
	virtual	int				ObjectCaps( void );

	void					Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

protected:
	COutputEvent			m_OnPlayerUse;		// Fired when the player uses the weapon.
	COutputEvent			m_OnPlayerPickup;	// Fired when the player picks up the weapon.
	COutputEvent			m_OnNPCPickup;		// Fired when an NPC picks up the weapon.
	COutputEvent			m_OnCacheInteraction;	// For awarding lambda cache achievements in HL2 on 360. See .FGD file for details 
public:


#else // Client only

	virtual bool			OnFireEvent( C_BaseViewModel *pViewModel, const Vector& origin, const QAngle& angles, int event, const char *options ) 
	{
		return false; 
	}

	virtual ShadowType_t	ShadowCastType();
	virtual void			SetDormant( bool bDormant );
	virtual void			OnDataChanged( DataUpdateType_t updateType );
	virtual void			OnRestore();

	virtual void			RestartParticleEffect( void ) {}

	virtual int				DrawModel( int flags );
	virtual bool			ShouldDraw( void );
	virtual bool			ShouldDrawPickup( void );
	virtual void			HandleInput( void ) { return; };
	virtual void			OverrideMouseInput( float *x, float *y ) { return; };
	virtual int				KeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding ) { return 1; }
	virtual bool			AddLookShift( void ) { return true; };

	virtual void			NotifyShouldTransmit( ShouldTransmitState_t state );

	virtual int				GetWorldModelIndex( void );

	virtual void			GetToolRecordingState( KeyValues *msg );

#endif // End client-only methods

	IPhysicsConstraint		*GetConstraint() { return m_pConstraint; }

private:

	IPhysicsConstraint		*m_pConstraint;

};