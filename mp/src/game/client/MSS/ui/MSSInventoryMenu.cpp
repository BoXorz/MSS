
// BOXBOX The portion of the Main Menu, which will display player inventory

#include "cbase.h"
#include "MSSInventoryMenu.h"

#include "c_MSS_player.h"

#include "ienginevgui.h"

#include <networkstringtabledefs.h>
#include <cdll_client_int.h>
#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <FileSystem.h>
#include <KeyValues.h>
#include <convar.h>
#include <vgui_controls/ImageList.h>
#include <vgui_controls/TextEntry.h>
#include <vgui_controls/Button.h>
#include <game/client/iviewport.h>
#include <vgui/IInput.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;
extern INetworkStringTable *g_pStringTableInfoPanel;

//extern const char *pszGenderNames[];
//extern const char *pszRaceNames[];
//extern const char *pszSkillNames[];
//extern const char *pszSkillLevelNames[];
extern const char *pszPlayerModels[];
extern const char *pszBackpackNames[];

CMSInventoryMenu::CMSInventoryMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_INVENTORYMENU )
{
	m_pViewPort = pViewPort;

	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/MSScheme.res", "MSScheme");
	SetScheme( scheme );

	SetMoveable( false );
	SetSizeable( false );
	SetProportional( true );
	SetTitleBarVisible( false );
	SetKeyBoardInputEnabled( false );

	m_pRightPageTitleLabel = new Label( this, "RPageTitleLabel", "0" );
	m_pBackpackLabel = new Label( this, "BackpackLabel", "0" );
	m_pGoldLabel = new Label( this, "GoldLabel", "0" );

	for( int y = 0 ; y < BACKPACK_SLOTS_Y ; y++ )
	{
		for( int x = 0 ; x < BACKPACK_SLOTS_X ; x++ )
		{
			m_pBackpackItems[x][y] = new CDragItem( this, VarArgs( "BackpackItem %i %i", x, y ) );
		}
		m_pBeltItems[y] = new CDragItem( this, VarArgs( "BeltItem %i", y ) );
	}

	m_pLeftHandItem		= new CDragItem( this, "LeftHandItem" );
	m_pRightHandItem	= new CDragItem( this, "RightHandItem" );
	m_pArmorItem		= new CDragItem( this, "ArmorItem" );
	m_pHelmetItem		= new CDragItem( this, "HelmetItem" );
	m_pGlovesItem		= new CDragItem( this, "GlovesItem" );
	m_pBootsItem		= new CDragItem( this, "BootsItem" );

	m_pCharModel = new CModelPanel( this, "charmodel" );
}

void CMSInventoryMenu::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSInventoryMenu.res");

	int xpos = 279;
	int ypos = 159;

	for( int y = 0 ; y < BACKPACK_SLOTS_Y ; y++ )
	{
		for( int x = 0 ; x < BACKPACK_SLOTS_X ; x++ )
		{
			m_pBackpackItems[x][y]->SetDragEnabled( false );
			m_pBackpackItems[x][y]->SetPos( xpos + (x*65), ypos +(y*65) );
		}
		m_pBeltItems[y]->SetDragEnabled( false );
		m_pBeltItems[y]->SetPos( xpos + (y*65), 888 );
	}
	m_pLeftHandItem->SetDragEnabled( false );
	m_pLeftHandItem->SetPos( 1084, 500 );
	m_pLeftHandItem->m_nDropType = ITEMTYPE_WEAPON;

	m_pRightHandItem->SetDragEnabled( false );
	m_pRightHandItem->SetPos( 1600, 500 );
	m_pRightHandItem->m_nDropType = ITEMTYPE_WEAPON;

	m_pArmorItem->SetDragEnabled( false );
	m_pArmorItem->SetPos( 1345, 400 );
	m_pArmorItem->m_nDropType = ITEMTYPE_ARMOR;

	m_pHelmetItem->SetDragEnabled( false );
	m_pHelmetItem->SetPos( 1345, 180 );
	m_pHelmetItem->m_nDropType = ITEMTYPE_HELMET;

	m_pGlovesItem->SetDragEnabled( false );
	m_pGlovesItem->SetPos( 1140, 400 );
	m_pGlovesItem->m_nDropType = ITEMTYPE_GLOVES;

	m_pBootsItem->SetDragEnabled( false );
	m_pBootsItem->SetPos( 1345, 780 );
	m_pBootsItem->m_nDropType = ITEMTYPE_BOOTS;
}

CMSInventoryMenu::~CMSInventoryMenu()
{

}

void CMSInventoryMenu::Reset( void )
{

}

void CMSInventoryMenu::Update( void )
{
	C_MSS_Player *pPlayer = ToMSSPlayer( C_BasePlayer::GetLocalPlayer() );
	if( !pPlayer ) return;

	m_pRightPageTitleLabel->SetText( pPlayer->m_pszCharName );
	m_pBackpackLabel->SetText( pszBackpackNames[ pPlayer->m_nBackpackSize ] );

	char buf[10];
	itoa( pPlayer->m_nGold, buf, 10 );
	m_pGoldLabel->SetText( buf );

	for( int y = 0 ; y < BACKPACK_SLOTS_Y ; y++ )
	{
		for( int x = 0 ; x < BACKPACK_SLOTS_X ; x++ )
		{
			m_pBackpackItems[x][y]->SetVisible( pPlayer->m_nBackpackSize > y );

			if( !pPlayer->m_nBackpackItems[ y * 10 + x ] ) // BOXBOX no item in this slot
			{
				m_pBackpackItems[x][y]->m_hItem = GetInvalidItemInfoHandle();
				m_pBackpackItems[x][y]->SetImage( "icons/Blank" );
				m_pBackpackItems[x][y]->SetDragEnabled( false );
//				m_pBackpackItems[x][y]->m_nDropType = ITEMTYPE_ANY;
			}
			else
			{
				FileItemInfo_t *pItem = GetFileItemInfoFromHandle( (ITEM_FILE_INFO_HANDLE)pPlayer->m_nBackpackItems[ y * 10 + x  ] );
				if( !pItem ) return;

				m_pBackpackItems[x][y]->m_hItem = (ITEM_FILE_INFO_HANDLE)pPlayer->m_nBackpackItems[ y * 10 + x  ];
				m_pBackpackItems[x][y]->SetImage( pItem->szIconFileName );
				m_pBackpackItems[x][y]->SetDragEnabled( true );
				m_pBackpackItems[x][y]->m_nDropType = pItem->nItemType;
			}
		}

		if( !pPlayer->m_nBeltItems[ y ] ) // no item in this slot
		{
			m_pBeltItems[y]->m_hItem = GetInvalidItemInfoHandle();
			m_pBeltItems[y]->SetImage( "icons/Blank" );
			m_pBeltItems[y]->SetDragEnabled( false );
//			m_pBeltItems[y]->m_nDropType = ITEMTYPE_ANY;
		}
		else
		{
			FileItemInfo_t *pItem = GetFileItemInfoFromHandle( (ITEM_FILE_INFO_HANDLE)pPlayer->m_nBeltItems[ y ] );
			if( !pItem ) return;

			m_pBeltItems[y]->m_hItem = (ITEM_FILE_INFO_HANDLE)pPlayer->m_nBeltItems[ y ];
			m_pBeltItems[y]->SetImage( pItem->szIconFileName );
			m_pBeltItems[y]->SetDragEnabled( true );
			m_pBeltItems[y]->m_nDropType = pItem->nItemType;
		}
	}

	if( !pPlayer->m_nLeftHandItem )
	{
		m_pLeftHandItem->m_hItem = GetInvalidItemInfoHandle();
		m_pLeftHandItem->SetImage( "icons/Blank" );
		m_pLeftHandItem->SetDragEnabled( false );
	}
	else
	{
		FileItemInfo_t *pItem = GetFileItemInfoFromHandle( (ITEM_FILE_INFO_HANDLE)pPlayer->m_nLeftHandItem );
		if( !pItem ) return;

		m_pLeftHandItem->m_hItem = (ITEM_FILE_INFO_HANDLE)pPlayer->m_nLeftHandItem;
		m_pLeftHandItem->SetImage( pItem->szIconFileName );
		m_pLeftHandItem->SetDragEnabled( true );
	}

	if( !pPlayer->m_nRightHandItem )
	{
		m_pRightHandItem->m_hItem = GetInvalidItemInfoHandle();
		m_pRightHandItem->SetImage( "icons/Blank" );
		m_pRightHandItem->SetDragEnabled( false );
	}
	else
	{
		FileItemInfo_t *pItem = GetFileItemInfoFromHandle( (ITEM_FILE_INFO_HANDLE)pPlayer->m_nRightHandItem );
		if( !pItem ) return;

		m_pRightHandItem->m_hItem = (ITEM_FILE_INFO_HANDLE)pPlayer->m_nRightHandItem;
		m_pRightHandItem->SetImage( pItem->szIconFileName );
		m_pRightHandItem->SetDragEnabled( true );
	}

	if( !pPlayer->m_nArmorItem )
	{
		m_pArmorItem->m_hItem = GetInvalidItemInfoHandle();
		m_pArmorItem->SetImage( "icons/Blank" );
		m_pArmorItem->SetDragEnabled( false );
	}
	else
	{
		FileItemInfo_t *pItem = GetFileItemInfoFromHandle( (ITEM_FILE_INFO_HANDLE)pPlayer->m_nArmorItem );
		if( !pItem ) return;

		m_pArmorItem->m_hItem = (ITEM_FILE_INFO_HANDLE)pPlayer->m_nArmorItem;
		m_pArmorItem->SetImage( pItem->szIconFileName );
		m_pArmorItem->SetDragEnabled( true );
	}

	if( !pPlayer->m_nHelmetItem )
	{
		m_pHelmetItem->m_hItem = GetInvalidItemInfoHandle();
		m_pHelmetItem->SetImage( "icons/Blank" );
		m_pHelmetItem->SetDragEnabled( false );
	}
	else
	{
		FileItemInfo_t *pItem = GetFileItemInfoFromHandle( (ITEM_FILE_INFO_HANDLE)pPlayer->m_nHelmetItem );
		if( !pItem ) return;

		m_pHelmetItem->m_hItem = (ITEM_FILE_INFO_HANDLE)pPlayer->m_nHelmetItem;
		m_pHelmetItem->SetImage( pItem->szIconFileName );
		m_pHelmetItem->SetDragEnabled( true );
	}

	if( !pPlayer->m_nGlovesItem )
	{
		m_pGlovesItem->m_hItem = GetInvalidItemInfoHandle();
		m_pGlovesItem->SetImage( "icons/Blank" );
		m_pGlovesItem->SetDragEnabled( false );
	}
	else
	{
		FileItemInfo_t *pItem = GetFileItemInfoFromHandle( (ITEM_FILE_INFO_HANDLE)pPlayer->m_nGlovesItem );
		if( !pItem ) return;

		m_pGlovesItem->m_hItem = (ITEM_FILE_INFO_HANDLE)pPlayer->m_nGlovesItem;
		m_pGlovesItem->SetImage( pItem->szIconFileName );
		m_pGlovesItem->SetDragEnabled( true );
	}

	if( !pPlayer->m_nBootsItem )
	{
		m_pBootsItem->m_hItem = GetInvalidItemInfoHandle();
		m_pBootsItem->SetImage( "icons/Blank" );
		m_pBootsItem->SetDragEnabled( false );
	}
	else
	{
		FileItemInfo_t *pItem = GetFileItemInfoFromHandle( (ITEM_FILE_INFO_HANDLE)pPlayer->m_nBootsItem );
		if( !pItem ) return;

		m_pBootsItem->m_hItem = (ITEM_FILE_INFO_HANDLE)pPlayer->m_nBootsItem;
		m_pBootsItem->SetImage( pItem->szIconFileName );
		m_pBootsItem->SetDragEnabled( true );
	}

	m_pCharModel->SwapModel( pszPlayerModels[ pPlayer->m_nPlayerModelIndex ], NULL ); 
	m_pCharModel->MoveToFront();

}


