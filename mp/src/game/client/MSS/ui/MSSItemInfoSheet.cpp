
// BOXBOX An item information window that pops up when you hover over an item in the inventory menu

#include "cbase.h"
#include "MSSItemInfoSheet.h"

#include "c_MSS_player.h"

#include "ienginevgui.h"
//#include <utlstring.h>
#include <networkstringtabledefs.h>
#include <cdll_client_int.h>
#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
//#include <FileSystem.h>
//#include <KeyValues.h>
#include <convar.h>
#include <vgui_controls/ImageList.h>
#include <vgui_controls/TextEntry.h>
//#include <vgui_controls/Button.h>
#include <game/client/iviewport.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;
//extern INetworkStringTable *g_pStringTableInfoPanel;

extern const char *pszSkillNames[];
/*
extern const char *pszGenderNames[];
extern const char *pszRaceNames[];
extern const char *pszSkillLevelNames[];
extern const char *pszPlayerModels[];
*/

CMSItemInfoSheet::CMSItemInfoSheet(IViewPort *pViewPort) : Frame( NULL, PANEL_ITEMINFO )
{
	m_pViewPort = pViewPort;

	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/MSScheme.res", "MSScheme");
	SetScheme( scheme );

	SetMoveable( false );
	SetSizeable( false );
	SetProportional( true );
	SetTitleBarVisible( false );
	SetKeyBoardInputEnabled( false );

	m_pItemNameLabel		= new Label( this, "ItemNameLabel", "0" );
	m_pDescription			= new TextEntry(this, "Description");
	m_pDescription->SetMultiline( true );
//	m_pItemDescriptionLabel	= new Label( this, "ItemDescriptionLabel", "0" );

	m_pItemTypeLabel		= new Label( this, "ItemTypeLabel", "0" );
	m_pPropertyLabel		= new Label( this, "PropertyLabel", "0" );
	m_pValueLabel			= new Label( this, "ValueLabel", "0" );
//	m_pGoldValueLabel		= new Label( this, "GoldValueLabel", "0" );


//	m_pMinDamageLabel		= new Label( this, "MinDamageLabel", "0" );
//	m_pDashLabel			= new Label( this, "DashLabel", "0" );
//	m_pMaxDamageLabel		= new Label( this, "MaxDamageLabel", "0" );

	m_pItemModel = new CModelPanel( this, "itemmodel" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSItemInfoSheet::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSItemInfoSheet.res");

//	m_pDescription->SetFont( pScheme->GetFont( "WritingFontSmall" ) );
	m_pDescription->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
//	Reset();
}

CMSItemInfoSheet::~CMSItemInfoSheet()
{

}

void CMSItemInfoSheet::Reset( void )
{

}

void CMSItemInfoSheet::Update( void )
{

}

/*
void CMSStatsMenu::HideAllChildControls( void )
{
	m_pLeftPageTitleLabel->SetText( "" );
	m_pRightPageTitleLabel->SetText( "" );

	m_pSexLabel->SetVisible( false );
	m_pRaceLabel->SetVisible( false );
	m_pTotalExpText->SetVisible( false );
	m_pTotalExpLabel->SetVisible( false );

}
*/
/*
void CMSItemInfoSheet::OnCommand( const char *command )
{
	if (!Q_strcmp(command, "TurnPageRight"))
    {
		PlayPageTurnSound();
		m_pViewPort->ShowPanel( this, false );
		m_pViewPort->ShowPanel( PANEL_INVENTORYMENU, true );
		engine->ClientCmd( "turnpage 2" );
	}

	BaseClass::OnCommand(command);
}
*/

/*
const FileItemInfo_t &CMSItemInfoSheet::GetItemData( ITEM_FILE_INFO_HANDLE item ) const
{
	return *GetFileItemInfoFromHandle( item );
}
*/

void CMSItemInfoSheet::SetItemToDisplay( ITEM_FILE_INFO_HANDLE item )
{
	FileItemInfo_t	*pItem = GetFileItemInfoFromHandle( item );

	m_pItemNameLabel->SetText( pItem->szPrintName );
	m_pDescription->SetVisible( true );
	m_pDescription->SetText( pItem->szDescription );
	m_pDescription->GotoTextStart();
//	m_pItemDescriptionLabel->SetText( pItem->szDescription );

//	m_pGoldValueText		= new Label( this, "GoldValueText", "0" );
	if( pItem->nItemType == ITEMTYPE_WEAPON )
	{
		m_pItemTypeLabel->SetText( pszSkillNames[ pItem->nWeaponType ] );
		m_pPropertyLabel->SetText( VarArgs("DAMAGE:  %i - %i", 100, 100 ) );
	}
	
	m_pValueLabel->SetText( VarArgs("VALUE: %i", 10000 ) );

//	m_pDamageText			= new Label( this, "DamageText", "0" );
//	m_pMinDamageLabel->SetText( "BOXBOX TODO" );
//	m_pDashLabel			= new Label( this, "DashLabel", "0" );
//	m_pMaxDamageLabel->SetText( "BOXBOX TODO" );

	m_pItemModel->SwapModel( pItem->szWorldModel, NULL ); 
	m_pItemModel->MoveToFront();	
}

void CMSItemInfoSheet::ShowPanel( bool bShow )
{
//	if ( BaseClass::IsVisible() == bShow )
//		return;

//	m_pViewPort->ShowBackGround( bShow );

//	if( IsVisible() == bShow )
//		return;

	if ( bShow )
	{
		SetVisible( true );
//		Activate();
//		SetMouseInputEnabled( true );
	}
	else
	{
		SetVisible( false );
//		SetMouseInputEnabled( false );
	}
}

