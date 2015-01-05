
// BOXBOX The Character Selection Menu

using namespace vgui;
using namespace std;

#include "cbase.h"
#include <stdio.h>
#include <string>
#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui/ISurface.h>
#include <cdll_client_int.h>
#include "usermessages.h"

#include "clientmode_MSSnormal.h"
#include "MSS_shareddefs.h"
#include "MSSCharacterSelect.h"
#include "c_MSS_player.h"

extern const char *pszPlayerModels[];

CMSCharSelectMenu::CMSCharSelectMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_CHARSELECT )
{
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/MSScheme.res", "MSScheme");
	SetScheme(scheme);

	SetMoveable( false );
	SetSizeable( false );
	SetProportional( true );
	SetTitleBarVisible( false );

//	m_pTitleLabel	= new Label( this, "TitleText", "Title Text" );
	m_pSlot1Label	= new Label( this, "CharText1", "" );
	m_pSlot2Label	= new Label( this, "CharText2", "" );
	m_pSlot3Label	= new Label( this, "CharText3", "" );

	m_pConfirmBgImage		= new vgui::ImagePanel( this, "ConfirmBGImg" );
	m_pConfirmLabel			= new vgui::Label( this, "ConfirmLabel", "#MSS_DEL_CONFIRM" );
	m_pConfirmYesButton		= new vgui::Button( this, "YesButton", "#MSS_YES", this, "confirmyes" );
	m_pConfirmNoButton		= new vgui::Button( this, "NoButton", "#MSS_NO", this, "confirmno" );

//	m_pFullCharBgImage		= new vgui::ImagePanel( this, "FullCharBGImg" );
//	m_pFullCharLabel		= new vgui::Label( this, "FullCharLabel", "#MSS_FULL_CHAR" );
//	m_pFullCharButton		= new vgui::Button( this, "FullCharButton", "#MSS_OK", this, "fullcharok" );

	m_pCharOne = new CModelPanel( this, "charone" );
	m_pCharTwo = new CModelPanel( this, "chartwo" );
	m_pCharThree = new CModelPanel( this, "charthree" );

	m_bJustDeleted[ CHARSLOT_ONE ] = false;
	m_bJustDeleted[ CHARSLOT_TWO ] = false;
	m_bJustDeleted[ CHARSLOT_THREE ] = false;
}

CMSCharSelectMenu::~CMSCharSelectMenu()
{
}

void CMSCharSelectMenu::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSCharacterSelect.res");

//	m_pTitleLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
//	m_pTitleLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );

	m_pSlot1Label->SetFont( pScheme->GetFont( "WritingFont" ) );
	m_pSlot1Label->SetFgColor( pScheme->GetColor( "RedInk", Color(0, 0, 0, 0) ) );
	m_pSlot2Label->SetFont( pScheme->GetFont( "WritingFont" ) );
	m_pSlot2Label->SetFgColor( pScheme->GetColor( "RedInk", Color(0, 0, 0, 0) ) );
	m_pSlot3Label->SetFont( pScheme->GetFont( "WritingFont" ) );
	m_pSlot3Label->SetFgColor( pScheme->GetColor( "RedInk", Color(0, 0, 0, 0) ) );

	m_pConfirmLabel->SetFont( pScheme->GetFont( "HeaderFontSmall" ) );
	m_pConfirmLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );

//	m_pFullCharLabel->SetFont( pScheme->GetFont( "HeaderFontSmall" ) );
//	m_pFullCharLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
}

void CMSCharSelectMenu::ShowPanel(bool bShow)
{
	// If already open, do nothing
	if ( BaseClass::IsVisible() == bShow )
		return;

// BOXBOX if any main menu panels are open, don't open
	IViewPortPanel *panel = gViewPortInterface->FindPanelByName( PANEL_STATSMENU );
	if( panel->IsVisible() )
		return;
	panel = gViewPortInterface->FindPanelByName( PANEL_INVENTORYMENU );
	if( panel->IsVisible() )
		return;
	panel = gViewPortInterface->FindPanelByName( PANEL_POWERUPMENU );
	if( panel->IsVisible() )
		return;

	// Open the window
	if ( bShow )
	{
		vgui::surface()->PlaySound( "UI/scrollopen.wav" );
		Reset();
		Update();
		Activate();
		SetMouseInputEnabled(true);
	}

	// Close the window
	else
		Close( );
}


