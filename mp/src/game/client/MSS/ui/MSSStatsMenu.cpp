
// BOXBOX The Stats portion of the Main Menu

#include "cbase.h"
#include "MSSStatsMenu.h"
//#include "MSSPlayerStatsMenu.h"
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
extern const char *pszPlayerModels[];

CMSStatsMenu::CMSStatsMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_STATSMENU )
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

//	m_pLeftPageNumLabel = new Label( this, "LPageNumLabel", "0" );
//	m_pRightPageNumLabel = new Label( this, "RPageNumLabel", "0" );

//	m_pLeftPageTitleLabel = new Label( this, "LPageTitleLabel", "0" );
	m_pRightPageTitleLabel = new Label( this, "RPageTitleLabel", "0" );

	m_pGenderLabel		= new Label( this, "GenderLabel", "0" );
	m_pRaceLabel		= new Label( this, "RaceLabel", "0" );
	m_pTotalExpText		= new Label( this, "TotalExpText", "0" );
	m_pTotalExpLabel	= new Label( this, "TotalExpLabel", "0" );

	m_pUnarmedLabel		= new Label( this, "UnarmedLabel", "0" );
	m_p1HPiercingLabel	= new Label( this, "OneHandPiercingLabel", "0" );
	m_p1HSlashingLabel	= new Label( this, "OneHandSlashingLabel", "0" );
	m_p1HBashingLabel	= new Label( this, "OneHandBashingLabel", "0" );
	m_p2HPiercingLabel	= new Label( this, "TwoHandPiercingLabel", "0" );
	m_p2HSlashingLabel	= new Label( this, "TwoHandSlashingLabel", "0" );
	m_p2HBashingLabel	= new Label( this, "TwoHandBashingLabel", "0" );
	m_pArcheryLabel		= new Label( this, "ArcheryLabel", "0" );
	m_pThrowWeaponLabel	= new Label( this, "ThrownWeaponsLabel", "0" );

	m_pMightLabel		= new Label( this, "MightLabel", "0" );
	m_pAgilityLabel		= new Label( this, "AgilityLabel", "0" );
	m_pStaminaLabel		= new Label( this, "StaminaLabel", "0" );
	m_pIntellectLabel	= new Label( this, "IntellectLabel", "0" );
	m_pWitLabel			= new Label( this, "WitLabel", "0" );
	m_pFortitudeLabel	= new Label( this, "FortitudeLabel", "0" );
	m_pLuckLabel		= new Label( this, "LuckLabel", "0" );

	m_pAlchemyLabel		= new Label( this, "AlchemyLabel", "0" );
	m_pClothworkLabel	= new Label( this, "ClothworkLabel", "0" );
	m_pWoodworkLabel	= new Label( this, "WoodworkLabel", "0" );
	m_pStoneworkLabel	= new Label( this, "StoneworkLabel", "0" );
	m_pMetalworkLabel	= new Label( this, "MetalworkLabel", "0" );

	m_pCharModel = new CModelPanel( this, "charmodel" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSStatsMenu::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSStatsMenu.res");

//	m_pLeftPageNumLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
//	m_pRightPageNumLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );

//	m_pLeftPageTitleLabel->SetFont( pScheme->GetFont( "HeaderFontSmall" ) );
//	m_pRightPageTitleLabel->SetFont( pScheme->GetFont( "HeaderFontSmaller" ) );

	m_pGenderLabel->SetFgColor( pScheme->GetColor( "RedInk", Color(0, 0, 0, 0) ) );
	m_pRaceLabel->SetFgColor( pScheme->GetColor( "RedInk", Color(0, 0, 0, 0) ) );
	m_pTotalExpLabel->SetFgColor( pScheme->GetColor( "InkWellLight", Color(0, 0, 0, 0) ) );

	Reset();
}

CMSStatsMenu::~CMSStatsMenu()
{

}

void CMSStatsMenu::Reset( void )
{
//	m_nCurPage = 1;
	Update();
}


