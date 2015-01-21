
#ifndef MSSINVENTORYMENU_H
#define MSSINVENTORYMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>
#include <game/client/iviewport.h>
#include "vgui_controls/ImagePanel.h"
#include "basemodelpanel.h"
#include "shareddefs.h"
#include "MSS_shareddefs.h"

using namespace vgui;

class CDroppableLabel;

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  BOXBOX DRAGGABLE IMAGE  ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

class CDragItem : public vgui::ImagePanel
{
public:
	DECLARE_CLASS_SIMPLE( CDragItem, ImagePanel );

	CDragItem( Panel *pParent, const char *name );

//	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual bool IsDroppable( CUtlVector< KeyValues * >& msglist ) { return true; }

	virtual void OnDraggablePanelPaint();
	virtual void OnDroppablePanelPaint( CUtlVector< KeyValues * >& msglist, CUtlVector< Panel * >& dragPanels );

	void OnPanelEnteredDroppablePanel( CUtlVector< KeyValues * >& msglist );
	void OnPanelExitedDroppablePanel ( CUtlVector< KeyValues * >& msglist );
	virtual void OnPanelDropped(  CUtlVector< KeyValues * >& msglist );

//	virtual void OnStartDragging();

//	virtual void OnCreateDragData( KeyValues *msg );
//	void OnMoveAway();

	ITEM_FILE_INFO_HANDLE	m_hItem; // BOXBOX Index into item held in slot
	const FileItemInfo_t	&GetItemData( ITEM_FILE_INFO_HANDLE item ) const;

//	int m_nItemType;

};

/*
class CDraggableImage : public vgui::ImagePanel
{
public:
	DECLARE_CLASS_SIMPLE( CDraggableImage, ImagePanel );

	CDraggableImage( Panel *pParent, const char *name );
//	virtual void OnStartDragging();
	virtual void OnDraggablePanelPaint();
//	virtual void OnCreateDragData( KeyValues *msg );
//	void OnMoveAway();

	int m_nItemType;
};
*/
//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  BOXBOX DROPPABLE LABEL  ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/*
class CDroppableLabel : public Label
{
	DECLARE_CLASS_SIMPLE( CDroppableLabel, Label );

public:

	CDroppableLabel( Panel *parent, const char *name );

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual bool IsDroppable( CUtlVector< KeyValues * >& msglist ) { return true; }
	virtual void OnDroppablePanelPaint( CUtlVector< KeyValues * >& msglist, CUtlVector< Panel * >& dragPanels );

	virtual void OnPanelDropped(  CUtlVector< KeyValues * >& msglist );
	void OnPanelEnteredDroppablePanel( CUtlVector< KeyValues * >& msglist );
	void OnPanelExitedDroppablePanel ( CUtlVector< KeyValues * >& msglist );

	int				GetItemType( void ) { return m_nItemType; }
	void			SetItemType( int type ) { m_nItemType = type; }
private:

	int m_nItemType;
};
*/
//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  BOXBOX INVENTORY MENU  ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

class CMSInventoryMenu : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CMSInventoryMenu, vgui::Frame );

//	Color					 m_bgColor;
//	Color					 m_borderColor;

public:
	CMSInventoryMenu(IViewPort *pViewPort);
	virtual ~CMSInventoryMenu();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual const char *GetName( void ) { return PANEL_INVENTORYMENU; }
	virtual void SetData(KeyValues *data) { return; }
	virtual void Reset();
	virtual void Update();
	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return false; }	// BOXBOX do I need to change this?
	virtual void ShowPanel( bool bShow );
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
  	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

//	void	NewItem( KeyValues itemData );
//	const FileItemInfo_t	&GetItemData( ITEM_FILE_INFO_HANDLE item ) const;

	CDragItem		*m_pBackpackItems[ BACKPACK_SLOTS_X ][ BACKPACK_SLOTS_Y ];
	CDragItem		*m_pBeltItems[ 10 ];

	CDragItem		*m_pLeftHandItem;
	CDragItem		*m_pRightHandItem;
	CDragItem		*m_pArmorItem;
	CDragItem		*m_pHelmetItem;
	CDragItem		*m_pGlovesItem;
	CDragItem		*m_pBootsItem;

/*
	CDraggableImage		*m_pBackpackItems[ BACKPACK_SLOTS_X ][ BACKPACK_SLOTS_Y ];
	CDraggableImage		*m_pBeltItems[ 10 ];

	CDraggableImage		*m_pLeftHandItem;
	CDraggableImage		*m_pRightHandItem;
	CDraggableImage		*m_pArmorItem;
	CDraggableImage		*m_pHelmetItem;
	CDraggableImage		*m_pGlovesItem;
	CDraggableImage		*m_pBootsItem;

	CDroppableLabel		*m_pBackpackDropSlots[ BACKPACK_SLOTS_X ][ BACKPACK_SLOTS_Y ];
	CDroppableLabel		*m_pBeltSlots[ 10 ]; // BOXBOX TODO Apply Quick Slot to these (1 through 0 keys on keyboard)

	CDroppableLabel		*m_pLeftHandSlot;
	CDroppableLabel		*m_pRightHandSlot;
	CDroppableLabel		*m_pArmorSlot;
	CDroppableLabel		*m_pHelmetSlot;
	CDroppableLabel		*m_pGlovesSlot;
	CDroppableLabel		*m_pBootsSlot;

	CDragItem			*m_pDragTest;
	CDragItem			*m_pDragTest2;
*/
protected:	

//	ITEM_FILE_INFO_HANDLE	m_hItemFileInfo;

	virtual void OnCommand( const char *command);

	void PlayPageTurnSound( void );

	IViewPort	*m_pViewPort;

	vgui::Label			*m_pRightPageTitleLabel;
	vgui::Label			*m_pBackpackLabel;
	vgui::Label			*m_pGoldLabel;

	CModelPanel			*m_pCharModel;
};





#endif // MSSINVENTORYMENU_H





/*
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
*/