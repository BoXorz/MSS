
// BOXBOX The portion of the Main Menu, where players purchase level up items

#include "cbase.h"
#include "MSSPowerupMenu.h"

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

CMSPowerupMenu::CMSPowerupMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_POWERUPMENU )
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
/*
	m_pSexLabel			= new Label( this, "SexLabel", "0" );
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

	m_nCurPage = 1;
*/
}

void CMSPowerupMenu::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSPowerupMenu.res");

//	m_pLeftPageNumLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
//	m_pRightPageNumLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );

//	m_pLeftPageTitleLabel->SetFont( pScheme->GetFont( "HeaderFontSmall" ) );
//	m_pRightPageTitleLabel->SetFont( pScheme->GetFont( "HeaderFontSmaller" ) );

//	m_pSexLabel->SetFgColor( pScheme->GetColor( "RedInk", Color(0, 0, 0, 0) ) );
//	m_pRaceLabel->SetFgColor( pScheme->GetColor( "RedInk", Color(0, 0, 0, 0) ) );
//	m_pTotalExpLabel->SetFgColor( pScheme->GetColor( "InkWellLight", Color(0, 0, 0, 0) ) );

	Reset();
}

CMSPowerupMenu::~CMSPowerupMenu()
{

}

void CMSPowerupMenu::Reset( void )
{
	Update();
}


void CMSPowerupMenu::Update( void )
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

//	HideAllChildControls();

//	if( m_nCurPage == 1 )
//	{
//		m_pLeftPageTitleLabel->SetText( "#MSS_PLAYERSTATS" );
		m_pRightPageTitleLabel->SetText( pPlayer->m_pszCharName );
/*
		m_pSexLabel->SetVisible( true );
		m_pRaceLabel->SetVisible( true );
		m_pTotalExpText->SetVisible( true );
		m_pTotalExpLabel->SetVisible( true );

		m_pSexLabel->SetText( pszGenderNames[ pPlayer->m_nGender ] );
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
*/


}

void CMSPowerupMenu::OnCommand( const char *command )
{
    if (!Q_strcmp(command, "TurnPageLeft"))
    {
		PlayPageTurnSound();
		m_pViewPort->ShowPanel( this, false );
		m_pViewPort->ShowPanel( PANEL_INVENTORYMENU, true );
		engine->ClientCmd( "turnpage 2" );
	}
    else if (!Q_strcmp(command, "TurnPageRight"))
    {
//		PlayPageTurnSound();
		// BOXBOX TODO when more main menu sub panels are added...
	}

	BaseClass::OnCommand(command);
}

void CMSPowerupMenu::PlayPageTurnSound( void )
{
	int rnd = RandomInt( 2, 4 );
	vgui::surface()->PlaySound( VarArgs("UI/pageturn%i.wav", rnd ) );
}

void CMSPowerupMenu::ShowPanel( bool bShow )
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

