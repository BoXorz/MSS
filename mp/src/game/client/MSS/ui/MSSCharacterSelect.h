//========= Copyright © 2008, Mike Raineri, All rights reserved. ============//
//
// Purpose: Header file for the character menu
//
// $NoKeywords: $
//=============================================================================//

#ifndef MS_CHARMENU_H
#define MS_CHARMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <game/client/iviewport.h>
#include "MSSClient3DModel.h"
#include "ienginevgui.h"
#include "clientmode_MSSnormal.h"
#include "basemodelpanel.h" // BOXBOX added for models

#pragma warning( disable : 4481 )

//-----------------------------------------------------------------------------
// Purpose: Character menu
//-----------------------------------------------------------------------------
class CMSCharSelectMenu : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CMSCharSelectMenu, vgui::Frame );

public:
	CMSCharSelectMenu( IViewPort *pViewPort );
	virtual ~CMSCharSelectMenu();

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme ); // BOXBOX added this

	virtual void OnCommand(const char *command);

	virtual const char *GetName( void ) { return PANEL_CHARSELECT; }
	virtual void SetData(KeyValues *data) {};
	virtual void Reset();
	virtual void Update();
	virtual bool NeedsUpdate( void ) { return true; }
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );
//	virtual void Remove3DCharacters();
	virtual void OnClose( ) override;

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
  	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }		
	vgui::Panel *CMSCharSelectMenu::CreateControlByName(const char *controlName);

	//Camera view position
	static Vector g_ViewPos;
	static QAngle g_ViewAng;

//	bool m_CharsFollowCam;
//	bool m_DisplayedCharacters;
	static CMSCharSelectMenu *static_pCurrentMenu;

/*	//Character positions
	struct char_selection_spawnpoint_info_s
	{
		Vector Pos;
		QAngle Rot;
	};
*/
//	void GetCharPos( int charIdx, char_selection_spawnpoint_info_s &out_Pos );	//Get position of a ghost character

//	static CUtlVector<char_selection_spawnpoint_info_s> m_CharSelectSpots;
//	CUtlVector<CHandle<CClientSidePlayerModel>> m_SelectionCharacters;
/*
	CPanelAnimationVar( float, m_DistFromCamera, "3dchar_dist_forward", "float" );
	CPanelAnimationVar( float, m_DistFromCamera_Side, "3dchar_dist_side", "float" );
	CPanelAnimationVar( float, m_DistFromCamera_Up, "3dchar_dist_up", "float" );
	CPanelAnimationVar( float, m_DistFromCamera_SideSpacing, "3dchar_dist_side_spacing", "float" );
	CPanelAnimationVar( float, m_SlotTitleY, "SlotTitle_Y", "float" );
	CPanelAnimationVar( float, m_SlotY, "Slot_Y", "float" );
	CPanelAnimationVar( float, m_SlotW, "Slot_W", "float" );
	CPanelAnimationVar( float, m_SlotH, "Slot_H", "float" );
*/
protected:

	vgui::Label		*m_pTitleLabel;
	vgui::Label		*m_pSlot1Label;
	vgui::Label		*m_pSlot2Label;
	vgui::Label		*m_pSlot3Label;

//	vgui::Button	*m_pCharButton1;
//	vgui::Button	*m_pCharButton2;
//	vgui::Button	*m_pCharButton3;

//	CModelPanel	*m_pCharModel1;
//	CModelPanel	*m_pCharModel2;
//	CModelPanel	*m_pCharModel3;

	vgui::ImagePanel	*m_pConfirmBgImage; // BOXBOX child controls that pop up, asking if you are sure you want to delete a character
	vgui::Label		*m_pConfirmLabel;	// the text
	vgui::Button	*m_pConfirmYesButton;
	vgui::Button	*m_pConfirmNoButton;
	virtual void ShowConfirm( void );
	virtual void HideConfirm( void );

	vgui::ImagePanel	*m_pFullCharBgImage; // BOXBOX child controls that pop up if you try to create a character, and you already have 3
	vgui::Label		*m_pFullCharLabel;	// the text
	vgui::Button	*m_pFullCharButton;
	virtual void ShowFullChar( void );
	virtual void HideFullChar( void );

	int		m_nCharToDelete;
	bool	m_bJustDeleted[4]; // BOXBOX the server can't update characters and send info back fast enough, so use this as a makeshift 'prediction' so menu acts right.
};


#endif // MS_CHARMENU_H