void CMSCharSelectMenu::Reset( ) // BOXBOX redid entire function
{
	C_MSS_Player *pPlayer = ToMSSPlayer( C_BasePlayer::GetLocalPlayer() );
	if( !pPlayer ) return;

	if( pPlayer->m_bHasCharInSlot[ CHARSLOT_ONE ] )
	{
		m_pCharOne->SwapModel( pszPlayerModels[ pPlayer->m_nPreloadModelIndex[ CHARSLOT_ONE ] ], NULL ); 
		m_pCharOne->MoveToFront();
	}

	if( pPlayer->m_bHasCharInSlot[ CHARSLOT_TWO ] )
	{
		m_pCharTwo->SwapModel( pszPlayerModels[ pPlayer->m_nPreloadModelIndex[ CHARSLOT_TWO ] ], NULL ); 
		m_pCharTwo->MoveToFront();
	}

	if( pPlayer->m_bHasCharInSlot[ CHARSLOT_THREE ] )
	{
		m_pCharThree->SwapModel( pszPlayerModels[ pPlayer->m_nPreloadModelIndex[ CHARSLOT_THREE ] ], NULL ); 
		m_pCharThree->MoveToFront();
	}

	m_pCharOne->SetVisible( pPlayer->m_bHasCharInSlot[ CHARSLOT_ONE ] );
	if( m_bJustDeleted[ CHARSLOT_ONE ] )
	m_pCharOne->SetVisible( false );

	m_pCharTwo->SetVisible( pPlayer->m_bHasCharInSlot[ CHARSLOT_TWO ] );
	if( m_bJustDeleted[ CHARSLOT_TWO ] )
	m_pCharTwo->SetVisible( false );

	m_pCharThree->SetVisible( pPlayer->m_bHasCharInSlot[ CHARSLOT_THREE ] );
	if( m_bJustDeleted[ CHARSLOT_THREE ] )
	m_pCharThree->SetVisible( false );

	m_pSlot1Label->SetText( pPlayer->m_szPreloadCharName1 );
	if( m_bJustDeleted[ CHARSLOT_ONE ] )
		m_pSlot1Label->SetText( "" );
	m_pSlot2Label->SetText( pPlayer->m_szPreloadCharName2 );
	if( m_bJustDeleted[ CHARSLOT_TWO ] )
		m_pSlot2Label->SetText( "" );
	m_pSlot3Label->SetText( pPlayer->m_szPreloadCharName3 );
	if( m_bJustDeleted[ CHARSLOT_THREE ] )
		m_pSlot3Label->SetText( "" );

	Button *pButton = (Button *)FindChildByName( "CharButton1" );
	pButton->SetText( pPlayer->m_bHasCharInSlot[ CHARSLOT_ONE ] ? "" : "#MSS_NOCHAR" );
//	pButton->SetEnabled( pPlayer->m_bHasCharInSlot[ CHARSLOT_ONE ] ? true : false );
	if( m_bJustDeleted[ CHARSLOT_ONE ] )
	{
		pButton->SetText( "#MSS_NOCHAR" );
//		pButton->SetEnabled( false );
	}
	pButton = (Button *)FindChildByName( "CharButton2" );
	pButton->SetText( pPlayer->m_bHasCharInSlot[ CHARSLOT_TWO ] ? "" : "#MSS_NOCHAR" );
//	pButton->SetEnabled( pPlayer->m_bHasCharInSlot[ CHARSLOT_TWO ] ? true : false );
	if( m_bJustDeleted[ CHARSLOT_TWO ] )
	{
		pButton->SetText( "#MSS_NOCHAR" );
//		pButton->SetEnabled( false );
	}
	pButton = (Button *)FindChildByName( "CharButton3" );
	pButton->SetText( pPlayer->m_bHasCharInSlot[ CHARSLOT_THREE ] ? "" : "#MSS_NOCHAR" );
//	pButton->SetEnabled( pPlayer->m_bHasCharInSlot[ CHARSLOT_THREE ] ? true : false );
	if( m_bJustDeleted[ CHARSLOT_THREE ] )
	{
		pButton->SetText( "#MSS_NOCHAR" );
//		pButton->SetEnabled( false );
	}
}

void CMSCharSelectMenu::Update( void ) // BOXBOX don't need anything running constantly
{
}

