
// BOXBOX MSS item parsing -  formerly weapon_pasre.h from the SDK

#ifndef MSS_ITEM_PARSE_H
#define MSS_ITEM_PARSE_H
#ifdef _WIN32
#pragma once
#endif

#include "shareddefs.h"
#include "MSS_shareddefs.h"

class IFileSystem;

typedef unsigned short ITEM_FILE_INFO_HANDLE;

// Weapon sound types
// This needs to match pWeaponSoundCategories in MSS_item_parse.cpp

typedef enum {
//	EMPTY,
//	SINGLE,
//	SINGLE_NPC,
//	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
//	DOUBLE_NPC,
//	BURST,
//	RELOAD,
//	RELOAD_NPC,
	DEPLOY,				// Pulling weapon out
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
//	SPECIAL1,
//	SPECIAL2,
//	SPECIAL3,
//	TAUNT,

	NUM_WPN_SOUND_TYPES,
} WeaponSound_t;

int GetWeaponSoundFromString( const char *pszString );

#define MAX_WPN_SOUNDS	4			// Maximum number of weapon sounds per weapon type

#define MAX_ITEM_STRING	80
#define MAX_ITEM_PREFIX	16
#define MAX_DESCRIPTION_STRING	250
//#define MAX_WEAPON_AMMO_NAME		32

#define ITEM_PRINTNAME_MISSING "Missing printname on item!"

class CHudTexture;
class KeyValues;


//-----------------------------------------------------------------------------
// Purpose: Contains the data read from the item's script file. 
// It's cached so we only read each item's script file once.
// Each game provides a CreateItemInfo function so it can have game-specific
// data in the item script.
//-----------------------------------------------------------------------------
class FileItemInfo_t
{
public:

	FileItemInfo_t();
	
	void Parse( KeyValues *pKeyValuesData, const char *szItemName );
	
	bool			bParsedScript;
//	bool			bLoadedHudElements;

// ALL ITEMS
	char			szClassName[MAX_ITEM_STRING];		// use classname to create items (LinkEntityToClass)
	char			szPrintName[MAX_ITEM_STRING];		//	Name for showing in GUI
	char			szDescription[MAX_DESCRIPTION_STRING]; // The text that describes the item (like when you hold mouse over icon for a second, etc.)
	char			szIconFileName[MAX_ITEM_STRING];	// The path/filename for the vgui icon
	char			szWorldModel[MAX_ITEM_STRING];		//	Model of this item. In the case of weapons/armor, it's the 'world model' seen attached to the player
	int				nSkin;								//	If model has multiple skins, specify here

	int				nItemType;							//	What type of item is this? ( see MSSItemTypes_t )
	int				nItemLevel;							//	This will have different effects depending on Item Type.  For weapons, determines base damage
// WEAPONS
	char			szViewModel[MAX_ITEM_STRING];		// View model of this weapon
	char			szAnimationPrefix[MAX_ITEM_PREFIX];	// Prefix of the animations that should be used by the player carrying this weapon
//	bool			m_bBuiltRightHanded;				// This helps cl_righthand make the decision about whether to flip the model or not.
//	bool			m_bAllowFlipping;					// False to disallow flipping the model

//	int				iSlot;									// inventory slot.
//	int				iPosition;								// position in the inventory slot.
//	int				iMaxClip1;								// max primary clip size (-1 if no clip)
//	int				iMaxClip2;								// max secondary clip size (-1 if no clip)
//	int				iDefaultClip1;							// amount of primary ammo in the gun when it's created
//	int				iDefaultClip2;							// amount of secondary ammo in the gun when it's created
//	int				iWeight;								// this value used to determine this weapon's importance in autoselection.
//	int				iRumbleEffect;							// Which rumble effect to use when fired? (xbox)
//	bool			bAutoSwitchTo;							// whether this weapon should be considered for autoswitching to
//	bool			bAutoSwitchFrom;						// whether this weapon can be autoswitched away from when picking up another weapon or ammo
//	int				iFlags;									// miscellaneous weapon flags
//	char			szAmmo1[MAX_WEAPON_AMMO_NAME];			// "primary" ammo type
//	char			szAmmo2[MAX_WEAPON_AMMO_NAME];			// "secondary" ammo type

	// Sound blocks
	char					aWpnSounds[NUM_WPN_SOUND_TYPES][MAX_ITEM_STRING];	

//	int						iAmmoType;
//	int						iAmmo2Type;
//	bool					m_bMeleeWeapon;		// Melee weapons can always "fire" regardless of ammo.

												

// CLIENT DLL
	// Sprite data, read from the data file
//	int						iSpriteCount;
//	CHudTexture						*iconActive;
//	CHudTexture	 					*iconInactive;
//	CHudTexture 					*iconAmmo;
//	CHudTexture 					*iconAmmo2;
//	CHudTexture 					*iconCrosshair;
//	CHudTexture 					*iconAutoaim;
//	CHudTexture 					*iconZoomedCrosshair;
//	CHudTexture 					*iconZoomedAutoaim;
//	CHudTexture						*iconSmall;

//	bool					bShowUsageHint;							// if true, then when you receive the weapon, show a hint about it

// SERVER DLL

};



// The item parse function
bool ReadItemDataFromFile( IFileSystem* filesystem, const char *szWeaponName, 
	ITEM_FILE_INFO_HANDLE *phandle, const unsigned char *pICEKey = NULL );

// If item info has been loaded for the specified class name, this returns it.
ITEM_FILE_INFO_HANDLE LookupItemInfo( const char *name );

FileItemInfo_t *GetFileItemInfoFromHandle( ITEM_FILE_INFO_HANDLE handle );
ITEM_FILE_INFO_HANDLE GetInvalidItemInfoHandle( void );
void PrecacheFileItemInfoDatabase( IFileSystem *filesystem, const unsigned char *pICEKey );


// 
// Read a possibly-encrypted KeyValues file in. 
// If pICEKey is NULL, then it appends .txt to the filename and loads it as an unencrypted file.
// If pICEKey is non-NULL, then it appends .ctx to the filename and loads it as an encrypted file.
//
// (This should be moved into a more appropriate place).
//
KeyValues* ReadEncryptedKVFile( IFileSystem *filesystem, const char *szFilenameWithoutExtension, const unsigned char *pICEKey, bool bForceReadEncryptedFile = false );


// Each game implements this. It can return a derived class and override Parse() if it wants.
extern FileItemInfo_t* CreateItemInfo();


#endif // MSS_ITEM_PARSE_H
