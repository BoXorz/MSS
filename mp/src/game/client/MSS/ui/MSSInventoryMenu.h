//========= Copyright © 2007, Mike Raineri, All rights reserved. ============//
//
// Purpose: Header file for the inventory menu
//
// $NoKeywords: $
//=============================================================================//

#ifndef MS_INVENTORYMENU_H
#define MS_INVENTORYMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <game/client/iviewport.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/Button.h>
using namespace vgui;

#define MAX_BACKPACK_IMAGES 20
#define MAX_SIDEPACK_IMAGES 15
#define MAX_BELT_IMAGES 10
#define MAX_HANDS_IMAGES 6
#define MAX_EQUIP_IMAGES 6

#pragma warning( disable : 4481 )

class DraggableImage : public ImagePanel
{
public:

	DraggableImage( Panel *pParent, const char *name ) : ImagePanel( pParent, name ) { }
	void OnDraggablePanelPaint( ) override;
};

class DroppableButton : public Button
{
	DECLARE_CLASS_SIMPLE( DroppableButton, Button );

public:

	DroppableButton( Panel *pParent, const char *name, const char *text ) : Button( pParent, name, text ) { }
	bool IsDroppable( CUtlVector< KeyValues * >& msglist ) override;
	void OnPanelDropped(  CUtlVector< KeyValues * >& msglist ) override;
	void OnPanelEnteredDroppablePanel( CUtlVector< KeyValues * >& msglist );
	void OnPanelExitedDroppablePanel ( CUtlVector< KeyValues * >& msglist );
};

//-----------------------------------------------------------------------------
// Purpose: Inventory menu
//-----------------------------------------------------------------------------
class CClientMSInventoryMenu : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CClientMSInventoryMenu, vgui::Frame );

public:
	CClientMSInventoryMenu( IViewPort *pViewPort );
	virtual ~CClientMSInventoryMenu();

	virtual void OnCommand(const char *command);

	virtual const char *GetName( void ) { return PANEL_INVENTORY; }
	virtual void SetData(KeyValues *data) {};
	virtual void Reset();
	virtual void Update();
	virtual bool NeedsUpdate( void );
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );

	virtual void ShowBackContents();
	virtual void ShowSideContents();
	virtual void ShowBeltContents();
	virtual void ShowHandsContents();
	virtual void ShowEquipContents();
	virtual void CloseWindow();

	virtual	const char *GetIconFilePathNameForWeapon( const char *weaponName );

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
  	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

	void OnDraggedItem( Panel *pStartPanel, Panel *pEndPanel );

private:

	char lastMenuCommand[64]; //The last menu button they pressed (hands menu, backpack menu, etc. )
	float m_fNextUpdateTime;

	vgui::Label* title;
	vgui::Button* back;
	vgui::Button* side;
	vgui::Button* belt;
	vgui::Button* hands;
	vgui::Button* equip;
	vgui::Button* close;

	struct inv_slot_s		//Variables related to one inventory slot
	{
		DraggableImage *Icon;
		Button *pButton;
	};

	inv_slot_s m_BackPackSlots[MAX_BACKPACK_IMAGES]; //woohoo
	inv_slot_s m_SidePackSlots[MAX_SIDEPACK_IMAGES]; //yay for intelligence
	inv_slot_s m_BeltSlots[MAX_BELT_IMAGES]; //yay for more intelligence
	inv_slot_s m_HandSlots[MAX_HANDS_IMAGES]; //yay for more intelligence
	inv_slot_s m_EquipSlots[MAX_EQUIP_IMAGES]; //yay for more intelligence

	int m_DragSlotIdx;
	ImagePanel *m_DragDropIcon;

	int GetMouseOverSlot( );
	void OnCursorMoved( int x,int y ) override;
};


#endif // MS_INVENTORYMENU_H
