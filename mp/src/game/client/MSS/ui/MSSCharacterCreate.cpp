//========= Copyright © 2008, Mike Raineri, All rights reserved. ============//
//
// Purpose: Builds the character creation window
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include <stdio.h>
#include <string>

#include "MSSCharacterCreate.h"

using namespace vgui;
using namespace std;

#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/TextEntry.h>
#include <vgui_controls/RadioButton.h>
#include <vgui/ISurface.h>
#include <cdll_client_int.h>
#include "clientmode_MSSnormal.h"

extern const char *pszPlayerModels[];

Vector CMSCharCreateMenu::g_ViewPos;
QAngle CMSCharCreateMenu::g_ViewAng;
CMSCharCreateMenu *CMSCharCreateMenu::static_pCurrentMenu = NULL;

CMSCharCreateMenu::CMSCharCreateMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_CHARCREATE )
{
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/MSScheme.res", "MSScheme"); // BOXBOX added
	SetScheme(scheme);

	SetMoveable( false );
	SetSizeable( false );
	SetProportional( true );
	SetKeyBoardInputEnabled( true );
	SetTitleBarVisible( false );

//	m_pTitleLabel	= new Label( this, "TitleText", "Title Text" );

	m_nSelectedGender = MSS_GENDER_INVALID;
	m_nSelectedRace = RACE_INVALID;

	m_pMightLabel		= new Label( this, "MightLabel", "0" );
	m_pAgilityLabel		= new Label( this, "AgilityLabel", "0" );
	m_pStaminaLabel		= new Label( this, "StaminaLabel", "0" );
	m_pIntellectLabel	= new Label( this, "IntellectLabel", "0" );
	m_pWitLabel			= new Label( this, "WitLabel", "0" );
	m_pFortitudeLabel	= new Label( this, "FortitudeLabel", "0" );
	m_pLuckLabel		= new Label( this, "LuckLabel", "0" );
	m_pBonusLabel		= new Label( this, "BonusLabel", "0" );


	m_nMightValue = m_nBaseMightValue = 3;
	m_nAgilityValue = m_nBaseAgilityValue = 3;
	m_nStaminaValue = m_nBaseStaminaValue = 3;
	m_nIntellectValue = m_nBaseIntellectValue = 3;
	m_nWitValue = m_nBaseWitValue = 3;
	m_nFortitudeValue = m_nBaseFortitudeValue = 3;
	m_nLuckValue = m_nBaseLuckValue = 3;

	m_nPointsToSpread = 3;

	m_pCharModel = new CModelPanel( this, "charmodel" );
}

CMSCharCreateMenu::~CMSCharCreateMenu()
{
}

void CMSCharCreateMenu::ApplySchemeSettings( IScheme *pScheme ) // BOXBOX added this
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSCharacterCreate.res");

//	m_pTitleLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
//	m_pTitleLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );

}

void CMSCharCreateMenu::Reset( )
{
	m_nSelectedGender = MSS_GENDER_INVALID;
	m_nSelectedRace = RACE_INVALID;

	Button *pButton = (Button *)FindChildByName( "ButtonMale" );
	pButton->SetEnabled( true );
	pButton = (Button *)FindChildByName( "ButtonFemale" );
	pButton->SetEnabled( true );
	pButton = (Button *)FindChildByName( "ButtonHuman" );
	pButton->SetEnabled( true );
	pButton = (Button *)FindChildByName( "ButtonDwarf" );
	pButton->SetEnabled( true );
	pButton = (Button *)FindChildByName( "ButtonElf" );
	pButton->SetEnabled( true );

	TextEntry *pText = (TextEntry *)FindChildByName( "ChooseNameTextBox" );
	pText->SetText( "" );
}

void CMSCharCreateMenu::ShowPanel(bool bShow)
{
	// If already open, do nothing
	if ( BaseClass::IsVisible() == bShow )
		return;

	// Open the window
	if ( bShow )
	{
		Reset();
		Update();
		Activate();
		SetMouseInputEnabled(true);
	}

	// Closes the window
	else
	{
		Close( );
		//BaseClass::SetVisible( false );
		//SetMouseInputEnabled( false );
		//SetKeyBoardInputEnabled( false );
	}
}

bool CMSCharCreateMenu::NeedsUpdate( void )
{
	return false;
}

void CMSCharCreateMenu::Update( void )
{
	char buf[8];
	itoa( m_nMightValue, buf, 10 );
	m_pMightLabel->SetText( buf );

	itoa( m_nAgilityValue, buf, 10 );
	m_pAgilityLabel->SetText( buf );

	itoa( m_nStaminaValue, buf, 10 );
	m_pStaminaLabel->SetText( buf );

	itoa( m_nIntellectValue, buf, 10 );
	m_pIntellectLabel->SetText( buf );

	itoa( m_nWitValue, buf, 10 );
	m_pWitLabel->SetText( buf );

	itoa( m_nFortitudeValue, buf, 10 );
	m_pFortitudeLabel->SetText( buf );

	itoa( m_nLuckValue, buf, 10 );
	m_pLuckLabel->SetText( buf );

	itoa( m_nPointsToSpread, buf, 10 );
	m_pBonusLabel->SetText( buf );
}