void CMSCharSelectMenu::OnCommand( const char *command )
{
	C_MSS_Player *pPlayer = ToMSSPlayer( C_BasePlayer::GetLocalPlayer() );
	if( !pPlayer ) return;

	if( Q_strstr( command, "choosechar 1" ) )
	{
		if( pPlayer->m_bHasCharInSlot[ CHARSLOT_ONE ] )
		{
			engine->ClientCmd( command );
			gViewPortInterface->ShowPanel( this, false );
			vgui::surface()->PlaySound( "UI/pageturn.wav" );
			return;
		}
		else
		{
			pPlayer->m_nCharSlotToCreate = CHARSLOT_ONE; // BOXBOX tell char creation menu which slot to put new char in
			gViewPortInterface->ShowPanel( this, false );
			gViewPortInterface->ShowPanel( PANEL_CHARCREATE, true );
			vgui::surface()->PlaySound( "UI/pageturn.wav" );
			return;
		}
	}
	else if( Q_strstr( command, "choosechar 2" ) )
	{
		if( pPlayer->m_bHasCharInSlot[ CHARSLOT_TWO ] )
		{
			engine->ClientCmd( command );
			gViewPortInterface->ShowPanel( this, false );
			vgui::surface()->PlaySound( "UI/pageturn.wav" );
			return;
		}
		else
		{
			pPlayer->m_nCharSlotToCreate = CHARSLOT_TWO;
			gViewPortInterface->ShowPanel( this, false );
			gViewPortInterface->ShowPanel( PANEL_CHARCREATE, true );
			vgui::surface()->PlaySound( "UI/pageturn.wav" );
			return;
		}
	}
	else if( Q_strstr( command, "choosechar 3" ) )
	{
		if( pPlayer->m_bHasCharInSlot[ CHARSLOT_THREE ] )
		{
			engine->ClientCmd( command );
			gViewPortInterface->ShowPanel( this, false );
			vgui::surface()->PlaySound( "UI/pageturn.wav" );
			return;
		}
		else
		{
			pPlayer->m_nCharSlotToCreate = CHARSLOT_THREE;
			gViewPortInterface->ShowPanel( this, false );
			gViewPortInterface->ShowPanel( PANEL_CHARCREATE, true );
			vgui::surface()->PlaySound( "UI/pageturn.wav" );
			return;
		}
	}
	else if( Q_strstr( command, "deletechar 1" ) )
	{
		if( !pPlayer->m_bHasCharInSlot[ CHARSLOT_ONE ] )// BOXBOX no character to delete
		{
			return;
		}
		m_nCharToDelete = 1;
		ShowConfirm();
		return;
	}
	else if( Q_strstr( command, "deletechar 2" ) )
	{
		if( !pPlayer->m_bHasCharInSlot[ CHARSLOT_TWO ] )
		{
			return;
		}
		m_nCharToDelete = 2;
		ShowConfirm();
		return;
	}
	else if( Q_strstr( command, "deletechar 3" ) )
	{
		if( !pPlayer->m_bHasCharInSlot[ CHARSLOT_THREE ] )
		{
			return;
		}
		m_nCharToDelete = 3;
		ShowConfirm();
		return;
	}
	else if( Q_strstr( command, "confirmyes" ) )
	{
		if( m_nCharToDelete == CHARSLOT_ONE )
		{
			engine->ClientCmd( "deletechar 1" );
			m_bJustDeleted[1] = true;
		}
		else if( m_nCharToDelete == CHARSLOT_TWO )
		{
			engine->ClientCmd( "deletechar 2" );
			m_bJustDeleted[2] = true;
		}
		else if( m_nCharToDelete == CHARSLOT_THREE )
		{
			engine->ClientCmd( "deletechar 3" );
			m_bJustDeleted[3] = true;
		}
		HideConfirm();
		Reset(); // BOXBOX added here to update with prediction
		return;
	}
	else if( Q_strstr( command, "confirmno" ) )
	{
		HideConfirm();
		return;
	}
//	else if( Q_strstr( command, "fullcharok" ) )
//	{
//		HideFullChar();
//		return;
//	}

	else
		BaseClass::OnCommand( command );
}

void CMSCharSelectMenu::OnClose( )
{
	m_bJustDeleted[ CHARSLOT_ONE ] = false;
	m_bJustDeleted[ CHARSLOT_TWO ] = false;
	m_bJustDeleted[ CHARSLOT_THREE ] = false;

	BaseClass::OnClose( );
}

