#include "cbase.h"

//========= Copyright © 2008, Mike Raineri, All rights reserved. ============//
//
// Purpose: Builds the character window
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

CUtlVector<CMSCharCreateMenu::char_selection_spawnpoint_info_s> CMSCharCreateMenu::m_CharSelectSpots;

Vector CMSCharCreateMenu::g_ViewPos;
QAngle CMSCharCreateMenu::g_ViewAng;
CMSCharCreateMenu *CMSCharCreateMenu::static_pCurrentMenu = NULL;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CMSCharCreateMenu::CMSCharCreateMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_CHARCREATE )
{
	// set the scheme before any child control is created
	SetScheme("ClientScheme");

	SetMoveable( false );
	SetSizeable( false );
	SetProportional( true );
	SetKeyBoardInputEnabled( true );

	// hide the system buttons
	SetTitleBarVisible( false );
	m_CharsFollowCam = true;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CMSCharCreateMenu::~CMSCharCreateMenu()
{
	
}

void CMSCharCreateMenu::GetCharPos( int charIdx, CMSCharCreateMenu::char_selection_spawnpoint_info_s &out_Pos )
{
	if( m_CharsFollowCam )
	{
		Vector vForward, vRight, vUp;
		AngleVectors( CMSCharCreateMenu::g_ViewAng, &vForward, &vRight, &vUp );

		out_Pos.Pos =  CMSCharCreateMenu::g_ViewPos 
					+ vForward * m_DistFromCamera 
					+ vRight * m_DistFromCamera_Side 
					+ vRight * m_DistFromCamera_SideSpacing * (charIdx-1)
					+ vUp * m_DistFromCamera_Up;

		Vector vFacingPlayer = CMSCharCreateMenu::g_ViewPos - out_Pos.Pos;
		vFacingPlayer.NormalizeInPlace( );
		out_Pos.Rot = QAngle( 0, UTIL_VecToYaw(vFacingPlayer), 0 );
	}
}
//-----------------------------------------------------------------------------
// Purpose: Clears the screen before building it
//-----------------------------------------------------------------------------
void CMSCharCreateMenu::Reset( )
{
	LoadControlSettings("Resource/UI/MSCharacterCreate.res");

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

	for( int i = 0; i < 3; i++ )
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
		}
	}


	/*if( !m_SelectionCharacters.Count( ) )
		for( int i = 0; i < m_CharSelectSpots.Count( ); i++ )
		{
			char_selection_spawnpoint_info_s &Point = m_CharSelectSpots[i];

			CClientSidePlayerModel *pPlayerModel = new CClientSidePlayerModel( );
			Q_strcpy( pPlayerModel->m_ModelName, "models/Combine_Soldier.mdl" );
			pPlayerModel->AddEffects( EF_NODRAW ); // don't let the renderer draw the model normally
			pPlayerModel->InitializeAsClientEntity( pPlayerModel->m_ModelName, RENDER_GROUP_OPAQUE_ENTITY );
			pPlayerModel->SetAbsOrigin( Point.Pos );
			pPlayerModel->SetAbsAngles( Point.Rot );
			pPlayerModel->ToggleBBoxVisualization( CBaseEntity::VISUALIZE_SURROUNDING_BOUNDS );

			if( i == 1 )
				pPlayerModel->SetSequence( pPlayerModel->LookupSequence( "Run_Melee" ) );
			else
				pPlayerModel->SetSequence( pPlayerModel->LookupSequence( "Idle_Slam" ) );
			pPlayerModel->SetPoseParameter( 0, 0.0f ); // move_yaw
			pPlayerModel->SetPoseParameter( 1, 10.0f ); // body_pitch, look down a bit
			pPlayerModel->SetPoseParameter( 2, 0.0f ); // body_yaw
			pPlayerModel->SetPoseParameter( 3, 0.0f ); // move_y
			pPlayerModel->SetPoseParameter( 4, 0.0f ); // move_x
			pPlayerModel->m_SlotIndex = i;

			CHandle<CClientSidePlayerModel> ehandlePlayerModel( pPlayerModel );
			CClientSidePlayerModel::g_ClientSideModels.AddToTail( ehandlePlayerModel );
			m_SelectionCharacters.AddToTail( pPlayerModel );
		}*/

		CMSCharCreateMenu::static_pCurrentMenu = this;
}

//-----------------------------------------------------------------------------
// Purpose: Displays the window
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSCharCreateMenu::Update( void )
{
}

void CMSCharCreateMenu::OnClose( )
{
	//Remove3DCharacters( );

	BaseClass::OnClose( );
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSCharCreateMenu::Remove3DCharacters()
{
	/*static_pCurrentMenu = NULL;

	int count = m_SelectionCharacters.Count( );
	for( int i = 0; i < count; i++ )
	{
		if( m_SelectionCharacters[i].Get( ) )
			(m_SelectionCharacters[i].Get( ))->Remove( );
		CClientSidePlayerModel::g_ClientSideModels.Remove( i );
	}

	m_SelectionCharacters.RemoveAll( );
	m_CharSelectSpots.RemoveAll( );*/
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSCharCreateMenu::OnCommand( const char *command )
{
	if( Q_strstr( command, "createchar" ) )
	{
		bool dataIsValid = true;
		char charName[256] = "peon";
		int gender = 0;


		TextEntry *pCharName = (TextEntry *)FindChildByName( "Text_ChooseName_TextBox" );
		pCharName->GetText( charName, 256 );

		if( !charName[0] )
			dataIsValid = false;

		RadioButton *pGenderMale = (RadioButton *)FindChildByName( "Button_GenderMale" );
		RadioButton *pGenderFemale = (RadioButton *)FindChildByName( "Button_GenderFemale" );
		if( pGenderMale->IsSelected() )
			gender = 0;
		else if( pGenderFemale->IsSelected() )
			gender = 1;
		else
			dataIsValid = false;

		if( dataIsValid )
		{
			string sendCommand = VarArgs( "createchar \"%s\" %i", charName, gender );
			engine->ClientCmd( sendCommand.c_str() );
			gViewPortInterface->ShowPanel( this, false );
			vgui::surface()->PlaySound( "UI/pageturn.wav" ); // BOXBOX added
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

