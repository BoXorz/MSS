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

	m_pTitleLabel	= new Label( this, "TitleText", "Title Text" );

	m_nSelectedGender = -1;
	m_nSelectedRace = -1;
}

CMSCharCreateMenu::~CMSCharCreateMenu()
{
}

void CMSCharCreateMenu::ApplySchemeSettings( IScheme *pScheme ) // BOXBOX added this
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSCharacterCreate.res");

	m_pTitleLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
	m_pTitleLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );

}

void CMSCharCreateMenu::Reset( )
{
	m_nSelectedGender = -1;
	m_nSelectedRace = -1;

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
		m_nSelectedGender = GENDER_MALE;
	}
	else if( Q_strstr( command, "genderfemale" ) )
	{
		Button *pButton = (Button *)FindChildByName( "ButtonMale" );
		pButton->SetEnabled( true );
		pButton = (Button *)FindChildByName( "ButtonFemale" );
		pButton->SetEnabled( false );
		m_nSelectedGender = GENDER_FEMALE;
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

		if( m_nSelectedGender == -1)
			dataIsValid = false;

		if( m_nSelectedRace == -1 )
			dataIsValid = false;

		if( dataIsValid )
		{
			string sendCommand = VarArgs( "createchar \"%s\" %i %i", charName, m_nSelectedGender, m_nSelectedRace );
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

