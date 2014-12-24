//========= Copyright © 2008, Mike Raineri, All rights reserved. ============//
//
// Purpose: Builds the character window
//
// $NoKeywords: $
//=============================================================================//

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

//#define MSS_PLAYER_MODEL "models/player/humanmale.mdl"

extern const char *pszPlayerModels[];

//CUtlVector<CMSCharSelectMenu::char_selection_spawnpoint_info_s> CMSCharSelectMenu::m_CharSelectSpots;

//Vector CMSCharSelectMenu::g_ViewPos;
//QAngle CMSCharSelectMenu::g_ViewAng;
//CMSCharSelectMenu *CMSCharSelectMenu::static_pCurrentMenu = NULL;

//LINK_ENTITY_TO_CLASS( ms_clientsidemodel, CClientSidePlayerModel );
//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CMSCharSelectMenu::CMSCharSelectMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_CHARSELECT )/*,
	m_DisplayedCharacters( false ), m_CharsFollowCam( true )*/
{
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/MSScheme.res", "MSScheme");
	SetScheme(scheme);

	SetMoveable( false );
	SetSizeable( false );
	SetProportional( true );
	SetTitleBarVisible( false );

	m_pTitleLabel	= new Label( this, "TitleText", "Title Text" );
	m_pSlot1Label	= new Label( this, "CharText1", "" );
	m_pSlot2Label	= new Label( this, "CharText2", "" );
	m_pSlot3Label	= new Label( this, "CharText3", "" );

//	m_pCharButton1		= new vgui::Button( this, "CharButton1", "#MSS_NOCHAR", this, "choosechar 0" );
//	m_pCharButton2		= new vgui::Button( this, "CharButton2", "#MSS_NOCHAR", this, "choosechar 1" );
//	m_pCharButton3		= new vgui::Button( this, "CharButton3", "#MSS_NOCHAR", this, "choosechar 2" );

//	m_pCharModel1 = new CModelPanel( this, "charmodel1" );
//	m_pCharModel2 = new CModelPanel( this, "charmodel2" );
//	m_pCharModel3 = new CModelPanel( this, "charmodel3" );


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

void CMSCharSelectMenu::ApplySchemeSettings( IScheme *pScheme ) // BOXBOX added this
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSCharacterSelect.res");

	m_pTitleLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
	m_pTitleLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );

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

	IViewPortPanel *panel = gViewPortInterface->FindPanelByName( PANEL_MAINMENU ); // BOXBOX don't open if main menu is open
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
//		Msg("CHAR PANEL OPENING\n");
	}

	// Closes the window
	else
		Close( );