void CMSInventoryMenu::OnCommand( const char *command )
{
    if (!Q_strcmp(command, "TurnPageLeft"))
    {
		PlayPageTurnSound();
		m_pViewPort->ShowPanel( this, false );
		m_pViewPort->ShowPanel( PANEL_STATSMENU, true );
		engine->ClientCmd( "turnpage 1" );
	}
    else if (!Q_strcmp(command, "TurnPageRight"))
    {
		PlayPageTurnSound();
		m_pViewPort->ShowPanel( this, false );
		m_pViewPort->ShowPanel( PANEL_POWERUPMENU, true );
		engine->ClientCmd( "turnpage 3" );
	}

	BaseClass::OnCommand(command);
}

void CMSInventoryMenu::PlayPageTurnSound( void )
{
	int rnd = RandomInt( 0, 1 );
	if( rnd )
		vgui::surface()->PlaySound( "UI/pageturn2.wav" );
	else
		vgui::surface()->PlaySound( "UI/pageturn3.wav" );
}

void CMSInventoryMenu::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetVisible( true );
	}
	else
	{
		SetVisible( false );
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  BOXBOX DRAGGABLE IMAGE  ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

CDragItem::CDragItem( Panel *pParent, const char *name ) : ImagePanel( pParent, name )
{
	SetDragEnabled( true );
	SetDropEnabled( true );
	SetVisible( true );
	SetZPos( 2 );
	SetSize( 64, 64 );
//	SetImage( "icons/Blank" );
	m_nDropType = ITEMTYPE_ANY;
}

const FileItemInfo_t &CDragItem::GetItemData( ITEM_FILE_INFO_HANDLE item ) const
{
	return *GetFileItemInfoFromHandle( m_hItem );
}

// BOXBOX this is for when a drag/drop item is being dragged, but is not over another drag/drop item
void CDragItem::OnDraggablePanelPaint( )
{
	IImage *pImage = GetImage();
	if( pImage )
	{
		int x = 0, y = 0;
		input()->GetCursorPos(x, y);

		int w = 0, h = 0;
		pImage->GetSize( w, h );

		pImage->SetPos(x - (w * 0.5f), y - (h * 0.5f));

		pImage->Paint();
	}
}

// BOXBOX this is for when a drag/drop item is being dragged, and IS over another drag/drop item
void CDragItem::OnDroppablePanelPaint( CUtlVector< KeyValues * >& msglist, CUtlVector< Panel * >& dragPanels )
{
	Panel *pDrag = (Panel *)msglist.Head()->GetFirstSubKey()->GetPtr(); // BOXBOX get pointer to dragged image!
	if( !pDrag ) return;

	CDragItem *pItem = (CDragItem*)pDrag;

	if( ( m_nDropType == ITEMTYPE_ANY ) || ( m_nDropType == pItem->m_nDropType ) )
	{
		// BOXBOX Draw an outline around me(the drop item)
		int w, h;
		GetSize( w, h );

		int x, y;
		x = y = 0;
		LocalToScreen( x, y );

		surface()->DrawSetColor( Color(255, 255, 255, 255) );
		surface()->DrawOutlinedRect( x-1, y-1, x + w+1, y + h+1 );
		surface()->DrawOutlinedRect( x+1, y+1, x + w-1, y + h-1 );

	}

// BOXBOX now get the dragged item and draw it normally
	IImage *pImage = pItem->GetImage();
	if( pImage )
	{
		int x = 0, y = 0;
		input()->GetCursorPos(x, y);

		int w = 0, h = 0;
		pImage->GetSize( w, h );
		pImage->SetPos(x - (w * 0.5f), y - (h * 0.5f));
		pImage->Paint();
	}
}

void CDragItem::OnPanelEnteredDroppablePanel( CUtlVector< KeyValues * >& msglist )
{
	if( m_nDropType == ITEMTYPE_ANY ) return; // BOXBOX any type of item can be dropped here (backpack, belt)

	Panel *pDrag = (Panel *)msglist.Head()->GetFirstSubKey()->GetPtr(); // BOXBOX get pointer to dragged image!
	if( !pDrag ) return;

	CDragItem *pItem = (CDragItem*)pDrag;
//	CDragItem *pItem = dynamic_cast<CDragItem *>(pDrag); // BOXBOX cast it to our draggable class
//	if( !pItem ) return;

//	Warning("ITEM TYPE %i DRAGGED OVER ITEM TYPE %i\n", pItem->m_nDropType, m_nDropType );

	if( m_nDropType != pItem->m_nDropType )
	{
		SetDropEnabled( false );
	}
}

void CDragItem::OnPanelExitedDroppablePanel( CUtlVector< KeyValues * >& msglist )
{
	SetDropEnabled( true );
}

void CDragItem::OnPanelDropped( CUtlVector< KeyValues * >& msglist ) // BOXBOX an item has been dropped on me!
{
	C_MSS_Player *pPlayer = ToMSSPlayer( C_BasePlayer::GetLocalPlayer() );
	if( !pPlayer ) return;

	Panel *pDrag = (Panel *)msglist.Head()->GetFirstSubKey()->GetPtr(); // BOXBOX get pointer to dragged image!
	if( !pDrag ) return;

	CDragItem *pItem = (CDragItem*)pDrag;
//	CDragItem *pItem = dynamic_cast<CDragItem *>(pDrag);
//	if( !pItem ) return;

	if( m_nDropType != ITEMTYPE_ANY )
	{
		if( m_nDropType != pItem->m_nDropType )
		{
			SetDropEnabled( true );
			return;
		}
	}

	CMSInventoryMenu *pMenu = (CMSInventoryMenu *)GetParent( );
	if( !pMenu ) return;

	SetDragEnabled( true );
	SetImage( pItem->GetImage() );
	pItem->SetDragEnabled( false );
	pItem->SetImage( "icons/Blank" ); // BOXBOX why does this only work once?

// BOXBOX Find the from slot and the to slot, and adjust inventory
	int nFrom = -1;
	int nTo = -1;

	for( int y = 0 ; y < BACKPACK_SLOTS_Y ; y++ )
	{
		for( int x = 0 ; x < BACKPACK_SLOTS_X ; x++ )
		{
			if( pMenu->m_pBackpackItems[ x ][ y ] == this ) // BOXBOX the filled backpack slot
			{
				nTo = y * 10 + x;
				m_hItem = pItem->m_hItem;
				pPlayer->m_nBackpackItems[ nTo ] = m_hItem;
				m_nDropType = pItem->m_nDropType;
			}
		}

		if( pMenu->m_pBeltItems[ y ] == this )
		{
			nTo = y + 100;
			m_hItem = pItem->m_hItem;
			pPlayer->m_nBeltItems[ y ] = m_hItem;
			m_nDropType = pItem->m_nDropType;
		}
	}

	if( pMenu->m_pLeftHandItem == this )
	{
		nTo = 110;
		m_hItem = pItem->m_hItem;
		pPlayer->m_nLeftHandItem = m_hItem;
	}
	if( pMenu->m_pRightHandItem == this )
	{
		nTo = 111;
		m_hItem = pItem->m_hItem;
		pPlayer->m_nRightHandItem = m_hItem;
	}
	if( pMenu->m_pArmorItem == this )
	{
		nTo = 112;
		m_hItem = pItem->m_hItem;
		pPlayer->m_nArmorItem = m_hItem;
	}
	if( pMenu->m_pHelmetItem == this )
	{
		nTo = 113;
		m_hItem = pItem->m_hItem;
		pPlayer->m_nHelmetItem = m_hItem;
	}
	if( pMenu->m_pGlovesItem == this )
	{
		nTo = 114;
		m_hItem = pItem->m_hItem;
		pPlayer->m_nGlovesItem = m_hItem;
	}
	if( pMenu->m_pBootsItem == this )
	{
		nTo = 115;
		m_hItem = pItem->m_hItem;
		pPlayer->m_nBootsItem = m_hItem;
	}

	for( int y = 0 ; y < BACKPACK_SLOTS_Y ; y++ )
	{
		for( int x = 0 ; x < BACKPACK_SLOTS_X ; x++ )
		{
			if( pMenu->m_pBackpackItems[ x ][ y ] == pItem ) // BOXBOX the abandoned backpack slot
			{
				nFrom = y * 10 + x;
				m_hItem = GetInvalidItemInfoHandle();
				pPlayer->m_nBackpackItems[ nFrom ] = 0;
				pItem->m_nDropType = ITEMTYPE_ANY;
			}
		}

		if( pMenu->m_pBeltItems[ y ] == pItem )
		{
			nFrom = y + 100;
			m_hItem = GetInvalidItemInfoHandle();
			pPlayer->m_nBeltItems[ y ] = 0;
			pItem->m_nDropType = ITEMTYPE_ANY;
		}
	}

	if( pMenu->m_pLeftHandItem == pItem )
		{
			nFrom = 110;
			m_hItem = GetInvalidItemInfoHandle();
			pPlayer->m_nLeftHandItem = 0;
		}
	if( pMenu->m_pRightHandItem == pItem )
		{
			nFrom = 111;
			m_hItem = GetInvalidItemInfoHandle();
			pPlayer->m_nRightHandItem = 0;
		}
	if( pMenu->m_pArmorItem == pItem )
		{
			nFrom = 112;
			m_hItem = GetInvalidItemInfoHandle();
			pPlayer->m_nArmorItem = 0;
		}
	if( pMenu->m_pHelmetItem == pItem )
		{
			nFrom = 113;
			m_hItem = GetInvalidItemInfoHandle();
			pPlayer->m_nHelmetItem = 0;
		}
	if( pMenu->m_pGlovesItem == pItem )
		{
			nFrom = 114;
			m_hItem = GetInvalidItemInfoHandle();
			pPlayer->m_nGlovesItem = 0;
		}
	if( pMenu->m_pBootsItem == pItem )
		{
			nFrom = 115;
			m_hItem = GetInvalidItemInfoHandle();
			pPlayer->m_nBootsItem = 0;
		}

// BOXBOX Finally, tell the server about the move and update the Inventory Menu
	engine->ClientCmd( VarArgs("moveitem %i %i", nFrom, nTo ) );

	pMenu->Update();
	DrawItemTypeHalo();
}

void CDragItem::DrawItemTypeHalo( void )
{

		int w, h;
		GetSize( w, h );

		int x, y;
		x = y = 0;
		LocalToScreen( x, y );

		Warning( "DRAWITEMTYPEHALO W: %i  H: %i  X: %i  Y: %i\n", w, h, x, y );
//		surface()-> // BOXBOX TODO why this not working?

		vgui::surface()->DrawSetColor( Color(255, 50, 50, 200) );
		vgui::surface()->DrawOutlinedRect( x-1, y-1, x + w+1, y + h+1 );

		vgui::surface()->DrawSetColor( Color(255, 50, 50, 150) );
		vgui::surface()->DrawOutlinedRect( x+1, y+1, x + w-1, y + h-1 );

		vgui::surface()->DrawSetColor( Color(255, 50, 50, 100) );
		vgui::surface()->DrawOutlinedRect( x+2, y+2, x + w-2, y + h-2 );

		vgui::surface()->DrawSetColor( Color(255, 50, 50, 50) );
		vgui::surface()->DrawOutlinedRect( x+3, y+3, x + w-3, y + h-3 );
}





// BOXBOX OLD CODE BELOW




/*
CDraggableImage::CDraggableImage( Panel *pParent, const char *name ) : ImagePanel( pParent, name )
{
	SetDragEnabled( true );
	SetZPos( 3 );
	SetSize( 64, 64 );
	m_nItemType = ITEMTYPE_ANY;
//	m_pPreviousSlot = NULL;
}
*/

/*
void CDraggableImage::OnStartDragging() // BOXBOX this is BS I gotta do all this...
{
//	IViewPortPanel *panel = gViewPortInterface->FindPanelByName( PANEL_INVENTORYMENU );

	Panel *panel = GetParent();
	if( !panel ) return;

	for (int i = 0; i < panel->GetChildCount(); i++)
	{
		CDroppableLabel *child = dynamic_cast<CDroppableLabel *>(panel->GetChild(i));
		if( child )
			child->SetZPos( 4 );
	}

	BaseClass::OnStartDragging();
}
*/
/*
void CDraggableImage::OnDraggablePanelPaint( )
{
	IImage *pImage = GetImage();
	if( pImage )
	{
		int x = 0, y = 0;
		input()->GetCursorPos(x, y);

		int w = 0, h = 0;
		pImage->GetSize( w, h );

		pImage->SetPos(x - (w * 0.5f), y - (h * 0.5f));

		pImage->Paint();
	}
}
*/
/*
void CDraggableImage::OnCreateDragData( KeyValues *msg )
{
	Warning("*** CREATING DRAG DATA ***\n" );

	BaseClass::OnCreateDragData( msg );
}
*/

/*
void CDraggableImage::OnMoveAway()
{
	m_pPreviousSlot->SetItem( NULL );
}
*/



//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  BOXBOX DROPPABLE LABEL  ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/*
CDroppableLabel::CDroppableLabel( Panel *parent, const char *name ) : Label( parent, name, "" )
{
	SetVisible( true );
	SetDropEnabled( true );
	SetZPos( 2 );
	SetSize( 66, 66 );
	SetBgColor( Color( 0, 0, 0, 255 ) );

//	m_pItem = NULL;
	m_nItemType = ITEMTYPE_ANY;
}

void CDroppableLabel::ApplySchemeSettings( IScheme *pScheme )
{
	SetBorder(pScheme->GetBorder("MSBorder"));
}

void CDroppableLabel::OnDroppablePanelPaint( CUtlVector< KeyValues * >& msglist, CUtlVector< Panel * >& dragPanels ) // BOXBOX damn this actually works
{
	Panel *pDrag = (Panel *)msglist.Head()->GetFirstSubKey()->GetPtr(); // BOXBOX get pointer to dragged image!
	if( !pDrag ) return;

	CDraggableImage	*pItem = (CDraggableImage*)pDrag;

	IImage *pImage = pItem->GetImage();
	if( pImage )
	{
		int x = 0, y = 0;
		input()->GetCursorPos(x, y);

		int w = 0, h = 0;
		pImage->GetSize( w, h );

		pImage->SetPos(x - (w * 0.5f), y - (h * 0.5f));

		pImage->Paint();
	}
}
*/

/*
void CDroppableLabel::OnPanelEnteredDroppablePanel( CUtlVector< KeyValues * >& msglist )
{
	SetZPos( 4 );
	
	//	Warning("*** PANEL ENTERED %s ***\n", GetName() );

	if( HasItem() )
	{
		Warning("*** HAS ITEM ***\n" );
	}
	else
	{
		Warning("*** NO ITEM ***\n" );
	}



	if( m_nItemType == ITEMTYPE_ANY ) return; // BOXBOX any type of item can be dropped here (backpack, belt)

	Panel *pDrag = (Panel *)msglist.Head()->GetFirstSubKey()->GetPtr(); // BOXBOX get pointer to dragged image!
	if( !pDrag ) return;

	CDraggableImage *pItem = dynamic_cast<CDraggableImage *>(pDrag); // BOXBOX cast it to our draggable class
	if( !pItem ) return;

	if( m_nItemType != pItem->m_nItemType )
	{
		SetDropEnabled( false );
	}
	else
	{
		SetBgColor( Color( 0, 0, 0, 150 ) );
	}
}

void CDroppableLabel::OnPanelExitedDroppablePanel( CUtlVector< KeyValues * >& msglist )
{
	SetBgColor( Color( 0, 0, 0, 255 ) );
	SetDropEnabled( true );
	
	SetZPos( 2 );
	
	//	Warning("*** PANEL EXITED %s ***\n", GetName() );
}
*/

/*
void CDroppableLabel::OnPanelDropped( CUtlVector< KeyValues * >& msglist ) // BOXBOX an item has been dropped on me!
{

	SetZPos( 2 );

	SetBgColor( Color( 0, 0, 0, 255 ) );
	Panel *pDrag = (Panel *)msglist.Head()->GetFirstSubKey()->GetPtr(); // BOXBOX get pointer to dragged image!
	if( !pDrag ) return;

	CDraggableImage *pItem = dynamic_cast<CDraggableImage *>(pDrag);
	if( !pItem ) return;

	if( m_nItemType != ITEMTYPE_ANY )
	{
		if( m_nItemType != pItem->m_nItemType )
			return;
	}

	pDrag->SetVisible( false );

	CMSInventoryMenu *pMenu = (CMSInventoryMenu *)GetParent( );
	if( !pMenu ) return;

// BOXBOX Find the drop slot we were dragged from, to, and change icons
	int nFrom = -1;
	int nTo = -1;

	for( int y = 0 ; y < BACKPACK_SLOTS_Y ; y++ )
	{
		for( int x = 0 ; x < BACKPACK_SLOTS_X ; x++ )
		{
			if( pMenu->m_pBackpackItems[ x ][ y ] == pItem )
			{
				nFrom = y * 10 + x;
			}

			if( pMenu->m_pBackpackDropSlots[ x ][ y ] == this )
			{
				pMenu->m_pBackpackItems[ x ][ y ]->SetImage( pItem->GetImage() );
				pMenu->m_pBackpackItems[ x ][ y ]->SetVisible( true );
				nTo = y * 10 + x;
			}
		}

		if( pMenu->m_pBeltItems[ y ] == pItem )
		{
			nFrom = y + 100;
		}

		if( pMenu->m_pBeltSlots[ y ] == this )
		{
			pMenu->m_pBeltItems[ y ]->SetImage( pItem->GetImage() );
			pMenu->m_pBeltItems[ y ]->SetVisible( true );
			nTo = y + 100;
		}
	}

	if( pMenu->m_pLeftHandItem == pItem ) nFrom = 110;
	if( pMenu->m_pRightHandItem == pItem ) nFrom = 111;
	if( pMenu->m_pArmorItem == pItem ) nFrom = 112;
	if( pMenu->m_pHelmetItem == pItem ) nFrom = 113;
	if( pMenu->m_pGlovesItem == pItem ) nFrom = 114;
	if( pMenu->m_pBootsItem == pItem ) nFrom = 115;

	if( pMenu->m_pLeftHandSlot == this )
	{
		pMenu->m_pLeftHandItem->SetImage( pItem->GetImage() );
		pMenu->m_pLeftHandItem->SetVisible( true );
		nTo = 110;
	}
	if( pMenu->m_pRightHandSlot == this )
	{
		pMenu->m_pRightHandItem->SetImage( pItem->GetImage() );
		pMenu->m_pRightHandItem->SetVisible( true );
		nTo = 111;
	}
	if( pMenu->m_pArmorSlot == this )
	{
		pMenu->m_pArmorItem->SetImage( pItem->GetImage() );
		pMenu->m_pArmorItem->SetVisible( true );
		nTo = 112;
	}
	if( pMenu->m_pHelmetSlot == this )
	{
		pMenu->m_pHelmetItem->SetImage( pItem->GetImage() );
		pMenu->m_pHelmetItem->SetVisible( true );
		nTo = 113;
	}
	if( pMenu->m_pGlovesSlot == this )
	{
		pMenu->m_pGlovesItem->SetImage( pItem->GetImage() );
		pMenu->m_pGlovesItem->SetVisible( true );
		nTo = 114;
	}
	if( pMenu->m_pBootsSlot == this )
	{
		pMenu->m_pBootsItem->SetImage( pItem->GetImage() );
		pMenu->m_pBootsItem->SetVisible( true );
		nTo = 115;
	}

// BOXBOX whew! Finally, tell the server about the move so the player's inventory can be updated.
	engine->ClientCmd( VarArgs("moveitem %i %i", nFrom, nTo ) );

//	m_pItem = pItem; // BOXBOX now set the item in this one
//	m_pItem->m_pPreviousSlot = this;

//	int x, y;
//	GetPos(x, y);
//	m_pItem->SetPos(x+1, y+1);

//	Warning("*** DROPPED %s ON %s ***\n", pDrag->GetName(), GetName() );

// BOXBOX use this code below if we want to access the Inventory Menu
//	CMSInventoryMenu *pParentMenu = (CMSInventoryMenu *)GetParent( );
//	pParentMenu->OnDraggedItem( pDragPanel, this );

}
*/



/*
void CDroppableLabel::SetItem( CDraggableImage *item )
{
	m_pItem = item;
	int x, y;
	GetPos(x, y);
	m_pItem->SetPos(x, y);
}
*/

























/*
#include "cbase.h"
#include <stdio.h>
#include <string>

#include "MSSInventoryMenu.h"
#include "filesystem.h"
#include "fmtstr.h"

using namespace vgui;
using namespace std;

#include <vgui_controls/Label.h>
#include <vgui/ISurface.h>
#include <vgui/IInput.h>

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CClientMSInventoryMenu::CClientMSInventoryMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_INVENTORY )
{
	Q_strcmp(lastMenuCommand, "none" );

	// initialize dialog
	SetProportional(true);
	SetKeyBoardInputEnabled(false);
	SetSize(650, 500);
	SetSizeable(false);
	SetMoveable(false);

	// hide the system buttons
	SetTitleBarVisible( false );

	title = new vgui::Label(this, "title", "");
	back = new DroppableButton(this, "back", "Backpack" );
	side = new DroppableButton(this, "side", "Sidepack" );
	belt = new DroppableButton(this, "belt", "Beltpack" );
	hands = new DroppableButton(this, "hands", "Hands" );
	equip = new DroppableButton(this, "equip", "Equipment" );
	close = new vgui::Button(this, "close", "Close", 0, 0);

	for ( int i = 0; i < ARRAYSIZE(m_BackPackSlots); i++ )
	{
		inv_slot_s &slot = m_BackPackSlots[i];
		slot.pButton = new DroppableButton(this, CFmtStrN<32> ( "backpackInventory%i", (i+1) ), "" );
		slot.pButton->SetVisible( false );
		//slot.pButton->SetReflectMouse( true );
	}
	for ( int i = 0; i < ARRAYSIZE(m_SidePackSlots); i++ )
	{
		inv_slot_s &slot = m_SidePackSlots[i];
		slot.pButton = new DroppableButton(this, CFmtStrN<32> ( "sidepackInventory%i", (i+1) ), "" );
		slot.pButton->SetVisible( false );
		//slot.pButton->SetReflectMouse( true );
	}
	for ( int i = 0; i < ARRAYSIZE(m_BeltSlots); i++ )
	{
		inv_slot_s &slot = m_BeltSlots[i];
		slot.pButton = new DroppableButton(this, CFmtStrN<32> ( "beltInventory%i", (i+1) ), "" );
		slot.pButton->SetVisible( false );
		//slot.pButton->SetReflectMouse( true );
	}
	for ( int i = 0; i < ARRAYSIZE(m_HandSlots); i++ )
	{
		inv_slot_s &slot = m_HandSlots[i];
		slot.pButton = new DroppableButton(this, CFmtStrN<32> ( "handsInventory%i", (i+1) ), "" );
		slot.pButton->SetVisible( false );
		//slot.pButton->SetReflectMouse( true );
	}
	for ( int i = 0; i < ARRAYSIZE(m_EquipSlots); i++ )
	{
		inv_slot_s &slot = m_EquipSlots[i];
		slot.pButton = new DroppableButton(this, CFmtStrN<32> ( "equipInventory%i", (i+1) ), "" );
		slot.pButton->SetVisible( false );
		//slot.pButton->SetReflectMouse( true );
	}

	char backpackImageName[] = "backpackInventoryImage";
	char sidepackImageName[] = "sidepackInventoryImage";
	char beltImageName[] = "beltInventoryImage";
	char handsImageName[] = "handsInventoryImage";
	char equipImageName[] = "equipInventoryImage";

	char imageNameHolder[32];
	char number[3];

	for ( int i = 0; i < MAX_BACKPACK_IMAGES; i++ )
	{
		Q_strcpy( imageNameHolder, backpackImageName );
		itoa( i + 1, number, 10 );
		Q_strcat( imageNameHolder, number, 5 );
		
		DraggableImage &newIcon = *( m_BackPackSlots[i].Icon = new DraggableImage(this, imageNameHolder) );
		newIcon.SetShouldScaleImage(true);
		newIcon.SetSize(32,32);
		newIcon.SetImage( "test" ); //Remove later
		newIcon.SetVisible(false);
	
		if ( i == 0 ) newIcon.SetPos(175,50);
		else if ( i == 1 ) newIcon.SetPos(207,50);
		else if ( i == 2 ) newIcon.SetPos(239,50);
		else if ( i == 3 ) newIcon.SetPos(271,50);
		else if ( i == 4 ) newIcon.SetPos(303,50);
		else if ( i == 5 ) newIcon.SetPos(175,82);
		else if ( i == 6 ) newIcon.SetPos(207,82);
		else if ( i == 7 ) newIcon.SetPos(239,82);
		else if ( i == 8 ) newIcon.SetPos(271,82);
		else if ( i == 9 ) newIcon.SetPos(303,82);
		else if ( i == 10 ) newIcon.SetPos(175,114);
		else if ( i == 11 ) newIcon.SetPos(207,114);
		else if ( i == 12 ) newIcon.SetPos(239,114);
		else if ( i == 13 ) newIcon.SetPos(271,114);
		else if ( i == 14 ) newIcon.SetPos(303,114);
		else if ( i == 15 ) newIcon.SetPos(175,146);
		else if ( i == 16 ) newIcon.SetPos(207,146);
		else if ( i == 17 ) newIcon.SetPos(239,146);
		else if ( i == 18 ) newIcon.SetPos(271,146);
		else if ( i == 19 ) newIcon.SetPos(303,146);

		newIcon.SetDragEnabled( true );
//		newIcon.SetReflectMouse( true );
	}
	


	for ( int i = 0; i < MAX_SIDEPACK_IMAGES; i++ )
	{
		Q_strcpy( imageNameHolder, sidepackImageName );
		itoa( i + 1, number, 10 );
		Q_strcat( imageNameHolder, number, 5 );
		
		DraggableImage &newIcon = *( m_SidePackSlots[i].Icon = new DraggableImage(this, imageNameHolder) );
		newIcon.SetShouldScaleImage(true);
		newIcon.SetSize(32,32);
		newIcon.SetImage( "test" ); //Remove later
		newIcon.SetVisible(false);
	
		if ( i == 0 ) newIcon.SetPos(175,50);
		else if ( i == 1 ) newIcon.SetPos(207,50);
		else if ( i == 2 ) newIcon.SetPos(239,50);
		else if ( i == 3 ) newIcon.SetPos(271,50);
		else if ( i == 4 ) newIcon.SetPos(303,50);
		else if ( i == 5 ) newIcon.SetPos(175,82);
		else if ( i == 6 ) newIcon.SetPos(207,82);
		else if ( i == 7 ) newIcon.SetPos(239,82);
		else if ( i == 8 ) newIcon.SetPos(271,82);
		else if ( i == 9 ) newIcon.SetPos(303,82);
		else if ( i == 10 ) newIcon.SetPos(175,114);
		else if ( i == 11 ) newIcon.SetPos(207,114);
		else if ( i == 12 ) newIcon.SetPos(239,114);
		else if ( i == 13 ) newIcon.SetPos(271,114);
		else if ( i == 14 ) newIcon.SetPos(303,114);

		newIcon.SetDragEnabled( true );
//		newIcon.SetReflectMouse( true );
	}

	for ( int i = 0; i < MAX_BELT_IMAGES; i++ )
	{
		Q_strcpy( imageNameHolder, beltImageName );
		itoa( i + 1, number, 10 );
		Q_strcat( imageNameHolder, number, 5 );
		
		DraggableImage &newIcon = *( m_BeltSlots[i].Icon = new DraggableImage(this, imageNameHolder) );
		newIcon.SetShouldScaleImage(false); //true
		newIcon.SetSize(32,32);
		newIcon.SetImage( "test" ); //Remove later
		newIcon.SetVisible(false);
	
		if ( i == 0 ) newIcon.SetPos(175,50);
		else if ( i == 1 ) newIcon.SetPos(207,50);
		else if ( i == 2 ) newIcon.SetPos(239,50);
		else if ( i == 3 ) newIcon.SetPos(271,50);
		else if ( i == 4 ) newIcon.SetPos(303,50);
		else if ( i == 5 ) newIcon.SetPos(175,82);
		else if ( i == 6 ) newIcon.SetPos(207,82);
		else if ( i == 7 ) newIcon.SetPos(239,82);
		else if ( i == 8 ) newIcon.SetPos(271,82);
		else if ( i == 9 ) newIcon.SetPos(303,82);

		newIcon.SetDragEnabled( true );
//		newIcon.SetReflectMouse( true );
	}


	for ( int i = 0; i < MAX_HANDS_IMAGES; i++ )
	{
		Q_strcpy( imageNameHolder, handsImageName );
		itoa( i + 1, number, 10 );
		Q_strcat( imageNameHolder, number, 5 );
		
		DraggableImage &newIcon = *( m_HandSlots[i].Icon = new DraggableImage(this, imageNameHolder) );
		newIcon.SetShouldScaleImage(true);
		newIcon.SetSize(32,32);
		newIcon.SetImage( "test" ); //Remove later
		newIcon.SetVisible(false);
		newIcon.SetPos(175 + 32 * i,50);
		newIcon.SetDragEnabled( true );
//		newIcon.SetReflectMouse( true );
	}

	for ( int i = 0; i < MAX_EQUIP_IMAGES; i++ )
	{
		Q_strcpy( imageNameHolder, equipImageName );
		itoa( i + 1, number, 10 );
		Q_strcat( imageNameHolder, number, 5 );
		
		DraggableImage &newIcon = *( m_EquipSlots[i].Icon = new DraggableImage(this, imageNameHolder) );
		newIcon.SetShouldScaleImage(true);
		newIcon.SetSize(32,32);
		newIcon.SetImage( "test" ); //Remove later
		newIcon.SetVisible(false);
		newIcon.SetPos(175 + 32 * i,50);
		newIcon.SetDragEnabled( true );
//		newIcon.SetReflectMouse( true );
	}

	m_DragDropIcon = new ImagePanel( this, "equipInventoryImage" );
	m_DragDropIcon->SetVisible( false );
//	m_DragDropIcon->SetReflectMouse( true );
	m_DragSlotIdx = -1;

	// set the scheme before any child control is created
	SetScheme("ClientScheme");

	back->SetDropEnabled(true);
	side->SetDropEnabled(true);
	belt->SetDropEnabled(true);
	hands->SetDropEnabled(true);
	equip->SetDropEnabled(true);

	back->Repaint();
	side->Repaint();
	belt->Repaint();
	hands->Repaint();
	equip->Repaint();

}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CClientMSInventoryMenu::~CClientMSInventoryMenu()
{
	
}

//-----------------------------------------------------------------------------
// Purpose: Clears the inventory menu and rebuilds it
//-----------------------------------------------------------------------------
void CClientMSInventoryMenu::Reset()
{
	// Sets up fonts and colors used
	vgui::IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme());
	HFont titleFont = pScheme->GetFont("MSInvenTitle");
	HFont buttonFont = pScheme->GetFont("MSInvenButton");
	vgui::IBorder *border = pScheme->GetBorder("MSStatsBorder");
	vgui::IBorder *buttonBorder = pScheme->GetBorder("MSButtonBorder");
	SetFgColor(Color(255, 255, 255, 255));
	Color* white = new Color(255, 255, 255, 255);
	Color* greenTrans = new Color(96, 224, 64, 56);
	Color* greenTrans2 = new Color(96, 224, 64, 128);
	Color* black = new Color(0, 0, 0, 76);
	
	title->SetPos(20, 20);
	title->SetSize(100, 20);
	title->SetFont(titleFont);
	title->SetFgColor(*white);

	back->SetPos(20, 50);
	back->SetSize(100, 20);
	back->SetCommand("showBack");
	back->SetFont(buttonFont);
	back->SetArmedColor(*white, *greenTrans);
	back->SetDepressedColor(*white, *greenTrans2);
	back->SetDepressedBorder(buttonBorder);
	back->SetDefaultColor(*white, *black);
	back->SetDefaultBorder(buttonBorder);
	back->SetBorder(buttonBorder);
	back->SetKeyFocusBorder(buttonBorder);
	back->Repaint();

	side->SetPos(20, 80);
	side->SetSize(100, 20);
	side->SetCommand("showSide");
	side->SetFont(buttonFont);
	side->SetArmedColor(*white, *greenTrans);
	side->SetDepressedColor(*white, *greenTrans2);
	side->SetDepressedBorder(buttonBorder);
	side->SetDefaultColor(*white, *black);
	side->SetDefaultBorder(buttonBorder);
	side->SetBorder(buttonBorder);
	side->SetKeyFocusBorder(buttonBorder);
	side->Repaint();

	belt->SetPos(20, 110);
	belt->SetSize(100, 20);
	belt->SetCommand("showBelt");
	belt->SetFont(buttonFont);
	belt->SetArmedColor(*white, *greenTrans);
	belt->SetDepressedColor(*white, *greenTrans2);
	belt->SetDepressedBorder(buttonBorder);
	belt->SetDefaultColor(*white, *black);
	belt->SetDefaultBorder(buttonBorder);
	belt->SetBorder(buttonBorder);
	belt->SetKeyFocusBorder(buttonBorder);
	belt->Repaint();

	hands->SetPos(20, 140);
	hands->SetSize(100, 20);
	hands->SetCommand("showHands");
	hands->SetFont(buttonFont);
	hands->SetArmedColor(*white, *greenTrans);
	hands->SetDepressedColor(*white, *greenTrans2);
	hands->SetDepressedBorder(buttonBorder);
	hands->SetDefaultColor(*white, *black);
	hands->SetDefaultBorder(buttonBorder);
	hands->SetBorder(buttonBorder);
	hands->SetKeyFocusBorder(buttonBorder);
	hands->Repaint();

	equip->SetPos(20, 170);
	equip->SetSize(100, 20);
	equip->SetCommand("showEquip");
	equip->SetFont(buttonFont);
	equip->SetArmedColor(*white, *greenTrans);
	equip->SetDepressedColor(*white, *greenTrans2);
	equip->SetDepressedBorder(buttonBorder);
	equip->SetDefaultColor(*white, *black);
	equip->SetDefaultBorder(buttonBorder);
	equip->SetBorder(buttonBorder);
	equip->SetKeyFocusBorder(buttonBorder);
	equip->Repaint();

	close->SetPos(20, 460);
	close->SetSize(100, 20);
	close->SetCommand("closeWindow");
	close->SetFont(buttonFont);
	close->SetArmedColor(*white, *greenTrans);
	close->SetDepressedColor(*white, *greenTrans2);
	close->SetDepressedBorder(buttonBorder);
	close->SetDefaultColor(*white, *black);
	close->SetDefaultBorder(buttonBorder);
	close->SetBorder(buttonBorder);
	close->SetKeyFocusBorder(buttonBorder);
	close->Repaint();

	//Q_strcmp(lastMenuCommand, "showBack" );
	//this->ShowBackContents();

	// Sets the border
	SetBorder(border);
}

//-----------------------------------------------------------------------------
// Purpose: Displays the window
//-----------------------------------------------------------------------------
void CClientMSInventoryMenu::ShowPanel(bool bShow)
{
	MoveToCenterOfScreen();

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
		BaseClass::SetVisible( false );
		SetMouseInputEnabled( false );
		SetKeyBoardInputEnabled( false );
	}
}

bool CClientMSInventoryMenu::NeedsUpdate( void )
{
	return (m_fNextUpdateTime < gpGlobals->curtime);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientMSInventoryMenu::Update( void )
{
	if ( !Q_strcmp( lastMenuCommand, "showHands" ) )
		ShowHandsContents();
	else if ( !Q_strcmp( lastMenuCommand, "showBack" ) )
		ShowBackContents();
	else if ( !Q_strcmp( lastMenuCommand, "showSide" ) )
		ShowSideContents();
	else if ( !Q_strcmp( lastMenuCommand, "showBelt" ) )
		ShowBeltContents();
	else if ( !Q_strcmp( lastMenuCommand, "showEquip" ) )
		ShowEquipContents();

	// Update every half a second
	m_fNextUpdateTime = gpGlobals->curtime + 0.5f; 
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientMSInventoryMenu::ShowBackContents()
{
	title->SetText("Backpack");
	title->Repaint();

	// Sets up fonts and colors used
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	vgui::IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme());
	HFont buttonFont = pScheme->GetFont("MSInvenButton");
	vgui::IBorder *buttonBorder = pScheme->GetBorder("MSButtonBorder");
	SetFgColor(Color(255, 255, 255, 255));
	Color* white = new Color(255, 255, 255, 255);
	Color* greenTrans = new Color(96, 224, 64, 56);
	Color* greenTrans2 = new Color(96, 224, 64, 128);
	Color* black = new Color(0, 0, 0, 76);


	for ( int i = 0; i < ARRAYSIZE(m_BackPackSlots); i++ )
	{
		Button &button = *m_BackPackSlots[i].pButton;
		button.SetDropEnabled( true );
		if ( i == 0 ) button.SetPos(175,50);
		else if ( i == 1 ) button.SetPos(207,50);
		else if ( i == 2 ) button.SetPos(239,50);
		else if ( i == 3 ) button.SetPos(271,50);
		else if ( i == 4 ) button.SetPos(303,50);
		else if ( i == 5 ) button.SetPos(175,82);
		else if ( i == 6 ) button.SetPos(207,82);
		else if ( i == 7 ) button.SetPos(239,82);
		else if ( i == 8 ) button.SetPos(271,82);
		else if ( i == 9 ) button.SetPos(303,82);
		else if ( i == 10 ) button.SetPos(175,114);
		else if ( i == 11 ) button.SetPos(207,114);
		else if ( i == 12 ) button.SetPos(239,114);
		else if ( i == 13 ) button.SetPos(271,114);
		else if ( i == 14 ) button.SetPos(303,114);
		else if ( i == 15 ) button.SetPos(175,146);
		else if ( i == 16 ) button.SetPos(207,146);
		else if ( i == 17 ) button.SetPos(239,146);
		else if ( i == 18 ) button.SetPos(271,146);
		else if ( i == 19 ) button.SetPos(303,146);

		button.SetSize(32, 32);
		button.SetCommand( CFmtStrN<32> ( "activateBackPackInventory%i", (i+1) ) );
		button.SetFont(buttonFont);
		button.SetArmedColor(*white, *greenTrans);
		button.SetDepressedColor(*white, *greenTrans2);
		button.SetDepressedBorder(buttonBorder);
		button.SetDefaultColor(*white, *black);
		button.SetDefaultBorder(buttonBorder);
		button.SetBorder(buttonBorder);
		button.SetKeyFocusBorder(buttonBorder);
		button.Repaint();

		char pszItemName[32];

		if ( pPlayer->backpackSlots[ i ] && pPlayer->backpackSlots[ i ][ 0 ] )
		{
			Q_strcpy(pszItemName, pPlayer->backpackSlots[i]);
			m_BackPackSlots[i].Icon->SetImage( GetIconFilePathNameForWeapon(pszItemName) );
			m_BackPackSlots[i].Icon->SetVisible(true);
			m_BackPackSlots[i].Icon->Repaint();
		}
		else
			m_BackPackSlots[i].Icon->SetVisible(false);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientMSInventoryMenu::ShowSideContents()
{
	title->SetText("Sidepack");
	title->Repaint();

	// Sets up fonts and colors used
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	vgui::IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme());
	HFont buttonFont = pScheme->GetFont("MSInvenButton");
	vgui::IBorder *buttonBorder = pScheme->GetBorder("MSButtonBorder");
	SetFgColor(Color(255, 255, 255, 255));
	Color* white = new Color(255, 255, 255, 255);
	Color* greenTrans = new Color(96, 224, 64, 56);
	Color* greenTrans2 = new Color(96, 224, 64, 128);
	Color* black = new Color(0, 0, 0, 76);


	for ( int i = 0; i < ARRAYSIZE(m_SidePackSlots); i++ )
	{
		Button &button = *m_SidePackSlots[i].pButton;
		button.SetDropEnabled( true );
		if ( i == 0 ) button.SetPos(175,50);
		else if ( i == 1 ) button.SetPos(207,50);
		else if ( i == 2 ) button.SetPos(239,50);
		else if ( i == 3 ) button.SetPos(271,50);
		else if ( i == 4 ) button.SetPos(303,50);
		else if ( i == 5 ) button.SetPos(175,82);
		else if ( i == 6 ) button.SetPos(207,82);
		else if ( i == 7 ) button.SetPos(239,82);
		else if ( i == 8 ) button.SetPos(271,82);
		else if ( i == 9 ) button.SetPos(303,82);
		else if ( i == 10 ) button.SetPos(175,114);
		else if ( i == 11 ) button.SetPos(207,114);
		else if ( i == 12 ) button.SetPos(239,114);
		else if ( i == 13 ) button.SetPos(271,114);
		else if ( i == 14 ) button.SetPos(303,114);

		button.SetSize(32, 32);
		button.SetCommand( CFmtStrN<32> ( "activateSidePackInventory%i", (i+1) ) );
		button.SetFont(buttonFont);
		button.SetArmedColor(*white, *greenTrans);
		button.SetDepressedColor(*white, *greenTrans2);
		button.SetDepressedBorder(buttonBorder);
		button.SetDefaultColor(*white, *black);
		button.SetDefaultBorder(buttonBorder);
		button.SetBorder(buttonBorder);
		button.SetKeyFocusBorder(buttonBorder);
		button.Repaint();

		char pszItemName[32];

		if ( pPlayer->sidepackSlots[ i ] && pPlayer->sidepackSlots[ i ][ 0 ] )
		{
			Q_strcpy(pszItemName, pPlayer->sidepackSlots[i]);
			m_SidePackSlots[i].Icon->SetImage( GetIconFilePathNameForWeapon(pszItemName) );
			m_SidePackSlots[i].Icon->SetVisible(true);
			m_SidePackSlots[i].Icon->Repaint();
		}
		else
			m_SidePackSlots[i].Icon->SetVisible(false);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientMSInventoryMenu::ShowBeltContents()
{
	title->SetText("Beltpack");
	title->Repaint();

	// Sets up fonts and colors used
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	vgui::IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme());
	HFont buttonFont = pScheme->GetFont("MSInvenButton");
	vgui::IBorder *buttonBorder = pScheme->GetBorder("MSButtonBorder");
	SetFgColor(Color(255, 255, 255, 255));
	Color* white = new Color(255, 255, 255, 255);
	Color* greenTrans = new Color(96, 224, 64, 56);
	Color* greenTrans2 = new Color(96, 224, 64, 128);
	Color* black = new Color(0, 0, 0, 76);

	for ( int i = 0; i < ARRAYSIZE(m_BeltSlots); i++ )
	{
		Button &button = *m_BeltSlots[i].pButton;
		button.SetDropEnabled( true );
		if ( i == 0 ) button.SetPos(175,50);
		else if ( i == 1 ) button.SetPos(207,50);
		else if ( i == 2 ) button.SetPos(239,50);
		else if ( i == 3 ) button.SetPos(271,50);
		else if ( i == 4 ) button.SetPos(303,50);
		else if ( i == 5 ) button.SetPos(175,82);
		else if ( i == 6 ) button.SetPos(207,82);
		else if ( i == 7 ) button.SetPos(239,82);
		else if ( i == 8 ) button.SetPos(271,82);
		else if ( i == 9 ) button.SetPos(303,82);

		button.SetSize(32, 32);
		button.SetCommand( CFmtStrN<32> ( "activateBeltInventory%i", (i+1) ) );
		button.SetFont(buttonFont);
		button.SetArmedColor(*white, *greenTrans);
		button.SetDepressedColor(*white, *greenTrans2);
		button.SetDepressedBorder(buttonBorder);
		button.SetDefaultColor(*white, *black);
		button.SetDefaultBorder(buttonBorder);
		button.SetBorder(buttonBorder);
		button.SetKeyFocusBorder(buttonBorder);
		button.Repaint();

		char pszItemName[32];

		if ( pPlayer->beltSlots[ i ] && pPlayer->beltSlots[ i ][ 0 ] )
		{
			Q_strcpy(pszItemName, pPlayer->beltSlots[i]);
			m_BeltSlots[i].Icon->SetImage( GetIconFilePathNameForWeapon(pszItemName) );
			m_BeltSlots[i].Icon->SetVisible(true);
			m_BeltSlots[i].Icon->Repaint();
		}
		else
			m_BeltSlots[i].Icon->SetVisible(false);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientMSInventoryMenu::ShowHandsContents()
{
	title->SetText("Hands");
	title->Repaint();

	// Sets up fonts and colors used
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	vgui::IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme());
	HFont buttonFont = pScheme->GetFont("MSInvenButton");
	vgui::IBorder *buttonBorder = pScheme->GetBorder("MSButtonBorder");
	SetFgColor(Color(255, 255, 255, 255));
	Color* white = new Color(255, 255, 255, 255);
	Color* greenTrans = new Color(96, 224, 64, 56);
	Color* greenTrans2 = new Color(96, 224, 64, 128);
	Color* black = new Color(0, 0, 0, 76);

	for ( int i = 0; i < ARRAYSIZE(m_HandSlots); i++ )
	{
		Button &button = *m_HandSlots[i].pButton;
		button.SetDropEnabled( true );
		button.SetPos(175 + 32 * i, 50);
		button.SetSize(32, 32);
		button.SetCommand( CFmtStrN<32> ( "activateHandsInventory%i", (i+1) ) );
		button.SetFont(buttonFont);
		button.SetArmedColor(*white, *greenTrans);
		button.SetDepressedColor(*white, *greenTrans2);
		button.SetDepressedBorder(buttonBorder);
		button.SetDefaultColor(*white, *black);
		button.SetDefaultBorder(buttonBorder);
		button.SetBorder(buttonBorder);
		button.SetKeyFocusBorder(buttonBorder);
		button.Repaint();

		char pszItemName[32];

		if ( pPlayer->handsSlots[ i ] && pPlayer->handsSlots[ i ][ 0 ] ) //This is how you do a NULL check on the client side
		{
			//Msg("\n\n\n");
			//Msg( VarArgs("pPlayer->handsSlots[%i] = ", i) );
			//Msg( STRING(pPlayer->handsSlots[i]) ); //COMMENT ME
			//Msg( pPlayer->handsSlots[i] ); //REMOVE THIS
			//Msg( "\n" );
			Q_strcpy(pszItemName, pPlayer->handsSlots[i]);
			m_HandSlots[i].Icon->SetImage( GetIconFilePathNameForWeapon(pszItemName) );
			m_HandSlots[i].Icon->SetVisible(true);
			m_HandSlots[i].Icon->Repaint();
			//Msg( "pszItemName = " );
			//Msg( pszItemName );
			//Msg("\n\n\n");
		}
		else
			m_HandSlots[i].Icon->SetVisible(false);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientMSInventoryMenu::ShowEquipContents()
{
	title->SetText("Equipped");
	title->Repaint();

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	// Sets up fonts and colors used
	vgui::IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme());
	HFont buttonFont = pScheme->GetFont("MSInvenButton");
	vgui::IBorder *buttonBorder = pScheme->GetBorder("MSButtonBorder");
	SetFgColor(Color(255, 255, 255, 255));
	Color* white = new Color(255, 255, 255, 255);
	Color* greenTrans = new Color(96, 224, 64, 56);
	Color* greenTrans2 = new Color(96, 224, 64, 128);
	Color* black = new Color(0, 0, 0, 76);

	for ( int i = 0; i < ARRAYSIZE(m_EquipSlots); i++ )
	{
		Button &button = *m_EquipSlots[i].pButton;
		button.SetDropEnabled( true );
		button.SetPos(175 + 32 * i, 50);
		button.SetSize(32, 32);
		button.SetCommand( CFmtStrN<32> ( "activateEquipsInventory%i", (i+1) ) );
		button.SetFont(buttonFont);
		button.SetArmedColor(*white, *greenTrans);
		button.SetDepressedColor(*white, *greenTrans2);
		button.SetDepressedBorder(buttonBorder);
		button.SetDefaultColor(*white, *black);
		button.SetDefaultBorder(buttonBorder);
		button.SetBorder(buttonBorder);
		button.SetKeyFocusBorder(buttonBorder);
		button.Repaint();

		char pszItemName[32];

		if ( pPlayer->equipSlots[ i ] && pPlayer->equipSlots[ i ][ 0 ] ) //This is how you do a NULL check on the client side
		{
			Q_strcpy(pszItemName, pPlayer->equipSlots[i]);
			m_EquipSlots[i].Icon->SetImage( GetIconFilePathNameForWeapon(pszItemName) );
			m_EquipSlots[i].Icon->SetVisible(true);
			m_EquipSlots[i].Icon->Repaint();
		}
		else
			m_EquipSlots[i].Icon->SetVisible(false);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientMSInventoryMenu::CloseWindow()
{
	BaseClass::SetVisible( false );
	SetMouseInputEnabled( false );
	SetKeyBoardInputEnabled( false );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClientMSInventoryMenu::OnCommand( const char *command )
{
	if (!Q_strcmp(command, "showBack"))
	{
		this->ShowBackContents();

		for( int i = 0; i < ARRAYSIZE(m_HandSlots); i++ )
			m_HandSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_BackPackSlots); i++ )
			m_BackPackSlots[i].pButton->SetVisible(true);
		for( int i = 0; i < ARRAYSIZE(m_SidePackSlots); i++ )
			m_SidePackSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_BeltSlots); i++ )
			m_BeltSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_EquipSlots); i++ )
			m_EquipSlots[i].pButton->SetVisible(false);
	
		for ( int i = 0; i < MAX_SIDEPACK_IMAGES; i++ )
			m_SidePackSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_BELT_IMAGES; i++ )
			m_BeltSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_HANDS_IMAGES; i++ )
			m_HandSlots[i].Icon->SetVisible(false);
		for (int i = 0; i < MAX_EQUIP_IMAGES; i++ )
			m_EquipSlots[i].Icon->SetVisible(false);	

		Q_strcpy( lastMenuCommand, command );
	}

	if (!Q_strcmp(command, "showSide"))
	{
		this->ShowSideContents();

		for( int i = 0; i < ARRAYSIZE(m_HandSlots); i++ )
			m_HandSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_BackPackSlots); i++ )
			m_BackPackSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_SidePackSlots); i++ )
			m_SidePackSlots[i].pButton->SetVisible(true);
		for( int i = 0; i < ARRAYSIZE(m_BeltSlots); i++ )
			m_BeltSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_EquipSlots); i++ )
			m_EquipSlots[i].pButton->SetVisible(false);

		for ( int i = 0; i < MAX_BACKPACK_IMAGES; i++ )
			m_BackPackSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_BELT_IMAGES; i++ )
			m_BeltSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_HANDS_IMAGES; i++ )
			m_HandSlots[i].Icon->SetVisible(false);
		for (int i = 0; i < MAX_EQUIP_IMAGES; i++ )
			m_EquipSlots[i].Icon->SetVisible(false);

		Q_strcpy( lastMenuCommand, command );
	}


	if (!Q_strcmp(command, "showBelt"))
	{
		this->ShowBeltContents();

		for( int i = 0; i < ARRAYSIZE(m_HandSlots); i++ )
			m_HandSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_BackPackSlots); i++ )
			m_BackPackSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_SidePackSlots); i++ )
			m_SidePackSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_BeltSlots); i++ )
			m_BeltSlots[i].pButton->SetVisible(true);
		for( int i = 0; i < ARRAYSIZE(m_EquipSlots); i++ )
			m_EquipSlots[i].pButton->SetVisible(false);

		for ( int i = 0; i < MAX_BACKPACK_IMAGES; i++ )
			m_BackPackSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_SIDEPACK_IMAGES; i++ )
			m_SidePackSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_HANDS_IMAGES; i++ )
			m_HandSlots[i].Icon->SetVisible(false);
		for (int i = 0; i < MAX_EQUIP_IMAGES; i++ )
			m_EquipSlots[i].Icon->SetVisible(false);

		Q_strcpy( lastMenuCommand, command );
	}

	if (!Q_strcmp(command, "showHands"))
	{
		this->ShowHandsContents();

		for( int i = 0; i < ARRAYSIZE(m_HandSlots); i++ )
			m_HandSlots[i].pButton->SetVisible(true);
		for( int i = 0; i < ARRAYSIZE(m_BackPackSlots); i++ )
			m_BackPackSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_SidePackSlots); i++ )
			m_SidePackSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_BeltSlots); i++ )
			m_BeltSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_EquipSlots); i++ )
			m_EquipSlots[i].pButton->SetVisible(false);

		for ( int i = 0; i < MAX_BACKPACK_IMAGES; i++ )
			m_BackPackSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_SIDEPACK_IMAGES; i++ )
			m_SidePackSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_BELT_IMAGES; i++ )
			m_BeltSlots[i].Icon->SetVisible(false);
		for (int i = 0; i < MAX_EQUIP_IMAGES; i++ )
			m_EquipSlots[i].Icon->SetVisible(false);

		Q_strcpy( lastMenuCommand, command );
	}
	if (!Q_strcmp(command, "showEquip"))
	{
		this->ShowEquipContents();

		for( int i = 0; i < ARRAYSIZE(m_HandSlots); i++ )
			m_HandSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_BackPackSlots); i++ )
			m_BackPackSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_SidePackSlots); i++ )
			m_SidePackSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_BeltSlots); i++ )
			m_BeltSlots[i].pButton->SetVisible(false);
		for( int i = 0; i < ARRAYSIZE(m_EquipSlots); i++ )
			m_EquipSlots[i].pButton->SetVisible(true);

		for ( int i = 0; i < MAX_BACKPACK_IMAGES; i++ )
			m_BackPackSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_SIDEPACK_IMAGES; i++ )
			m_SidePackSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_BELT_IMAGES; i++ )
			m_BeltSlots[i].Icon->SetVisible(false);
		for ( int i = 0; i < MAX_HANDS_IMAGES; i++ )
			m_HandSlots[i].Icon->SetVisible(false);

		Q_strcpy( lastMenuCommand, command );
	}

	if (!Q_strcmp(command, "closeWindow"))
	{
		this->CloseWindow();
	}
}

//Returns the image icon filename/pathway for the weapon name
const char *CClientMSInventoryMenu::GetIconFilePathNameForWeapon( const char *weaponName )
{
	char iconPathName[64] = "scripts/";
	strcat(iconPathName, weaponName);
	strcat(iconPathName, ".txt");

	KeyValues *kV = new KeyValues( "WeaponData" );

	kV->LoadFromFile( filesystem, iconPathName, "MOD" );

	if ( !Q_strcmp( kV->GetName(), "WeaponData" ) )
	{
		return kV->GetString( "icon", "Error" );
	}
	kV->deleteThis(); //Revisit later
	return "Error";
}

int CClientMSInventoryMenu::GetMouseOverSlot( )
{	
	if ( !Q_strcmp( lastMenuCommand, "showHands" ) )
	{
		for( int i = 0; i < ARRAYSIZE(m_HandSlots); i++ )
		{
			inv_slot_s &CheckSlot = m_HandSlots[i];
		
			if( CheckSlot.pButton->IsCursorOver( ) )
				return i;
		}
	}
	else if ( !Q_strcmp( lastMenuCommand, "showBack" ) )
	{
		for( int i = 0; i < ARRAYSIZE(m_BackPackSlots); i++ )
		{
			inv_slot_s &CheckSlot = m_BackPackSlots[i];
	
			if( CheckSlot.pButton->IsCursorOver( ) )
				return i;
		}
	}
	else if ( !Q_strcmp( lastMenuCommand, "showSide" ) )
	{
		for( int i = 0; i < ARRAYSIZE(m_SidePackSlots); i++ )
		{
			inv_slot_s &CheckSlot = m_SidePackSlots[i];
		
			if( CheckSlot.pButton->IsCursorOver( ) )
				return i;
		}
	}
	else if ( !Q_strcmp( lastMenuCommand, "showBelt" ) )
	{
		for( int i = 0; i < ARRAYSIZE(m_BeltSlots); i++ )
		{
			inv_slot_s &CheckSlot = m_BeltSlots[i];
		
			if( CheckSlot.pButton->IsCursorOver( ) )
				return i;
		}
	}
	else if ( !Q_strcmp( lastMenuCommand, "showEquip" ) )
	{
		for( int i = 0; i < ARRAYSIZE(m_EquipSlots); i++ )
		{
			inv_slot_s &CheckSlot = m_EquipSlots[i];
		
			if( CheckSlot.pButton->IsCursorOver( ) )
				return i;
		}
	}

	return -1;
}

void CClientMSInventoryMenu::OnCursorMoved( int x, int y )
{
	if( vgui::input()->IsMouseDown( MOUSE_LEFT ) )
	{
		//If I haven't started dragging yet...
		if( !m_DragDropIcon->IsVisible( ) )
		{
			int dragSlotIdx = GetMouseOverSlot( );
			if( dragSlotIdx > -1 )
			{
				m_DragSlotIdx = dragSlotIdx;

				if ( !Q_strcmp( lastMenuCommand, "showHands" ) )
				{
					inv_slot_s &dragSlot = m_HandSlots[dragSlotIdx];
					m_DragDropIcon->SetImage( dragSlot.Icon->GetImage( ) );
					m_DragDropIcon->SetVisible( true );
				}
				else if ( !Q_strcmp( lastMenuCommand, "showBack" ) )
				{
					inv_slot_s &dragSlot = m_BackPackSlots[dragSlotIdx];
					m_DragDropIcon->SetImage( dragSlot.Icon->GetImage( ) );
					m_DragDropIcon->SetVisible( true );
				}
				else if ( !Q_strcmp( lastMenuCommand, "showSide" ) )
				{
					inv_slot_s &dragSlot = m_SidePackSlots[dragSlotIdx];
					m_DragDropIcon->SetImage( dragSlot.Icon->GetImage( ) );
					m_DragDropIcon->SetVisible( true );
				}
				else if ( !Q_strcmp( lastMenuCommand, "showBelt" ) )
				{
					inv_slot_s &dragSlot = m_BeltSlots[dragSlotIdx];
					m_DragDropIcon->SetImage( dragSlot.Icon->GetImage( ) );
					m_DragDropIcon->SetVisible( true );
				}
				else if ( !Q_strcmp( lastMenuCommand, "showEquip" ) )
				{
					inv_slot_s &dragSlot = m_EquipSlots[dragSlotIdx];
					m_DragDropIcon->SetImage( dragSlot.Icon->GetImage( ) );
					m_DragDropIcon->SetVisible( true );
				}
			}
		}

		//If I'm dragging an item...
		else
		{
			m_DragDropIcon->SetPos( x - 16, y -16 );
		}
	}

	//If I was dragging an item and let go...
	else if( m_DragDropIcon->IsVisible( ) )
	{
		int dragSlotIdx = GetMouseOverSlot( );
		if( dragSlotIdx > -1 )
		{
			//Move to new slot
		}

		//Stop dragging
		m_DragSlotIdx = -1;
		m_DragDropIcon->SetVisible( false );
	}
}

void CClientMSInventoryMenu::OnDraggedItem( Panel *pStartPanel, Panel *pEndPanel )
{	
	if ( !Q_strcmp( lastMenuCommand, "showHands" ) )
	{
		for( int i = 0; i < ARRAYSIZE(m_HandSlots); i++ )
		{
			if( m_HandSlots[i].Icon == pStartPanel )
			{
				if ( pEndPanel == back )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 1, i, 2, -1) );
					return;
				}
				else if ( pEndPanel == side )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 1, i, 3, -1) );
					return;
				}
				else if ( pEndPanel == belt )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 1, i, 4, -1) );
					return;
				}
				else if ( pEndPanel == equip )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 1, i, 5, -1) );
					return;
				}

				for( int r = 0; r < ARRAYSIZE(m_HandSlots); r++ )
				{
					if( m_HandSlots[r].pButton == pEndPanel )
					{
						//Dragged item to a new slot
						engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 1, i, 1, r) );
						break;
					}
				}
			}
		}
	}
	else if ( !Q_strcmp( lastMenuCommand, "showBack" ) )
	{
		for( int i = 0; i < ARRAYSIZE(m_BackPackSlots); i++ )
		{
			if( m_BackPackSlots[i].Icon == pStartPanel )
			{
				if ( pEndPanel == hands )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 2, i, 1, -1) );
					return;
				}
				else if ( pEndPanel == side )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 2, i, 3, -1) );
					return;
				}
				else if ( pEndPanel == belt )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 2, i, 4, -1) );
					return;
				}
				else if ( pEndPanel == equip )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 1, i, 5, -1) );
					return;
				}

				for( int r = 0; r < ARRAYSIZE(m_BackPackSlots); r++ )
				{
					if( m_BackPackSlots[r].pButton == pEndPanel )
					{
						//Dragged item to a new slot
						engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 2, i, 2, r) );
						break;
					}
				}
			}
		}
	}
	else if ( !Q_strcmp( lastMenuCommand, "showSide" ) )
	{
		for( int i = 0; i < ARRAYSIZE(m_SidePackSlots); i++ )
		{
			if( m_SidePackSlots[i].Icon == pStartPanel )
			{
				if ( pEndPanel == hands )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 3, i, 1, -1) );
					return;
				}
				else if ( pEndPanel == back )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 3, i, 2, -1) );
					return;
				}
				else if ( pEndPanel == belt )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 3, i, 4, -1) );
					return;
				}
				else if ( pEndPanel == equip )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 3, i, 5, -1) );
					return;
				}

				for( int r = 0; r < ARRAYSIZE(m_SidePackSlots); r++ )
				{
					if( m_SidePackSlots[r].pButton == pEndPanel )
					{
						//Dragged item to a new slot
						engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 3, i, 3, r) );
						break;
					}
				}
			}
		}
	}
	else if ( !Q_strcmp( lastMenuCommand, "showBelt" ) )
	{
		for( int i = 0; i < ARRAYSIZE(m_BeltSlots); i++ )
		{
			if( m_BeltSlots[i].Icon == pStartPanel )
			{
				if ( pEndPanel == hands )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 4, i, 1, -1) );
					return;
				}
				else if ( pEndPanel == back )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 4, i, 2, -1) );
					return;
				}
				else if ( pEndPanel == side )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 4, i, 3, -1) );
					return;
				}
				else if ( pEndPanel == equip )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 4, i, 5, -1) );
					return;
				}

				for( int r = 0; r < ARRAYSIZE(m_BeltSlots); r++ )
				{
					if( m_BeltSlots[r].pButton == pEndPanel )
					{
						//Dragged item to a new slot
						engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 4, i, 4, r) );
						break;
					}
				}
			}
		}
	}
	else if ( !Q_strcmp( lastMenuCommand, "showEquip" ) )
	{
		for( int i = 0; i < ARRAYSIZE(m_EquipSlots); i++ )
		{
			if( m_EquipSlots[i].Icon == pStartPanel )
			{
				if ( pEndPanel == hands )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 5, i, 1, -1) );
					return;
				}
				else if ( pEndPanel == back )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 5, i, 2, -1) );
					return;
				}
				else if ( pEndPanel == side )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 5, i, 3, -1) );
					return;
				}
				else if ( pEndPanel == belt )
				{
					engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 5, i, 4, -1) );
					return;
				}

				for( int r = 0; r < ARRAYSIZE(m_EquipSlots); r++ )
				{
					if( m_EquipSlots[r].pButton == pEndPanel )
					{
						//Dragged item to a new slot
						engine->ClientCmd( VarArgs("moveitem %i %i %i %i", 5, i, 5, r) );
						break;
					}
				}
			}
		}
	}
}

void DraggableImage::OnDraggablePanelPaint( )
{
	IImage *pImage = GetImage();
	if( pImage )
	{
		int x = 0, y = 0;
		input()->GetCursorPos(x, y);

		int w = 0, h = 0;
		pImage->GetSize( w, h );

		pImage->SetPos(x - (w * 0.5f), y - (h * 0.5f));

		pImage->Paint();
	}
}
bool DroppableButton::IsDroppable( CUtlVector< KeyValues * >& msglist )
{
	return true;
}

void DroppableButton::OnPanelDropped( CUtlVector< KeyValues * >& msglist )
{
	Panel *pDragPanel = (Panel *)msglist.Head()->GetFirstSubKey()->GetPtr();
	CClientMSInventoryMenu *pParentMenu = (CClientMSInventoryMenu *)GetParent( );
	pParentMenu->OnDraggedItem( pDragPanel, this );
}

void DroppableButton::OnPanelEnteredDroppablePanel( CUtlVector< KeyValues * >& msglist )
{

}
void DroppableButton::OnPanelExitedDroppablePanel ( CUtlVector< KeyValues * >& msglist )
{
}

*/