
// Purpose: For saving characters onto text files.

#include "cbase.h"
#include "MSS_charsave.h"
#include "filesystem.h"
#include "steam/steam_api.h"
#include "MSS_player.h"
#include "variant_t.h"
#include "checksum_md5.h"


//float CharacterSave::m_flLastSaveTime = 0.0f;
const char *CharacterSave::m_pszSavePath = "Characters";

//#define ENCRYPTION_ENABLED
	
//All data that should be saved
//=============================
void CMSS_Player::GetCharFileFields( CUtlVector<MSSaveProperty> &allPlayerData )
{
	//allPlayerData.AddToTail( MSSaveProperty( "Version", 1.0f, NULL, NULL ) );
	allPlayerData.AddToTail( MSSaveProperty( "Character Name",	m_szCharName,			m_szCharName.GetForModify( ) ) );
	allPlayerData.AddToTail( MSSaveProperty( "Gender",			m_nGender.Get(),		&m_nGender, m_nGender.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Race",			m_nRace.Get(),			&m_nRace, m_nRace.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "TotalExp",		m_nTotalExp.Get(),		&m_nTotalExp, m_nTotalExp.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Gold",			m_nGold.Get(),			&m_nGold, m_nGold.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Might",			m_nMight.Get(),			&m_nMight, m_nMight.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Agility",			m_nAgility.Get(),		&m_nAgility, m_nAgility.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Stamina",			m_nStamina.Get(),		&m_nStamina, m_nStamina.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Intellect",		m_nIntellect.Get(),		&m_nIntellect, m_nIntellect.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Wit",				m_nWit.Get(),			&m_nWit, m_nWit.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Fortitude",		m_nFortitude.Get(),		&m_nFortitude, m_nFortitude.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Luck",			m_nLuck.Get(),			&m_nLuck, m_nLuck.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Unarmed",				m_nUnarmed.Get(),			&m_nUnarmed, m_nUnarmed.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "OneHandPiercing",		m_nOneHandPiercing.Get(),	&m_nOneHandPiercing, m_nOneHandPiercing.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "OneHandSlashing",		m_nOneHandSlashing.Get(),	&m_nOneHandSlashing, m_nOneHandSlashing.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "OneHandBashing",		m_nOneHandBashing.Get(),	&m_nOneHandBashing, m_nOneHandBashing.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "TwoHandPiercing",		m_nTwoHandPiercing.Get(),	&m_nTwoHandPiercing, m_nTwoHandPiercing.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "TwoHandSlashing",		m_nTwoHandSlashing.Get(),	&m_nTwoHandSlashing, m_nTwoHandSlashing.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "TwoHandBashing",		m_nTwoHandBashing.Get(),	&m_nTwoHandBashing, m_nTwoHandBashing.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Archery",				m_nArchery.Get(),			&m_nArchery, m_nArchery.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "ThrowingWeapons",		m_nThrowingWeapons.Get(),	&m_nThrowingWeapons, m_nThrowingWeapons.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Alchemy",				m_nAlchemy.Get(),			&m_nAlchemy, m_nAlchemy.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Clothwork",			m_nClothwork.Get(),			&m_nClothwork, m_nClothwork.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Woodwork",			m_nWoodwork.Get(),			&m_nWoodwork, m_nWoodwork.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Stonework",			m_nStonework.Get(),			&m_nStonework, m_nStonework.Static_Set ) );
	allPlayerData.AddToTail( MSSaveProperty( "Metalwork",			m_nMetalwork.Get(),			&m_nMetalwork, m_nMetalwork.Static_Set ) );



//	allPlayerData.AddToTail( MSSaveProperty( "PlayerKills",		ms_playerKills.Get( ), &ms_playerKills, ms_playerKills.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Gold",			ms_gold.Get( ),		&ms_gold, ms_gold.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "MaxHealth",		ms_maxHealth.Get( ), &ms_maxHealth, ms_maxHealth.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Warrior Skills",	ms_warriorSkills.Get( ), &ms_warriorSkills, ms_warriorSkills.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Martial Arts",	ms_martialArts.Get( ), &ms_martialArts, ms_martialArts.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Archery",			ms_archery.Get( ),	&ms_archery, ms_archery.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Spell Casting",	ms_spellCasting.Get( ), &ms_spellCasting, ms_spellCasting.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Parry",			ms_parry.Get( ),	&ms_parry, ms_parry.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Warrior Skills Exp", ms_warriorSkillsExpPercent.Get( ), &ms_warriorSkillsExpPercent, ms_warriorSkillsExpPercent.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Martial Arts Exp", ms_martialArtsExpPercent.Get( ), &ms_martialArtsExpPercent, ms_martialArtsExpPercent.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Small Arms Exp",	ms_smallArmsExpPercent.Get( ), &ms_smallArmsExpPercent, ms_smallArmsExpPercent.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Archery Exp",		ms_archeryExpPercent.Get( ), &ms_archeryExpPercent, ms_archeryExpPercent.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Spell Casting Exp", ms_spellCastingExpPercent.Get( ), &ms_spellCastingExpPercent, ms_spellCastingExpPercent.Static_Set ) );
//	allPlayerData.AddToTail( MSSaveProperty( "Parry Exp",		ms_parryExpPercent.Get( ), &ms_parryExpPercent, ms_parryExpPercent.Static_Set ) );

}


void CMSS_Player::SaveChar( int slot )
{
	if( !IsReadyToPlay( ) )
		return;

	//Get the fields to save
	CUtlVector<MSSaveProperty> allPlayerData;
	GetCharFileFields( allPlayerData );

	//Now save the data
	char filePath[MAX_PATH];
	CharacterSave::GetSaveFileNameForPlayer( this, slot, filePath );

	Msg("SaveChar() called with filePath = ");
	Msg( filePath );
	Msg("\n");

	m_flLastSaveTime = gpGlobals->curtime;

	KeyValues *values = new KeyValues( "PlayerData" );
	
	int originalKey = -1;

#ifdef ENCRYPTION_ENABLED
	int encryptionKey = originalKey = random->RandomInt( 0, 1024 );
	encryptionKey = originalKey;
	encryptionKey += 87;
	encryptionKey *= 4027;

	char encryptionString[256];
	Q_snprintf( encryptionString, sizeof(encryptionString), "%.2X", encryptionKey );

	int len = Q_strlen( encryptionString );

	int offset = 2;
	for( int i = 0; i < len; i++ )
	{
		encryptionString[i] += offset;
		offset *= -(offset - 1);
	}

	char temp[32];
	char encryptionString2[256] = "";
	for( int i = 0; i < len; i++ )
	{
		Q_snprintf( temp, sizeof(temp), "%.2X", encryptionString[i] );
		Q_strcat( encryptionString2, temp, sizeof(encryptionString2) );
	}

	if( (Q_strlen(encryptionString2) % 2) )
	{
		//error
	}

	values->SetString( "EncryptionString", encryptionString2 );
#endif

	for( int i = 0; i < allPlayerData.Count(); i++ )
		allPlayerData[i].AddToKey( values, originalKey );

	char checkSum[256];
	CharacterSave::GetCheckSum( values, checkSum );

	values->SetString( "EncryptionString2", checkSum );
	filesystem->CreateDirHierarchy( CharacterSave::m_pszSavePath, "MOD" );

	values->SaveToFile( filesystem, filePath, "MOD" );

	values->deleteThis();
}

charloadstatus_e CMSS_Player::LoadChar( int slot )
{
	//Get the fields to load
	CUtlVector<MSSaveProperty> allPlayerData;
	GetCharFileFields( allPlayerData );

	char filePath[MAX_PATH];
	CharacterSave::GetSaveFileNameForPlayer( this, slot, filePath );
	Msg("LoadChar called with Filepath = ");
	Msg(filePath);
	Msg("\n");

	KeyValues *values = new KeyValues( "PlayerData" );

	if( !values->LoadFromFile( filesystem, filePath, "MOD" ) ) // BOXBOX No character file!
	{
		m_bHasCharInSlot.Set( slot, false );
		return CHARLOAD_STATUS_FILE_NOT_FOUND;
	}

	const char *encryptString = values->GetString( "EncryptionString" );

	int encryptionKey = -1;
	if( encryptString && strlen(encryptString) )
	{
		Msg("It's being passed.\n");
		char checkSum[256];
		CharacterSave::GetCheckSum( values, checkSum );

		const char *encryptString2 = values->GetString( "EncryptionString2" );
		if( !encryptString2 )
			return CHARLOAD_STATUS_FILE_CORRUPT;

		if( Q_strcmp( checkSum, encryptString2 ) )
			return CHARLOAD_STATUS_FILE_CORRUPT;

		char temp[32];
		char encryptionString1[256];

		int hexChars = Q_strlen(encryptString) / 2;
		for( int i = 0; i < hexChars; i++ )
		{
			Q_strncpy( temp, &encryptString[i * 2], 3 );
			
			int tempInt = 0;
			sscanf_s( temp, "%X", &tempInt );
			encryptionString1[i] = tempInt;
		}

		int offset = 2;
		for( int i = 0; i < hexChars; i++ )
		{
			encryptionString1[i] -= offset;
			offset *= -(offset - 1);
		}
		encryptionString1[hexChars] = 0;

		sscanf( encryptionString1, "%X", &encryptionKey );

		encryptionKey /= 4027;
		encryptionKey -= 87;
	}

	for( int i = 0; i < allPlayerData.Count(); i++ )
	{
		allPlayerData[i].SetValueFromEncryptedString( values->GetString( allPlayerData[i].m_Name ), encryptionKey );
	}


	m_bHasCharInSlot.Set( slot, true );
	values->deleteThis();

//	UpdateStats( );

	return CHARLOAD_STATUS_OK;
}

void CMSS_Player::PreLoadChar( int slot )
{
//	m_nNumChars = 0;

//	CMSS_Player::LoadChar( slot );
	LoadChar( slot );

	if( m_bHasCharInSlot[ slot ] )
	{
		if( slot == CHARSLOT_ONE )
		{
			V_strncpy( m_szPreloadCharName1.GetForModify(), m_szCharName.GetForModify(), MAX_CHAR_NAME_LENGTH );
		}
		else if( slot == CHARSLOT_TWO )
		{
			V_strncpy( m_szPreloadCharName2.GetForModify(), m_szCharName.GetForModify(), MAX_CHAR_NAME_LENGTH );
		}
		else if( slot == CHARSLOT_THREE )
		{
			V_strncpy( m_szPreloadCharName3.GetForModify(), m_szCharName.GetForModify(), MAX_CHAR_NAME_LENGTH );
		}

		m_nPreloadModelIndex.Set( slot, (m_nRace * 2) + m_nGender - 2 );
	}
	else
	{
		if( slot == CHARSLOT_ONE )
		{
			V_strncpy( m_szPreloadCharName1.GetForModify(), "", MAX_CHAR_NAME_LENGTH );
		}
		else if( slot == CHARSLOT_TWO )
		{
			V_strncpy( m_szPreloadCharName2.GetForModify(), "", MAX_CHAR_NAME_LENGTH );
		}
		else if( slot == CHARSLOT_THREE )
		{
			V_strncpy( m_szPreloadCharName3.GetForModify(), "", MAX_CHAR_NAME_LENGTH );
		}

		m_nPreloadModelIndex.Set( slot, MODEL_INVALID );
	}
}


/*
void CharacterSave::Think( )
{
	if( ( m_flLastSaveTime ) && ( gpGlobals->curtime - m_flLastSaveTime > 10.0f ) )
	{
		for ( int i = 1; i <= gpGlobals->maxClients; i++ )
		{
			CMSS_Player *pPlayer = (CMSS_Player *)UTIL_PlayerByIndex( i );

			if ( pPlayer && pPlayer->IsReadyToPlay( ) )
				pPlayer->SaveChar();
		}

		m_flLastSaveTime = gpGlobals->curtime;
	}
}
*/

void CharacterSave::GetSaveFileNameForPlayer( CMSS_Player *pPlayer, int slot, char filePath[MAX_PATH] )
{
	char convertedSteamID[MAX_PATH];

	Q_strncpy( convertedSteamID, pPlayer->GetNetworkIDString( ), MAX_PATH );

	while( strchr( convertedSteamID, ':' ) )
		strchr( convertedSteamID, ':' )[0] = '_';

	Q_strncpy( filePath, UTIL_VarArgs("%s/%s_%i.char", CharacterSave::m_pszSavePath, convertedSteamID, slot ), MAX_PATH );
}

int MSSaveProperty::offsetTable[] = { 
	127, 20, 56, 201, 3, 96, 43, 67, 9, 135, 78, 92, 4, 95, 31, 247, 1, 89, 70, 0, 125, 39, 114, 152, 62, 99, 
	17, 243, 18, 149, 12, 64, 51 };

void GetHexKey( hexkey &hexKey1, const char *pszValueName, int encryptionKey )
{
	int expandedKey = encryptionKey;

	for( int i = 0; i < Q_strlen(pszValueName); i++ )
		encryptionKey += pszValueName[i] * 13;

	Q_memset( hexKey1, 191, sizeof(hexkey) );
	int totalLen = 0;

	while( totalLen < 29 + (encryptionKey % 12) )
	{
		expandedKey += totalLen + 92 * totalLen;

		if( !(expandedKey % 11) )
			expandedKey *= 43;
		else if( !(expandedKey % 6) )
			expandedKey -= 34;
		else if( !(expandedKey % 2) )
			expandedKey *= 13;

		hexKey1[totalLen++] += expandedKey;
	}

	int r = expandedKey;
	int rotateAmt = ARRAYSIZE(hexKey1) * (encryptionKey % 2 ? 2 : 3);
	for( int i = 0; i < rotateAmt; i++ )
	{
		int rotPos = i % ARRAYSIZE(hexKey1);
		r = r % ARRAYSIZE( MSSaveProperty::offsetTable );

		hexKey1[rotPos] += MSSaveProperty::offsetTable[r] + (encryptionKey % 256) + i * 2;
		r++;
	}
}
void MSSaveProperty::AddToKey( KeyValues *keyValue, int encryptionKey )
{
	const char *pszStoredValue = m_Value.String( );

	if( encryptionKey != -1 )
	{
		m_EncryptedValue[0] = 0;

		hexkey hexKey1;
		GetHexKey( hexKey1, m_Name, encryptionKey );

		char msgLen[256];
		int len = Q_strlen(pszStoredValue);
		Q_snprintf( msgLen, sizeof(msgLen), "%.4i", len );

		int encryptedLen = max( len+4, ARRAYSIZE(hexKey1) );
		
		unsigned char values[256];
		int r = 0;
		int n = 0;
		for( int i = 0; i < encryptedLen; i++ )
		{
			unsigned char &targetValue = values[i];
			if( r >= ARRAYSIZE(hexKey1) )
				r = 0;

			if( i < 4 )
			{
				targetValue = hexKey1[r] + msgLen[i] * ((i % 2) ? -1 : 1);
			}
			else if( i < 4 + (len * 2) )
			{
				if( i % 2 )
					targetValue = hexKey1[r] + pszStoredValue[n++] * ((i % 3) ? -1 : 1);
				else
					targetValue = hexKey1[r] + 13 * ((i % 3) ? -1 : 1);
			}
			else
				targetValue = hexKey1[r] + ((i % 2) ? -1 : 1);

			char temp[32];
			Q_snprintf( temp, sizeof(temp), "%.2X", targetValue );
			Q_strcat( m_EncryptedValue, temp, sizeof(m_EncryptedValue) );
			r++;
		}
		
		pszStoredValue = m_EncryptedValue;
	}

	keyValue->SetString( m_Name, pszStoredValue );
}


void MSSaveProperty::SetValueFromEncryptedString( const char *encryptedString, int encryptionKey )
{
	if( !m_NetworkVar )
		return;

	char unencryptedValue[256];

	if( encryptionKey == -1 )
		Q_strncpy( unencryptedValue, encryptedString, 256 );
	else
	{
		hexkey hexKey1;
		GetHexKey( hexKey1, m_Name, encryptionKey );

		byte sizeString[32] = "";
		char mixedString[256] = "";

		int nums = Q_strlen(encryptedString) / 2;
		int sizeofOriginal = 0;
		int r = 0;
		int n = 0;
		for( int i = 0; i < nums; i++ )
		{
			if( r >= ARRAYSIZE(hexKey1) )
				r = 0;

			int num = 0;
			char oneNum[32] = "";
			Q_strncpy( oneNum, &encryptedString[i*2], 3 );
			sscanf_s( oneNum, "%X", &num );
			if( i < 4 )
			{
				sizeString[i] = (byte)((num - hexKey1[r]) * ((i % 2) ? -1 : 1));
				if( i == 3 )
				{
					sizeString[4] = 0;
					sizeofOriginal = Q_atoi( (char *)sizeString );
				}
			}
			else if( (i < 4 + sizeofOriginal * 2) && (i % 2) )
			{
				mixedString[n++] = (num - hexKey1[r]) * ((i % 3) ? -1 : 1);
				mixedString[n] = 0;
			}

			r++;
		}	

		Q_strncpy( unencryptedValue, mixedString, sizeof(unencryptedValue ) );
	}

	switch( m_NetworkVarType )
	{
		case MSSAVEPROP_INT:
			{
				int testInt = atoi( unencryptedValue );
				m_NetworkVarSetFunction( m_NetworkVar, &testInt );
				break;
			}
		case MSSAVEPROP_FLOAT:
			{
				float testFloat = atof( unencryptedValue );
				m_NetworkVarSetFunction( m_NetworkVar, &testFloat );
				break;
			}
		case MSSAVEPROP_BOOL:
			{
				bool testBool = !strcmp( unencryptedValue, "true" ) ?  true : false;
				m_NetworkVarSetFunction( m_NetworkVar, &testBool );
				break;
			}
		case MSSAVEPROP_STRING:
			Q_strcpy( (char *)m_NetworkVar, unencryptedValue );
			break;
	}
}

void CharacterSave::GetCheckSum( KeyValues *values, char out_CheckSum[256] )
{
	MD5Context_t context;
	MD5Init( &context );
	
	KeyValues *currentValue = values->GetFirstValue();
	while( currentValue )
	{
		if( !Q_strcmp( "EncryptionString2", currentValue->GetName( ) ) )
		{
			currentValue = currentValue->GetNextValue( );
			continue;
		}

		const char *strValue = (const char *)values->GetString( currentValue->GetName(), "" );
		MD5Update( &context, (const unsigned char *)strValue, strlen(strValue) );

		currentValue = currentValue->GetNextValue( );
	}

	unsigned char digest[MD5_DIGEST_LENGTH];
	MD5Final( digest, &context );

	Q_strncpy( out_CheckSum, MD5_Print( digest, MD5_DIGEST_LENGTH ), 256 );

	Q_strupr( out_CheckSum );
	int len = Q_strlen( out_CheckSum );

	for( int i = 3; i < len; i++ )
	{
		if( !(i % 2) )
		{
			unsigned char temp = out_CheckSum[i];
			out_CheckSum[i] = out_CheckSum[i-3];
			out_CheckSum[i-3] = temp + i + 9;
		}
		else
			out_CheckSum[i] += i * 113;
	}

	for( int i = 0; i < len; i++ )
	{
		out_CheckSum[i] = ((unsigned)out_CheckSum[i]) %16;
		if( out_CheckSum[i] < 10 )
			out_CheckSum[i] = '0' + out_CheckSum[i];
		else
			out_CheckSum[i] = 'A' + (out_CheckSum[i]-10);
	}

	out_CheckSum[len] = 0;
}