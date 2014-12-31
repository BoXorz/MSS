
// BOXBOX The Character Selection Menu

#include "cbase.h"
#include <stdio.h>
#include <string>

#include "MSS_shareddefs.h"
#include "MSSCharacterSelect.h"
#include "usermessages.h"

using namespace vgui;
using namespace std;

#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui/ISurface.h>
#include <cdll_client_int.h>
#include "clientmode_MSSnormal.h"

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

	m_pFullCharBgImage		= new vgui::ImagePanel( this, "FullCharBGImg" );
	m_pFullCharLabel		= new vgui::Label( this, "FullCharLabel", "#MSS_FULL_CHAR" );
	m_pFullCharButton		= new vgui::Button( this, "FullCharButton", "#MSS_OK", this, "fullcharok" );

	m_bJustDeleted[0] = false;
	m_bJustDeleted[1] = false;
	m_bJustDeleted[2] = false;
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

	m_pFullCharLabel->SetFont( pScheme->GetFont( "HeaderFontSmall" ) );
	m_pFullCharLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
}

void CMSCharSelectMenu::ShowPanel(bool bShow)
{
	// If already open, do nothing
	if ( BaseClass::IsVisible() == bShow )
		return;

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

	for( int i = 0; i< GetChildCount(); i++ ) 
	{
		CModelPanel *panel = dynamic_cast<CModelPanel *>( GetChild( i ) );

		if ( panel )
		{
			if( !Q_stricmp( panel->GetName(), "charone" ) )
			{
				panel->SetVisible( pPlayer->m_bHasCharInSlot[ CHARSLOT_ONE ] );
//				panel->m_hModel->SetModel( pszPlayerModels[pPlayer->m_PreloadedCharInfo_Model.Get( 0 )] ); // BOXBOX attempt to set model
				if( m_bJustDeleted[ CHARSLOT_ONE ] )
					panel->SetVisible( false );
			}
			else if( !Q_stricmp( panel->GetName(), "chartwo" ) )
			{
				panel->SetVisible( pPlayer->m_bHasCharInSlot[ CHARSLOT_TWO ] );
				if( m_bJustDeleted[ CHARSLOT_TWO ] )
					panel->SetVisible( false );
			}			
			else if( !Q_stricmp( panel->GetName(), "charthree" ) )
			{
				panel->SetVisible( pPlayer->m_bHasCharInSlot[ CHARSLOT_THREE ] );
				if( m_bJustDeleted[ CHARSLOT_THREE ] )
					panel->SetVisible( false );
			}			
		}
	}

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
	pButton->SetText( pPlayer->m_szPreloadCharName1[0] ? "" : "#MSS_NOCHAR" );
	pButton->SetEnabled( pPlayer->m_szPreloadCharName1[0] ? true : false );
	if( m_bJustDeleted[ CHARSLOT_ONE ] )
	{
		pButton->SetText( "#MSS_NOCHAR" );
		pButton->SetEnabled( false );
	}
	pButton = (Button *)FindChildByName( "CharButton2" );
	pButton->SetText( pPlayer->m_szPreloadCharName2[0] ? "" : "#MSS_NOCHAR" );
	pButton->SetEnabled( pPlayer->m_szPreloadCharName2[0] ? true : false );
	if( m_bJustDeleted[ CHARSLOT_TWO ] )
	{
		pButton->SetText( "#MSS_NOCHAR" );
		pButton->SetEnabled( false );
	}
	pButton = (Button *)FindChildByName( "CharButton3" );
	pButton->SetText( pPlayer->m_szPreloadCharName3[0] ? "" : "#MSS_NOCHAR" );
	pButton->SetEnabled( pPlayer->m_szPreloadCharName3[0] ? true : false );
	if( m_bJustDeleted[ CHARSLOT_THREE ] )
	{
		pButton->SetText( "#MSS_NOCHAR" );
		pButton->SetEnabled( false );
	}
}

void CMSCharSelectMenu::Update( void ) // BOXBOX don't need anything running constantly
{
}

void CMSCharSelectMenu::OnCommand( const char *command )
{
	C_MSS_Player *pPlayer = ToMSSPlayer( C_BasePlayer::GetLocalPlayer() );
	if( !pPlayer ) return;

	if( Q_strstr( command, "choosechar" ) )
	{
		engine->ClientCmd( command );
		gViewPortInterface->ShowPanel( this, false );
		vgui::surface()->PlaySound( "UI/pageturn.wav" );
		return;
	}
	else if( Q_strstr( command, "createchar" ) )
	{
		if( pPlayer->GetNumChars() == MAX_CHAR_SLOTS )
		{
			ShowFullChar();
			return;
		}

		gViewPortInterface->ShowPanel( this, false );
		gViewPortInterface->ShowPanel( PANEL_CHARCREATE, true );
		vgui::surface()->PlaySound( "UI/pageturn.wav" );
		return;
	}
	else if( Q_strstr( command, "deletechar 1" ) )
	{
		if( !pPlayer->m_nPreloadModelIndex[ CHARSLOT_ONE ] )// BOXBOX no character to delete
		{
			return;
		}
		m_nCharToDelete = 1;
		ShowConfirm();
		return;
	}
	else if( Q_strstr( command, "deletechar 2" ) )
	{
		if( !pPlayer->m_nPreloadModelIndex[ CHARSLOT_TWO ] )
		{
			return;
		}
		m_nCharToDelete = 2;
		ShowConfirm();
		return;
	}
	else if( Q_strstr( command, "deletechar 3" ) )
	{
		if( !pPlayer->m_nPreloadModelIndex[ CHARSLOT_THREE ] )
		{
			return;
		}
		m_nCharToDelete = 3;
		ShowConfirm();
		return;
	}
	else if( Q_strstr( command, "confirmyes" ) )
	{
		if( m_nCharToDelete == 1 )
		{
			engine->ClientCmd( "deletechar 1" );
			m_bJustDeleted[1] = true;
		}
		else if( m_nCharToDelete == 2 )
		{
			engine->ClientCmd( "deletechar 2" );
			m_bJustDeleted[2] = true;
		}
		else if( m_nCharToDelete == 3 )
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
	else if( Q_strstr( command, "fullcharok" ) )
	{
		HideFullChar();
		return;
	}

	else
		BaseClass::OnCommand( command );
}

void CMSCharSelectMenu::OnClose( )
{
	m_bJustDeleted[0] = false;
	m_bJustDeleted[1] = false;
	m_bJustDeleted[2] = false;

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
	pButton = (Button *)FindChildByName( "NewCharButton" );
	pButton->SetEnabled( false );
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
	pButton = (Button *)FindChildByName( "NewCharButton" );
	pButton->SetEnabled( true );
	Reset();
}

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

vgui::Panel *CMSCharSelectMenu::CreateControlByName(const char *controlName)
{
     return BaseClass::CreateControlByName( controlName );
}