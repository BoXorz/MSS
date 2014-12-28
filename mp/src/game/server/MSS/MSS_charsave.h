//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: For saving characters onto text files.
//
//=============================================================================//

#ifndef CHARACTERSAVE_H
#define CHARACTERSAVE_H

#include "MSS_shareddefs.h"

#ifdef _WIN32
#pragma once
#endif

typedef void ( *VoidParameters ) ( void *pvMember, void *pvData );

enum ms_save_prop_e
{
	MSSAVEPROP_INT,
	MSSAVEPROP_FLOAT,
	MSSAVEPROP_STRING,
	MSSAVEPROP_BOOL,
};

class MSSavePropertyBase
{
public:
	const char *m_Name;
	void *m_NetworkVar;
	VoidParameters m_NetworkVarSetFunction;
	ms_save_prop_e m_NetworkVarType;

	MSSavePropertyBase( const char *name, void *networkVar, VoidParameters networkVarSetFunction, ms_save_prop_e networkVarType )
	{
		m_Name = name;
		m_NetworkVar = networkVar;
		m_NetworkVarSetFunction = networkVarSetFunction;
		m_NetworkVarType = networkVarType;
	}
};

typedef char hexkey[41];

class MSSaveProperty : public MSSavePropertyBase
{
public:
	variant_t m_Value;
	static int offsetTable[];
	char m_EncryptedValue[256];

	MSSaveProperty( const char *name, const char *value, void *networkGetForModify ) : MSSavePropertyBase( name, networkGetForModify, NULL, MSSAVEPROP_STRING )
	{
		m_Value.SetString( AllocPooledString( value ) );
	}

	MSSaveProperty( const char *name, int value, void *networkVar, VoidParameters networkVarSetFunction ) : MSSavePropertyBase( name, networkVar, networkVarSetFunction, MSSAVEPROP_INT )
	{
		m_Value.SetInt( value );
	}

	MSSaveProperty( const char *name, unsigned int value, void *networkVar, VoidParameters networkVarSetFunction ) : MSSavePropertyBase( name, networkVar, networkVarSetFunction, MSSAVEPROP_INT )
	{
		m_Value.SetInt( value );
	}

	MSSaveProperty( const char *name, float value, void *networkVar, VoidParameters networkVarSetFunction ) : MSSavePropertyBase( name, networkVar, networkVarSetFunction, MSSAVEPROP_FLOAT )
	{
		m_Value.SetFloat( value );
	}

	MSSaveProperty( const char *name, bool value, void *networkVar, VoidParameters networkVarSetFunction ) : MSSavePropertyBase( name, networkVar, networkVarSetFunction, MSSAVEPROP_BOOL )
	{
		m_Value.SetBool( value );
	}

	void AddToKey( KeyValues *keyValue, int encryptionKey );
	
	void SetValueFromEncryptedString( const char *encryptedString, int encryptionKey );
};

enum charloadstatus_e
{
	CHARLOAD_STATUS_OK,
	CHARLOAD_STATUS_FILE_NOT_FOUND,
	CHARLOAD_STATUS_FILE_CORRUPT
};

class CharacterSave
{
public:

	static void GetSaveFileNameForPlayer( class CMSS_Player *pPlayer, int slot, char filePath[MAX_PATH] );
	static void GetCheckSum( KeyValues *values, char out_CheckSum[] );
	static const char *m_pszSavePath;

//	static void Think( );
//	static float m_flLastSaveTime; // BOXBOX not using this, saving characters now in CMSS_Player::PostThink
};

//#define MAX_CHAR_SLOTS 3

#endif