void CMSCharSelectMenu::ShowConfirm( void )
{
	m_pConfirmBgImage->SetVisible( true );
	m_pConfirmLabel->SetVisible( true );
	m_pConfirmYesButton->SetVisible( true );
	m_pConfirmYesButton->SetEnabled( true );
	m_pConfirmNoButton->SetVisible( true );
	m_pConfirmNoButton->SetEnabled( true );

	Button *pButton = (Button *)FindChildByName( "CharButton1" );
	pButton->SetEnabled( false );
	pButton->SetVisible( false );
	pButton = (Button *)FindChildByName( "CharButton2" );
	pButton->SetEnabled( false );
	pButton->SetVisible( false );
	pButton = (Button *)FindChildByName( "CharButton3" );
	pButton->SetEnabled( false );
	pButton->SetVisible( false );
	pButton = (Button *)FindChildByName( "DeleteCharButton1" );
	pButton->SetEnabled( false );
	pButton = (Button *)FindChildByName( "DeleteCharButton2" );
	pButton->SetEnabled( false );
	pButton = (Button *)FindChildByName( "DeleteCharButton3" );
	pButton->SetEnabled( false );
//	pButton = (Button *)FindChildByName( "NewCharButton" );
//	pButton->SetEnabled( false );
}

void CMSCharSelectMenu::HideConfirm( void )
{
	m_pConfirmBgImage->SetVisible( false );
	m_pConfirmLabel->SetVisible( false );
	m_pConfirmYesButton->SetVisible( false );
	m_pConfirmYesButton->SetEnabled( false );
	m_pConfirmNoButton->SetVisible( false );
	m_pConfirmNoButton->SetEnabled( false );

	Button *pButton = (Button *)FindChildByName( "CharButton1" );
	pButton->SetEnabled( true );
	pButton->SetVisible( true );
	pButton = (Button *)FindChildByName( "CharButton2" );
	pButton->SetEnabled( true );
	pButton->SetVisible( true );
	pButton = (Button *)FindChildByName( "CharButton3" );
	pButton->SetEnabled( true );
	pButton->SetVisible( true );
	pButton = (Button *)FindChildByName( "DeleteCharButton1" );
	pButton->SetEnabled( true );
	pButton = (Button *)FindChildByName( "DeleteCharButton2" );
	pButton->SetEnabled( true );
	pButton = (Button *)FindChildByName( "DeleteCharButton3" );
	pButton->SetEnabled( true );
//	pButton = (Button *)FindChildByName( "NewCharButton" );
//	pButton->SetEnabled( true );
	Reset();
}
/*
void CMSCharSelectMenu::ShowFullChar( void )
{
	m_pFullCharBgImage->SetVisible( true );
	m_pFullCharLabel->SetVisible( true );
	m_pFullCharButton->SetVisible( true );
	m_pFullCharButton->SetEnabled( true );

	Button *pButton = (Button *)FindChildByName( "CharButton1" );
	pButton->SetEnabled( false );
	pButton->SetVisible( false );
	pButton = (Button *)FindChildByName( "CharButton2" );
	pButton->SetEnabled( false );
	pButton->SetVisible( false );
	pButton = (Button *)FindChildByName( "CharButton3" );
	pButton->SetEnabled( false );
	pButton->SetVisible( false );
	pButton = (Button *)FindChildByName( "DeleteCharButton1" );
	pButton->SetEnabled( false );
	pButton = (Button *)FindChildByName( "DeleteCharButton2" );
	pButton->SetEnabled( false );
	pButton = (Button *)FindChildByName( "DeleteCharButton3" );
	pButton->SetEnabled( false );
	pButton = (Button *)FindChildByName( "NewCharButton" );
	pButton->SetEnabled( false );
}

void CMSCharSelectMenu::HideFullChar( void )
{
	m_pFullCharBgImage->SetVisible( false );
	m_pFullCharLabel->SetVisible( false );
	m_pFullCharButton->SetVisible( false );
	m_pFullCharButton->SetEnabled( false );

	Button *pButton = (Button *)FindChildByName( "CharButton1" );
	pButton->SetEnabled( true );
	pButton->SetVisible( true );
	pButton = (Button *)FindChildByName( "CharButton2" );
	pButton->SetEnabled( true );
	pButton->SetVisible( true );
	pButton = (Button *)FindChildByName( "CharButton3" );
	pButton->SetEnabled( true );
	pButton->SetVisible( true );
	pButton = (Button *)FindChildByName( "DeleteCharButton1" );
	pButton->SetEnabled( true );
	pButton = (Button *)FindChildByName( "DeleteCharButton2" );
	pButton->SetEnabled( true );
	pButton = (Button *)FindChildByName( "DeleteCharButton3" );
	pButton->SetEnabled( true );
	pButton = (Button *)FindChildByName( "NewCharButton" );
	pButton->SetEnabled( true );
}
*/
vgui::Panel *CMSCharSelectMenu::CreateControlByName(const char *controlName)
{
     return BaseClass::CreateControlByName( controlName );
}