void CMSCharCreateMenu::OnClose( )
{
	BaseClass::OnClose( );
}

void CMSCharCreateMenu::OnCommand( const char *command )
{
	if( Q_strstr( command, "gendermale" ) )
	{
		Button *pButton = (Button *)FindChildByName( "ButtonMale" );
		pButton->SetEnabled( false );
		pButton = (Button *)FindChildByName( "ButtonFemale" );
		pButton->SetEnabled( true );
		m_nSelectedGender = MSS_GENDER_MALE;
		ResetModel();
	}
	else if( Q_strstr( command, "genderfemale" ) )
	{
		Button *pButton = (Button *)FindChildByName( "ButtonMale" );
		pButton->SetEnabled( true );
		pButton = (Button *)FindChildByName( "ButtonFemale" );
		pButton->SetEnabled( false );
		m_nSelectedGender = MSS_GENDER_FEMALE;
		ResetModel();
	}
	else if( Q_strstr( command, "racehuman" ) )
	{
		Button *pButton = (Button *)FindChildByName( "ButtonHuman" );
		pButton->SetEnabled( false );
		pButton = (Button *)FindChildByName( "ButtonDwarf" );
		pButton->SetEnabled( true );
		pButton = (Button *)FindChildByName( "ButtonElf" );
		pButton->SetEnabled( true );
		m_nSelectedRace = RACE_HUMAN;
		ResetStats();
	}
	else if( Q_strstr( command, "racedwarf" ) )
	{
		Button *pButton = (Button *)FindChildByName( "ButtonDwarf" );
		pButton->SetEnabled( false );
		pButton = (Button *)FindChildByName( "ButtonHuman" );
		pButton->SetEnabled( true );
		pButton = (Button *)FindChildByName( "ButtonElf" );
		pButton->SetEnabled( true );
		m_nSelectedRace = RACE_DWARF;
		ResetStats();
	}
	else if( Q_strstr( command, "raceelf" ) )
	{
		Button *pButton = (Button *)FindChildByName( "ButtonElf" );
		pButton->SetEnabled( false );
		pButton = (Button *)FindChildByName( "ButtonDwarf" );
		pButton->SetEnabled( true );
		pButton = (Button *)FindChildByName( "ButtonHuman" );
		pButton->SetEnabled( true );
		m_nSelectedRace = RACE_ELF;
		ResetStats();
	}
	else if( Q_strstr( command, "minusmight" ) )
	{
		if( m_nMightValue < m_nBaseMightValue ) // BOXBOX You may only lower a stat one point under your racial base
			return;
		m_nMightValue--;
		m_nPointsToSpread++;
		Update();
	}
	else if( Q_strstr( command, "minusagility" ) )
	{
		if( m_nAgilityValue < m_nBaseAgilityValue )
			return;
		m_nAgilityValue--;
		m_nPointsToSpread++;
		Update();
	}
	else if( Q_strstr( command, "minusstamina" ) )
	{
		if( m_nStaminaValue < m_nBaseStaminaValue )
			return;
		m_nStaminaValue--;
		m_nPointsToSpread++;
		Update();
	}
	else if( Q_strstr( command, "minusintellect" ) )
	{
		if( m_nIntellectValue < m_nBaseIntellectValue )
			return;
		m_nIntellectValue--;
		m_nPointsToSpread++;
		Update();
	}
	else if( Q_strstr( command, "minuswit" ) )
	{
		if( m_nWitValue < m_nBaseWitValue )
			return;
		m_nWitValue--;
		m_nPointsToSpread++;
		Update();
	}
	else if( Q_strstr( command, "minusfortitude" ) )
	{
		if( m_nFortitudeValue < m_nBaseFortitudeValue )
			return;
		m_nFortitudeValue--;
		m_nPointsToSpread++;
		Update();
	}
	else if( Q_strstr( command, "minusluck" ) )
	{
		if( m_nLuckValue < m_nBaseLuckValue )
			return;
		m_nLuckValue--;
		m_nPointsToSpread++;
		Update();
	}
	else if( Q_strstr( command, "plusmight" ) )
	{
		if( !m_nPointsToSpread )
			return;
		m_nMightValue++;
		m_nPointsToSpread--;
		Update();
	}
	else if( Q_strstr( command, "plusagility" ) )
	{
		if( !m_nPointsToSpread )
			return;
		m_nAgilityValue++;
		m_nPointsToSpread--;
		Update();
	}
	else if( Q_strstr( command, "plusstamina" ) )
	{
		if( !m_nPointsToSpread )
			return;
		m_nStaminaValue++;
		m_nPointsToSpread--;
		Update();
	}
	else if( Q_strstr( command, "plusintellect" ) )
	{
		if( !m_nPointsToSpread )
			return;
		m_nIntellectValue++;
		m_nPointsToSpread--;
		Update();
	}
	else if( Q_strstr( command, "pluswit" ) )
	{
		if( !m_nPointsToSpread )
			return;
		m_nWitValue++;
		m_nPointsToSpread--;
		Update();
	}
	else if( Q_strstr( command, "plusfortitude" ) )
	{
		if( !m_nPointsToSpread )
			return;
		m_nFortitudeValue++;
		m_nPointsToSpread--;
		Update();
	}
	else if( Q_strstr( command, "plusluck" ) )
	{
		if( !m_nPointsToSpread )
			return;
		m_nLuckValue++;
		m_nPointsToSpread--;
		Update();
	}
	else if( Q_strstr( command, "cancel" ) )
	{
		gViewPortInterface->ShowPanel( this, false );
		gViewPortInterface->ShowPanel( PANEL_CHARSELECT, true );
		vgui::surface()->PlaySound( "UI/pageturn.wav" );
	}
	else if( Q_strstr( command, "createnew" ) )
	{
		bool dataIsValid = true;
		char charName[64] = "";

		TextEntry *pCharName = (TextEntry *)FindChildByName( "ChooseNameTextBox" );
		pCharName->GetText( charName, 64 );

		if( !charName[0] )
			dataIsValid = false;

		if( m_nSelectedGender == MSS_GENDER_INVALID)
			dataIsValid = false;

		if( m_nSelectedRace == RACE_INVALID )
			dataIsValid = false;

		if( m_nPointsToSpread )
			dataIsValid = false;

		if( dataIsValid )
		{
			C_MSS_Player *pPlayer = ToMSSPlayer( C_BasePlayer::GetLocalPlayer() );
			if( !pPlayer ) return;

			string sendCommand = VarArgs( "createchar %i \"%s\" %i %i %i %i %i %i %i %i %i", pPlayer->m_nCharSlotToCreate, charName, m_nSelectedGender, m_nSelectedRace, m_nMightValue, m_nAgilityValue, m_nStaminaValue, m_nIntellectValue, m_nWitValue, m_nFortitudeValue, m_nLuckValue );
			engine->ClientCmd( sendCommand.c_str() );
			gViewPortInterface->ShowPanel( this, false );
			vgui::surface()->PlaySound( "UI/pageturn.wav" );
		}
	}
	else
		BaseClass::OnCommand( command );
}