void CMSStatsMenu::Update( void )
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

	m_pGenderLabel->SetText( pszGenderNames[ pPlayer->m_nGender ] );
	m_pRaceLabel->SetText( pszRaceNames[ pPlayer->m_nRace ] );

	char buf[8];
	itoa( pPlayer->m_nTotalExp, buf, 10 );
	m_pTotalExpLabel->SetText( buf );

	m_pUnarmedLabel->SetText( pszSkillLevelNames[ pPlayer->m_nWeaponSkills[ WEAPONTYPE_UNARMED ] ] );
	m_p1HPiercingLabel->SetText( pszSkillLevelNames[ pPlayer->m_nWeaponSkills[ WEAPONTYPE_ONEHANDPIERCING ] ] );
	m_p1HSlashingLabel->SetText( pszSkillLevelNames[ pPlayer->m_nWeaponSkills[ WEAPONTYPE_ONEHANDSLASHING ] ] );
	m_p1HBashingLabel->SetText( pszSkillLevelNames[ pPlayer->m_nWeaponSkills[ WEAPONTYPE_ONEHANDBASHING ] ] );
	m_p2HPiercingLabel->SetText( pszSkillLevelNames[ pPlayer->m_nWeaponSkills[ WEAPONTYPE_TWOHANDPIERCING ] ] );
	m_p2HSlashingLabel->SetText( pszSkillLevelNames[ pPlayer->m_nWeaponSkills[ WEAPONTYPE_TWOHANDSLASHING ] ] );
	m_p2HBashingLabel->SetText( pszSkillLevelNames[ pPlayer->m_nWeaponSkills[ WEAPONTYPE_TWOHANDBASHING ] ] );
	m_pArcheryLabel->SetText( pszSkillLevelNames[ pPlayer->m_nWeaponSkills[ WEAPONTYPE_ARCHERY ] ] );
	m_pThrowWeaponLabel->SetText( pszSkillLevelNames[ pPlayer->m_nWeaponSkills[ WEAPONTYPE_THROWN ] ] );

	itoa( pPlayer->m_nMight, buf, 10 );
	m_pMightLabel->SetText( buf );
	itoa( pPlayer->m_nAgility, buf, 10 );
	m_pAgilityLabel->SetText( buf );
	itoa( pPlayer->m_nStamina, buf, 10 );
	m_pStaminaLabel->SetText( buf );
	itoa( pPlayer->m_nIntellect, buf, 10 );
	m_pIntellectLabel->SetText( buf );
	itoa( pPlayer->m_nWit, buf, 10 );
	m_pWitLabel->SetText( buf );
	itoa( pPlayer->m_nFortitude, buf, 10 );
	m_pFortitudeLabel->SetText( buf );
	itoa( pPlayer->m_nLuck, buf, 10 );
	m_pLuckLabel->SetText( buf );

	m_pAlchemyLabel->SetText( pszSkillLevelNames[ pPlayer->m_nCraftSkills[ CRAFT_ALCHEMY ] ] );
	m_pClothworkLabel->SetText( pszSkillLevelNames[ pPlayer->m_nCraftSkills[ CRAFT_CLOTHWORK ] ] );
	m_pWoodworkLabel->SetText( pszSkillLevelNames[ pPlayer->m_nCraftSkills[ CRAFT_WOODWORK ] ] );
	m_pStoneworkLabel->SetText( pszSkillLevelNames[ pPlayer->m_nCraftSkills[ CRAFT_STONEWORK ] ] );
	m_pMetalworkLabel->SetText( pszSkillLevelNames[ pPlayer->m_nCraftSkills[ CRAFT_METALWORK ] ] );

	m_pCharModel->SwapModel( pszPlayerModels[ pPlayer->m_nPlayerModelIndex ], NULL ); 
	m_pCharModel->MoveToFront();
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

void CMSStatsMenu::OnCommand( const char *command )
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

void CMSStatsMenu::PlayPageTurnSound( void )
{
	int rnd = RandomInt( 0, 1 );
	if( rnd )
		vgui::surface()->PlaySound( "UI/pageturn2.wav" );
	else
		vgui::surface()->PlaySound( "UI/pageturn3.wav" );
}

void CMSStatsMenu::ShowPanel( bool bShow )
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

