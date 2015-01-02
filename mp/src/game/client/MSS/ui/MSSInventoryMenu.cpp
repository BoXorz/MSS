
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

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;
extern INetworkStringTable *g_pStringTableInfoPanel;

//#define TEMP_HTML_FILE	"textwindow_temp.html"

extern const char *pszGenderNames[];
extern const char *pszRaceNames[];
extern const char *pszSkillNames[];
extern const char *pszSkillLevelNames[];

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
//	SetMenuButtonVisible( false );

	m_pRightPageTitleLabel = new Label( this, "RPageTitleLabel", "0" );

	m_pGoldLabel = new Label( this, "GoldLabel", "0" );
}

void CMSInventoryMenu::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSInventoryMenu.res");

//	m_pLeftPageNumLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
//	m_pRightPageNumLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );

//	m_pLeftPageTitleLabel->SetFont( pScheme->GetFont( "HeaderFontSmall" ) );
//	m_pRightPageTitleLabel->SetFont( pScheme->GetFont( "HeaderFontSmaller" ) );

//	m_pGenderLabel->SetFgColor( pScheme->GetColor( "RedInk", Color(0, 0, 0, 0) ) );
//	m_pRaceLabel->SetFgColor( pScheme->GetColor( "RedInk", Color(0, 0, 0, 0) ) );
//	m_pTotalExpLabel->SetFgColor( pScheme->GetColor( "InkWellLight", Color(0, 0, 0, 0) ) );

	Reset();
}

CMSInventoryMenu::~CMSInventoryMenu()
{

}

void CMSInventoryMenu::Reset( void )
{
//	m_nCurPage = 1;
	Update();
}


void CMSInventoryMenu::Update( void )
{
//	char lpg[4];
//	char rpg[4];
//	Q_snprintf( lpg, sizeof( lpg ), "%i", m_nCurPage );
//	Q_snprintf( rpg, sizeof( rpg ), "%i", m_nCurPage + 1 );
//	m_pLeftPageNumLabel->SetText( lpg );
//	m_pRightPageNumLabel->SetText( rpg );

	C_MSS_Player *pPlayer = ToMSSPlayer( C_BasePlayer::GetLocalPlayer() );
	if( !pPlayer ) return;

	pPlayer->TabulateStats();

	m_pRightPageTitleLabel->SetText( pPlayer->m_pszCharName );

	char buf[10];
	itoa( pPlayer->m_nGold, buf, 10 );
	m_pGoldLabel->SetText( buf );

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

//	m_pViewPort->ShowBackGround( bShow );

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