//		Msg("CHAR PANEL CLOSING\n");
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
				panel->SetVisible( pPlayer->m_PreloadedCharInfo_Model.Get( 0 ) != MODEL_NOCHAR );
				if( m_bJustDeleted[0] )
					panel->SetVisible( false );
			}
			else if( !Q_stricmp( panel->GetName(), "chartwo" ) )
			{
				panel->SetVisible( pPlayer->m_PreloadedCharInfo_Model.Get( 1 ) != MODEL_NOCHAR );
				if( m_bJustDeleted[1] )
					panel->SetVisible( false );
			}			
			else if( !Q_stricmp( panel->GetName(), "charthree" ) )
			{
				panel->SetVisible( pPlayer->m_PreloadedCharInfo_Model.Get( 2 ) != MODEL_NOCHAR );
				if( m_bJustDeleted[2] )
					panel->SetVisible( false );
			}			
		}
	}

	m_pSlot1Label->SetText( pPlayer->m_szPreloadCharName0 );
	if( m_bJustDeleted[0] )
		m_pSlot1Label->SetText( "" );
	m_pSlot2Label->SetText( pPlayer->m_szPreloadCharName1 );
	if( m_bJustDeleted[1] )
		m_pSlot2Label->SetText( "" );
	m_pSlot3Label->SetText( pPlayer->m_szPreloadCharName2 );
	if( m_bJustDeleted[2] )
		m_pSlot3Label->SetText( "" );

	Button *pButton = (Button *)FindChildByName( "CharButton1" );
	pButton->SetText( pPlayer->m_szPreloadCharName0[0] ? "" : "#MSS_NOCHAR" );
	pButton->SetEnabled( pPlayer->m_szPreloadCharName0[0] ? true : false );
	if( m_bJustDeleted[0] )
	{
		pButton->SetText( "#MSS_NOCHAR" );
		pButton->SetEnabled( false );
	}
	pButton = (Button *)FindChildByName( "CharButton2" );
	pButton->SetText( pPlayer->m_szPreloadCharName1[0] ? "" : "#MSS_NOCHAR" );
	pButton->SetEnabled( pPlayer->m_szPreloadCharName1[0] ? true : false );
	if( m_bJustDeleted[1] )
	{
		pButton->SetText( "#MSS_NOCHAR" );
		pButton->SetEnabled( false );
	}
	pButton = (Button *)FindChildByName( "CharButton3" );
	pButton->SetText( pPlayer->m_szPreloadCharName2[0] ? "" : "#MSS_NOCHAR" );
	pButton->SetEnabled( pPlayer->m_szPreloadCharName2[0] ? true : false );
	if( m_bJustDeleted[2] )
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
		vgui::surface()->PlaySound( "UI/pageturn.wav" ); // BOXBOX added
		return;
	}
	else if( Q_strstr( command, "createchar" ) )
	{
		if( pPlayer->m_nNumChars == MAX_CHAR_SLOTS )
		{
			ShowFullChar();
			return;
		}

		gViewPortInterface->ShowPanel( this, false );
		gViewPortInterface->ShowPanel( PANEL_CHARCREATE, true );
		vgui::surface()->PlaySound( "UI/pageturn.wav" ); // BOXBOX added
		return;
	}
	else if( Q_strstr( command, "deletechar 0" ) )
	{
		if( !pPlayer->m_PreloadedCharInfo_Model.Get( 0 ) )// BOXBOX no character to delete
		{
			return;
		}
		m_nCharToDelete = 0;
		ShowConfirm();
		return;
	}
	else if( Q_strstr( command, "deletechar 1" ) )
	{
		if( !pPlayer->m_PreloadedCharInfo_Model.Get( 1 ) )
		{
			return;
		}
		m_nCharToDelete = 1;
		ShowConfirm();
		return;
	}
	else if( Q_strstr( command, "deletechar 2" ) )
	{
		if( !pPlayer->m_PreloadedCharInfo_Model.Get( 2 ) )
		{
			return;
		}
		m_nCharToDelete = 2;
		ShowConfirm();
		return;
	}
	else if( Q_strstr( command, "confirmyes" ) )
	{
		if( m_nCharToDelete == 0 )
		{
			engine->ClientCmd( "deletechar 0" );
			m_bJustDeleted[0] = true;
		}
		else if( m_nCharToDelete == 1 )
		{
			engine->ClientCmd( "deletechar 1" );
			m_bJustDeleted[1] = true;
		}
		else if( m_nCharToDelete == 2 )
		{
			engine->ClientCmd( "deletechar 2" );
			m_bJustDeleted[2] = true;
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
    /*if ( Q_stricmp( controlName, "ClassImagePanel" ) == 0 )
    {
        return new CClassImagePanel( NULL, controlName );
    }*/

    return BaseClass::CreateControlByName( controlName );
}

/*
void CMSCharSelectMenu::GetCharPos( int charIdx, CMSCharSelectMenu::char_selection_spawnpoint_info_s &out_Pos )
{
	if( m_CharsFollowCam )
	{
		Vector vForward, vRight, vUp;
		AngleVectors( CMSCharSelectMenu::g_ViewAng, &vForward, &vRight, &vUp );

		out_Pos.Pos =  CMSCharSelectMenu::g_ViewPos 
					+ vForward * m_DistFromCamera 
					+ vRight * m_DistFromCamera_Side 
					+ vRight * m_DistFromCamera_SideSpacing * (charIdx-1)
					+ vUp * m_DistFromCamera_Up;

		Vector vFacingPlayer = CMSCharSelectMenu::g_ViewPos - out_Pos.Pos;
		vFacingPlayer.NormalizeInPlace( );
		out_Pos.Rot = QAngle( 0, UTIL_VecToYaw(vFacingPlayer), 0 );
	}
}
*/


/*
class C_InfoPlayerSelect : public CBaseEntity
{
public:
	DECLARE_CLASS( C_InfoPlayerSelect, CBaseEntity );
	DECLARE_CLIENTCLASS();

	CNetworkVar( int, m_Num );
	CNetworkVar( int, m_Initialized );

	void PostDataUpdate( DataUpdateType_t updateType )
	{
		if( m_Initialized )
		{
			//Position/Angles should be set by now
			CMSCharSelectMenu::char_selection_spawnpoint_info_s NewSpot;
			NewSpot.Pos = this->GetAbsOrigin( );
			NewSpot.Rot = this->GetAbsAngles( );

			CMSCharSelectMenu::m_CharSelectSpots.AddToTail( NewSpot );		
		}

		return BaseClass::PostDataUpdate( updateType );
	}

	void Spawn( )
	{
		//Position/Angles aren't set yet
	}
};

LINK_ENTITY_TO_CLASS( info_player_select, C_InfoPlayerSelect );

IMPLEMENT_CLIENTCLASS_DT( C_InfoPlayerSelect, DT_InfoPlayerSelect, CInfoPlayerSelect )
RecvPropVector( RECVINFO_NAME( m_vecAbsOrigin, m_vecOrigin ) ),
	RecvPropFloat( RECVINFO_NAME( m_angAbsRotation[0], m_angRotation[0] ) ),
	RecvPropFloat( RECVINFO_NAME( m_angAbsRotation[1], m_angRotation[1] ) ),
	RecvPropInt( RECVINFO( m_Num ) ),
	RecvPropBool( RECVINFO( m_Initialized ) ),
END_RECV_TABLE()


class CInfoPlayerStartCam : public CBaseEntity
{
public:
	DECLARE_CLASS( CInfoPlayerStartCam, CBaseEntity );
	DECLARE_CLIENTCLASS();

	//CNetworkVar( int, m_Num );
	CNetworkVar( int, m_Initialized );

	void PostDataUpdate( DataUpdateType_t updateType )
	{
		if( m_Initialized )
		{
			//Position/Angles should be set by now
			CMSCharSelectMenu::g_ViewPos = this->GetAbsOrigin( );
			CMSCharSelectMenu::g_ViewAng = this->GetAbsAngles( );
		}

		return BaseClass::PostDataUpdate( updateType );
	}

	void Spawn( )
	{
		//Position/Angles aren't set yet
	}
};

LINK_ENTITY_TO_CLASS( info_player_start_cam, CInfoPlayerStartCam );

IMPLEMENT_CLIENTCLASS_DT( CInfoPlayerStartCam, DT_InfoPlayerStart, CInfoPlayerStartCam )
RecvPropVector( RECVINFO_NAME( m_vecAbsOrigin, m_vecOrigin ) ),
	RecvPropFloat( RECVINFO_NAME( m_angAbsRotation[0], m_angRotation[0] ) ),
	RecvPropFloat( RECVINFO_NAME( m_angAbsRotation[1], m_angRotation[1] ) ),
	//RecvPropInt( RECVINFO( m_Num ) ),
	RecvPropBool( RECVINFO( m_Initialized ) ),
END_RECV_TABLE()
*/