vgui::Panel *CMSCharCreateMenu::CreateControlByName(const char *controlName)
{
    /*if ( Q_stricmp( controlName, "ClassImagePanel" ) == 0 )
    {
        return new CClassImagePanel( NULL, controlName );
    }*/

    return BaseClass::CreateControlByName( controlName );
}

void CMSCharCreateMenu::ResetModel( void )
{
	if( ( m_nSelectedGender > MSS_GENDER_INVALID ) && ( m_nSelectedRace > RACE_INVALID ) )
	{
		m_pCharModel->SwapModel( pszPlayerModels[ (m_nSelectedRace * 2) + m_nSelectedGender -2 ], NULL ); 
		m_pCharModel->MoveToFront();
	}
}

void CMSCharCreateMenu::ResetStats( void )
{
	if( m_nSelectedRace == RACE_HUMAN )
	{
		m_nMightValue = m_nBaseMightValue = 3;
		m_nAgilityValue = m_nBaseAgilityValue = 3;
		m_nStaminaValue = m_nBaseStaminaValue = 3;
		m_nIntellectValue = m_nBaseIntellectValue = 3;
		m_nWitValue = m_nBaseWitValue = 3;
		m_nFortitudeValue = m_nBaseFortitudeValue = 3;
		m_nLuckValue = m_nBaseLuckValue = 3;
	}
	else if( m_nSelectedRace == RACE_DWARF )
	{
		m_nMightValue = m_nBaseMightValue = 4;
		m_nAgilityValue = m_nBaseAgilityValue = 2;
		m_nStaminaValue = m_nBaseStaminaValue = 4;
		m_nIntellectValue = m_nBaseIntellectValue = 2;
		m_nWitValue = m_nBaseWitValue = 2;
		m_nFortitudeValue = m_nBaseFortitudeValue = 4;
		m_nLuckValue = m_nBaseLuckValue = 3;
	}
	else if( m_nSelectedRace == RACE_ELF )
	{
		m_nMightValue = m_nBaseMightValue = 2;
		m_nAgilityValue = m_nBaseAgilityValue = 4;
		m_nStaminaValue = m_nBaseStaminaValue = 2;
		m_nIntellectValue = m_nBaseIntellectValue = 4;
		m_nWitValue = m_nBaseWitValue = 4;
		m_nFortitudeValue = m_nBaseFortitudeValue = 2;
		m_nLuckValue = m_nBaseLuckValue = 3;
	}
	m_nPointsToSpread = 3;
	ResetModel();
	Update();
}