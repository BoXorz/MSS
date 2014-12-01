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

#define MSS_PLAYER_MODEL "models/player/humanmale.mdl"

CUtlVector<CMSCharSelectMenu::char_selection_spawnpoint_info_s> CMSCharSelectMenu::m_CharSelectSpots;

Vector CMSCharSelectMenu::g_ViewPos;
QAngle CMSCharSelectMenu::g_ViewAng;
CMSCharSelectMenu *CMSCharSelectMenu::static_pCurrentMenu = NULL;

LINK_ENTITY_TO_CLASS( ms_clientsidemodel, CClientSidePlayerModel );
//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CMSCharSelectMenu::CMSCharSelectMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_CHARSELECT ),
	m_DisplayedCharacters( false ), m_CharsFollowCam( true )
{
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/MSScheme.res", "MSScheme");
	SetScheme(scheme);

	SetMoveable( false );
	SetSizeable( false );
	SetProportional( true );

	// hide the system buttons
	SetTitleBarVisible( false );

	m_pTitleLabel	= new Label( this, "TitleText", "Title Text" );
	m_pSlot1Label	= new Label( this, "Text_Slot1", "Slot 1 Text" );
	m_pSlot2Label	= new Label( this, "Text_Slot2", "Slot 2 Text" );
	m_pSlot3Label	= new Label( this, "Text_Slot3", "Slot 3 Text" );

}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CMSCharSelectMenu::~CMSCharSelectMenu()
{
	
}

void CMSCharSelectMenu::ApplySchemeSettings( IScheme *pScheme ) // BOXBOX added this
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSCharacterSelect.res");

	m_pTitleLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
	m_pTitleLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );

	m_pSlot1Label->SetFont( pScheme->GetFont( "HeaderFont" ) );
	m_pSlot1Label->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
	m_pSlot2Label->SetFont( pScheme->GetFont( "WritingFont" ) );
	m_pSlot2Label->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
	m_pSlot3Label->SetFont( pScheme->GetFont( "WritingFont" ) );
	m_pSlot3Label->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );

}


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
//-----------------------------------------------------------------------------
// Purpose: Clears the screen before building it
//-----------------------------------------------------------------------------
void CMSCharSelectMenu::Reset( )
{
//	LoadControlSettings("Resource/UI/MSCharacterSelect.res");
	m_DisplayedCharacters = false;

	//Setup the characters (If using static positions)
	if( !CMSCharSelectMenu::m_CharSelectSpots.Count( ) )
		for( int i = 0; i < 3; i++ )
		{
			CMSCharSelectMenu::char_selection_spawnpoint_info_s NewSpot;
			GetCharPos( i, NewSpot );

			CMSCharSelectMenu::m_CharSelectSpots.AddToTail( NewSpot );
		}

	for( int i = 0; i < GetChildCount( ); i++ )
	{
		if( string( GetChild( i )->GetClassName( ) ) == "Label" )
		{
			Label *pLabel = (Label *)GetChild( i );
			if( string( pLabel->GetName( ) ) != "TitleText" )
				pLabel->SizeToContents( );
		}
		/*
		if( string( GetChild( i )->GetClassName( ) ) == "ImagePanel" )
		{
			ImagePanel *pPanel = (ImagePanel *)GetChild( i );
			if( pPanel->GetImage( ) )
			{
				int wide, tall;
				pPanel->GetImage( )->GetContentSize( wide, tall );
				pPanel->SetSize( wide, tall );
			}
		}
		*/
	}

	for( int i = 0; i < MAX_CHAR_SLOTS; i++ )
	{
		char sName[16];
		int slotidx = i + 1;
		Q_snprintf( sName, sizeof(sName), "Text_Slot%i", slotidx );
			
		Label *pLabel = (Label *)FindChildByName( sName );
		if( pLabel )
		{
			int x, y;
			pLabel->GetPos( x, y );
			pLabel->SetPos( x, scheme()->GetProportionalScaledValueEx(pLabel->GetScheme(), m_SlotTitleY ) );
			pLabel->SetVisible( false );
		}

		Q_snprintf( sName, sizeof(sName), "Button_Slot%i", slotidx );

		Button *pButton = (Button *)FindChildByName( sName );
		if( pButton )
		{
			int x, y;
			pButton->GetPos( x, y );
			pButton->SetPos( x, scheme()->GetProportionalScaledValueEx(pButton->GetScheme(), m_SlotY) );
			pButton->SetSize( scheme()->GetProportionalScaledValueEx(pButton->GetScheme(), m_SlotW) , scheme()->GetProportionalScaledValueEx(pButton->GetScheme(), m_SlotH) );
			pButton->SetText( "" );
			pButton->SetEnabled( false );
		}
	}


	if( !m_SelectionCharacters.Count( ) )
		for( int i = 0; i < m_CharSelectSpots.Count( ); i++ )
		{
			const char_selection_spawnpoint_info_s &Point = m_CharSelectSpots[i];

			CClientSidePlayerModel *pPlayerModel = dynamic_cast<CClientSidePlayerModel* >( CreateEntityByName( "ms_clientsidemodel" ) );
			if( pPlayerModel )
			{
				Q_strcpy( pPlayerModel->m_ModelName, MSS_PLAYER_MODEL );
				pPlayerModel->InitializeAsClientEntity( pPlayerModel->m_ModelName, RENDER_GROUP_OPAQUE_ENTITY );
				pPlayerModel->AddEffects( EF_NODRAW ); // don't let the renderer draw the model normally
				pPlayerModel->SetAbsOrigin( Point.Pos );
				pPlayerModel->SetAbsAngles( Point.Rot );

				pPlayerModel->SetSequence( pPlayerModel->SelectWeightedSequence( ACT_IDLE ) );
				pPlayerModel->SetPlaybackRate( 1.0f );
				pPlayerModel->SetPoseParameter( 0, 0.0f ); // move_yaw
				pPlayerModel->SetPoseParameter( 1, 10.0f ); // body_pitch, look down a bit
				pPlayerModel->SetPoseParameter( 2, 0.0f ); // body_yaw
				pPlayerModel->SetPoseParameter( 3, 0.0f ); // move_y
				pPlayerModel->SetPoseParameter( 4, 0.0f ); // move_x
				pPlayerModel->m_SlotIndex = i;

				pPlayerModel->m_Hidden = true;

				CHandle<CClientSidePlayerModel> ehandlePlayerModel( pPlayerModel );
				CClientSidePlayerModel::g_ClientSideModels.AddToTail( ehandlePlayerModel );
				m_SelectionCharacters.AddToTail( pPlayerModel );
			}
		}

		CMSCharSelectMenu::static_pCurrentMenu = this;
}

void CMSCharSelectMenu::OnClose( )
{
	Remove3DCharacters( );

	BaseClass::OnClose( );
}
//-----------------------------------------------------------------------------
// Purpose: Displays the window
//-----------------------------------------------------------------------------
void CMSCharSelectMenu::ShowPanel(bool bShow)
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
//		Msg("CHAR PANEL OPENING\n");
	}

	// Closes the window
	else
		Close( );
//		Msg("CHAR PANEL CLOSING\n");
}

bool CMSCharSelectMenu::NeedsUpdate( void )
{
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSCharSelectMenu::Update( void )
{
	C_MSS_Player *pLocalPlayer = ToHL2MPPlayer( C_BasePlayer::GetLocalPlayer() );
	if( pLocalPlayer )
	{
		if( pLocalPlayer->m_PreloadedCharInfo_DoneSending && !m_DisplayedCharacters )
		{
			for( int i = 0; i < pLocalPlayer->m_PreloadedCharInfo_Model.Count(); i++ )
			{
				char sName[16];
				int slotidx = i + 1;
				Q_snprintf( sName, sizeof(sName), "Text_Slot%i", slotidx );
				Label *pLabel = (Label *)FindChildByName( sName );
				if( pLabel )
				{
					pLabel->SetText( pLocalPlayer->m_PreloadedCharInfo_Name[i] );
					pLabel->SizeToContents( );
				}

				Q_snprintf( sName, sizeof(sName), "Button_Slot%i", slotidx );

				Button *pButton = (Button *)FindChildByName( sName );


				if( pLocalPlayer->m_PreloadedCharInfo_Model[i] == 0 )
				{
					if( m_SelectionCharacters.Count() > i && m_SelectionCharacters[i].Get( ) )
						m_SelectionCharacters[i]->m_Hidden = true;
					if( pButton )
						pButton->SetEnabled( false );
					pLabel->SetVisible( false );

				}
				else
				{
					if( m_SelectionCharacters.Count() > i && m_SelectionCharacters[i].Get( ) )
						m_SelectionCharacters[i]->m_Hidden = false;
					if( pButton )
						pButton->SetEnabled( true );
					pLabel->SetVisible( true );
				}
			}

			m_DisplayedCharacters = true;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSCharSelectMenu::Remove3DCharacters()
{
	static_pCurrentMenu = NULL;

	int count = m_SelectionCharacters.Count( );
	for( int i = 0; i < count; i++ )
	{
		if( m_SelectionCharacters[i].Get( ) )
			(m_SelectionCharacters[i].Get( ))->Remove( );
		CClientSidePlayerModel::g_ClientSideModels.Remove( i );
	}

	m_SelectionCharacters.RemoveAll( );
	m_CharSelectSpots.RemoveAll( );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSCharSelectMenu::OnCommand( const char *command ) // BOXBOX TODO should I change this to allow for arguments? or let ClientCmd handle it on server side?
{
	if( Q_strstr( command, "choosechar" ) )
	{
		engine->ClientCmd( command );
		gViewPortInterface->ShowPanel( this, false );
		vgui::surface()->PlaySound( "UI/pageturn.wav" ); // BOXBOX added
		return;
	}
	else if( Q_strstr( command, "createchar" ) )
	{
		gViewPortInterface->ShowPanel( this, false );
		gViewPortInterface->ShowPanel( PANEL_CHARCREATE, true );
		vgui::surface()->PlaySound( "UI/pageturn.wav" ); // BOXBOX added
		return;
	}
	else if( Q_strstr( command, "deletechar" ) )
	{
		Warning("DELETING CHARACTERS IS NOT INSTALLED YET!\n");
		return;
	}
	else
		BaseClass::OnCommand( command );
}

vgui::Panel *CMSCharSelectMenu::CreateControlByName(const char *controlName)
{
    /*if ( Q_stricmp( controlName, "ClassImagePanel" ) == 0 )
    {
        return new CClassImagePanel( NULL, controlName );
    }*/

    return BaseClass::CreateControlByName( controlName );
}


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