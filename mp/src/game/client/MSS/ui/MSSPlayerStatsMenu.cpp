
#include "cbase.h"
#include "MSSPlayerStatsMenu.h"
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

#define TEMP_HTML_FILE	"textwindow_temp.html"



CMSPlayerStatsMenu::CMSPlayerStatsMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_PLAYERSTATS )
{
	// initialize dialog
	m_pViewPort = pViewPort;

//	SetTitle("", true);

//	m_szTitle[0] = '\0';
//	m_szMessage[0] = '\0';
//	m_szExitCommand[0] = '\0';
	
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/MSScheme.res", "MSScheme");
	SetScheme(scheme);

	SetMoveable(false);
	SetSizeable(false);
//	SetSize( 700, 500 );
	SetProportional(true);
	SetTitleBarVisible( false );
//	SetVisible( false );

	SetMouseInputEnabled( false );
	SetKeyBoardInputEnabled( false );

//	m_pServerNameLabel	= new Label( this, "ServerName", "Server Name" );
//	m_pTextMessage		= new TextEntry(this, "TextMessage");
//	m_pHTMLMessage		= new HTML(this,"HTMLMessage");;

//	m_pOK = new Button(this, "ok", "#MSS_TURNPAGE");
//	m_pOK->SetCommand("JoinClose");

//	m_pTextMessage->SetMultiline( true );
//	m_nContentType = TYPE_TEXT;
//	m_bJustJoined = false; // BOXBOX added
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSPlayerStatsMenu::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSPlayerStatsMenu.res");

//	m_bgColor = GetSchemeColor("BgColor", GetBgColor(), pScheme);
//	m_borderColor = pScheme->GetColor( "MSBorderColor", Color( 0, 0, 0, 0 ) );

//	m_pServerNameLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
//	m_pServerNameLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
//	m_pTextMessage->SetFont( pScheme->GetFont( "WritingFont" ) );
//	m_pTextMessage->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
//	m_pOK->SetFont( pScheme->GetFont( "WritingFont" ) );
//	m_pOK->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
//	m_pOK->SetBorder( pScheme->GetBorder( "ButtonBorder" ) );

	Reset();
}

CMSPlayerStatsMenu::~CMSPlayerStatsMenu()
{
//	g_pFullFileSystem->RemoveFile( TEMP_HTML_FILE, "DEFAULT_WRITE_PATH" );
}
/*
void CMSJoinMarquis::PaintBackground()
{
	int wide, tall;
	GetSize( wide, tall );

//	DrawRoundedBackground( m_bgColor, wide, tall );
}

void CMSJoinMarquis::PaintBorder()
{
	int wide, tall;
	GetSize( wide, tall );

//	DrawRoundedBorder( m_borderColor, wide, tall );
}
*/
void CMSPlayerStatsMenu::Reset( void )
{
//	Q_strcpy( m_szTitle, "DEFAULT TITLE" );
//	Q_strcpy( m_szMessage, "DEFAULT MESSAGE" );
//	m_szExitCommand[0] = 0;
//	m_nContentType = TYPE_TEXT;
	Update();
}
/*
void CMSPlayerStatsMenu::ShowText( const char *text)
{
//	m_pTextMessage->SetVisible( true );
//	m_pTextMessage->SetText( text );
//	m_pTextMessage->GotoTextStart();
}

void CMSPlayerStatsMenu::ShowURL( const char *URL)
{
//	m_pHTMLMessage->SetVisible( true );
//	m_pHTMLMessage->OpenURL( URL, NULL, false );
}

void CMSPlayerStatsMenu::ShowIndex( const char *entry)
{
	const char *data = NULL;
	int length = 0;

	if ( NULL == g_pStringTableInfoPanel )
		return;

	int index = g_pStringTableInfoPanel->FindStringIndex( m_szMessage );
		
	if ( index != ::INVALID_STRING_INDEX )
		data = (const char *)g_pStringTableInfoPanel->GetStringUserData( index, &length );

	if ( !data || !data[0] )
		return; // nothing to show

//	Warning("BOXBOX DEBUG: JOIN MARQUIS: DATA FOUND: %s\n", data );


	// is this a web URL ?
	if ( !Q_strncmp( data, "http://", 7 ) )
	{
		ShowURL( data );
		return;
	}

	// try to figure out if this is HTML or not
	if ( data[0] != '<' )
	{
		ShowText( data );
		return;
	}

	// data is a HTML, we have to write to a file and then load the file
	FileHandle_t hFile = g_pFullFileSystem->Open( TEMP_HTML_FILE, "wb", "DEFAULT_WRITE_PATH" );

	if ( hFile == FILESYSTEM_INVALID_HANDLE )
		return;

	g_pFullFileSystem->Write( data, length, hFile );
	g_pFullFileSystem->Close( hFile );

	if ( g_pFullFileSystem->Size( TEMP_HTML_FILE ) != (unsigned int)length )
		return; // something went wrong while writing

	ShowFile( TEMP_HTML_FILE );
}

void CMSPlayerStatsMenu::ShowFile( const char *filename )
{
	if  ( Q_stristr( filename, ".htm" ) || Q_stristr( filename, ".html" ) )
	{
		// it's a local HTML file
		char localURL[ _MAX_PATH + 7 ];
		Q_strncpy( localURL, "file://", sizeof( localURL ) );
		
		char pPathData[ _MAX_PATH ];
		g_pFullFileSystem->GetLocalPath( filename, pPathData, sizeof(pPathData) );
		Q_strncat( localURL, pPathData, sizeof( localURL ), COPY_ALL_CHARACTERS );

		ShowURL( localURL );
	}
	else
	{
		// read from local text from file
		FileHandle_t f = g_pFullFileSystem->Open( m_szMessage, "rb", "GAME" );

		if ( !f )
			return;

		char buffer[2048];
			
		int size = min( g_pFullFileSystem->Size( f ), sizeof(buffer)-1 ); // just allow 2KB

		g_pFullFileSystem->Read( buffer, size, f );
		g_pFullFileSystem->Close( f );

		buffer[size]=0; //terminate string

		ShowText( buffer );
	}

}
*/

void CMSPlayerStatsMenu::Update( void )
{
//	SetTitle( m_szTitle, false );

//	m_pServerNameLabel->SetText( m_szTitle );

//	m_pHTMLMessage->SetVisible( false );
//	m_pTextMessage->SetVisible( false );
/*
	if ( m_nContentType == TYPE_INDEX )
	{
		ShowIndex( m_szMessage );
	}
	else if ( m_nContentType == TYPE_URL )
	{
		ShowURL( m_szMessage );
	}
	else if ( m_nContentType == TYPE_FILE )
	{
		ShowFile( m_szMessage );
	}
	else if ( m_nContentType == TYPE_TEXT )
	{
		ShowText( m_szMessage );
	}
	else
	{
		DevMsg("CMOTDPanel::Update: unknown content type %i\n", m_nContentType );
	}
*/
//	MoveToCenterOfScreen();
}
/*
void CMSJoinMarquis::MoveToCenterOfScreen()
{
	int wx, wy, ww, wt;
	surface()->GetWorkspaceBounds(wx, wy, ww, wt);
	SetPos((ww - GetWide()) / 2, (wt - GetTall()) / 2);
}
*/
void CMSPlayerStatsMenu::OnCommand( const char *command )
{
/*    if (!Q_strcmp(command, "JoinClose"))
    {
		if ( m_szExitCommand[0] )
		{
			engine->ClientCmd( m_szExitCommand );
		}
		
		m_pViewPort->ShowPanel( this, false );
		vgui::surface()->PlaySound( "UI/pageturn.wav" );

		if( m_bJustJoined )
			m_pViewPort->ShowPanel( PANEL_CHARSELECT, true );
		m_bJustJoined = false;
	}

	BaseClass::OnCommand(command);
*/
}
/*
void CMSPlayerStatsMenu::SetData(KeyValues *data)
{
	SetData( data->GetInt( "type" ), data->GetString( "title"), data->GetString( "msg" ), data->GetString( "cmd" ), data->GetBool( "justjoined" ) );
}

void CMSPlayerStatsMenu::SetData( int type, const char *title, const char *message, const char *command, bool bJustJoined )
{
	Q_strncpy(  m_szTitle, title, sizeof( m_szTitle ) );
	Q_strncpy(  m_szMessage, message, sizeof( m_szMessage ) );
	
	if ( command )
	{
		Q_strncpy( m_szExitCommand, command, sizeof(m_szExitCommand) );
	}
	else
	{
		m_szExitCommand[0]=0;
	}

	m_nContentType = type;
	m_bJustJoined = bJustJoined; // BOXBOX added
	Update();

}
*/
void CMSPlayerStatsMenu::ShowPanel( bool bShow )
{
//	if ( BaseClass::IsVisible() == bShow )
//		return;

//	m_pViewPort->ShowBackGround( bShow );

	if ( bShow )
	{
		Activate();
		MoveToFront();
	}
	else
	{
		SetVisible( false );
	}
}










/*
#include "cbase.h"
#include <stdio.h>
#include <string>

//#include <cdll_client_int.h> // BOXBOX added
#include "MSSPlayerStatsMenu.h"

#include <vgui_controls/Label.h>
#include <vgui/ISurface.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;
//using namespace std;


enum {HUMAN = 0, ELF, DWARF};


CMSPlayerStatsMenu::CMSPlayerStatsMenu(IViewPort *pViewPort) : Frame( NULL, PANEL_PLAYERSTATS )
{
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/MSScheme.res", "MSScheme"); // BOXBOX added
	SetScheme(scheme);

	m_pViewPort = pViewPort;

	SetMoveable( false );
	SetSizeable( false );
	SetProportional( true );

	SetMouseInputEnabled( true );
	SetKeyBoardInputEnabled( false );

	SetTitleBarVisible( false );

//	title = new vgui::Label(this, "title", "");
	generalInfoTitle = new vgui::Label(this, "generalInfoTitle", "");
	generalInfo = new vgui::Label(this, "generalInfo", "");
	statsTitle = new vgui::Label(this, "statsTitle", "");
	stats = new vgui::Label(this, "stats", "");
	skillsTitle = new vgui::Label(this, "skillsTitle", "");
	skills = new vgui::Label(this, "skills", "");

}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CMSPlayerStatsMenu::~CMSPlayerStatsMenu()
{
	
}


void CMSPlayerStatsMenu::ApplySchemeSettings( IScheme *pScheme ) // BOXBOX added this
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSPlayerStatsMenu.res");

//	m_pTitleLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
//	m_pTitleLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
}


//-----------------------------------------------------------------------------
// Purpose: clears everything in the stats window, then generates the screen
//			from the latest player info
//-----------------------------------------------------------------------------
void CMSPlayerStatsMenu::Reset()
{
	// Sets up fonts and colors used
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	vgui::IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme());
	HFont titleFont = pScheme->GetFont("HeaderFont");
	HFont sectionFont = pScheme->GetFont("WritingFont");
	HFont fieldFont = pScheme->GetFont("WritingFontSmall");
	vgui::IBorder *border = pScheme->GetBorder("MSStatsBorder"); // BOXBOX TODO change
	SetFgColor(Color(255, 255, 255, 255));
	Color* white = new Color(255, 255, 255, 255);
	char conversion[10];

	// Sets the border
	SetBorder(border);

	// Title for the window
	title->SetText("Character Information");
	title->SetPos(20, 20);
	title->SetSize(150, 20);
	title->SetFont(titleFont);
	title->SetFgColor(*white);
	title->Repaint();

	// Title for the general information section
	generalInfoTitle->SetText("General Information");
	generalInfoTitle->SetPos(25, 45);
	generalInfoTitle->SetSize(150, 20);
	generalInfoTitle->SetFont(sectionFont);
	generalInfoTitle->SetFgColor(*white);
	generalInfoTitle->Repaint();

	// Fields for the general information section
	string generalInfoContents = "Name: ";
	generalInfoContents.append(pPlayer->GetMSPlayerName());
	generalInfoContents.append("\nGender: ");
	if (pPlayer->GetMSGender())
		generalInfoContents.append("Male");
	else
		generalInfoContents.append("Female");
	if (pPlayer->GetMSRace() == HUMAN)
		generalInfoContents.append("\nHuman ");
	else if (pPlayer->GetMSRace() == ELF)
		generalInfoContents.append("\nElf ");
	else if (pPlayer->GetMSRace() == DWARF)
		generalInfoContents.append("\nDwarf ");
	generalInfoContents.append("<Insert title here>");
	generalInfoContents.append("\nAlliance: ");
	if (pPlayer->GetMSAlliance() >= 0 && pPlayer->GetMSAlliance() <= 3)
		generalInfoContents.append("Evil");
	else if (pPlayer->GetMSAlliance() >= 4 && pPlayer->GetMSAlliance() <= 7)
		generalInfoContents.append("Unlawful");
	else if (pPlayer->GetMSAlliance() >= 8 && pPlayer->GetMSAlliance() <= 12)
		generalInfoContents.append("Neutral");
	else if (pPlayer->GetMSAlliance() >= 13 && pPlayer->GetMSAlliance() <= 16)
		generalInfoContents.append("Lawful");
	else
		generalInfoContents.append("Hero");
	generalInfoContents.append("\n\nGold: ");
	generalInfoContents.append(itoa(pPlayer->GetMSGold(), conversion, 10));
	generalInfoContents.append("\nPlayer Kills: ");
	generalInfoContents.append(itoa(pPlayer->GetMSPlayerKills(), conversion, 10));
	generalInfo->SetText(generalInfoContents.c_str());
	generalInfo->SetPos(30, 70);
	generalInfo->SetSize(170, 125);
	generalInfo->SetFont(fieldFont);
	generalInfo->SetFgColor(*white);
	generalInfo->Repaint();

	// Title for the stats section
	statsTitle->SetText("Stats");
	statsTitle->SetPos(25, 215);
	statsTitle->SetSize(150, 20);
	statsTitle->SetFont(sectionFont);
	statsTitle->SetFgColor(*white);
	statsTitle->Repaint();

	// Fields for the stats section
	string statsContents = "Strength: ";
	statsContents.append(itoa(pPlayer->GetMSStrength(), conversion, 10));
	statsContents.append("\nDexterity: ");
	statsContents.append(itoa(pPlayer->GetMSDexterity(), conversion, 10));
	statsContents.append("\nConcentration: ");
	statsContents.append(itoa(pPlayer->GetMSConcentration(), conversion, 10));
	statsContents.append("\nAwareness: ");
	statsContents.append(itoa(pPlayer->GetMSAwareness(), conversion, 10));
	statsContents.append("\nFitness: ");
	statsContents.append(itoa(pPlayer->GetMSFitness(), conversion, 10));
	stats->SetText(statsContents.c_str());
	stats->SetPos(30, 240);
	stats->SetSize(170, 105);
	stats->SetFont(fieldFont);
	stats->SetFgColor(*white);
	stats->Repaint();

	// Title for the skills section
	skillsTitle->SetText("Skills");
	skillsTitle->SetPos(225, 45);
	skillsTitle->SetSize(150, 20);
	skillsTitle->SetFont(sectionFont);
	skillsTitle->SetFgColor(*white);
	skillsTitle->Repaint();

	// Fields for the skills section
	string skillsContents = "Warrior Skills: ";
	skillsContents.append(itoa(pPlayer->GetMSWarriorSkills(), conversion, 10));
	skillsContents.append(" (");
	skillsContents.append(itoa(pPlayer->GetMSWarriorSkillsExpPercent(), conversion, 10));
	skillsContents.append("%)\nMartial Arts: ");
	skillsContents.append(itoa(pPlayer->GetMSMartialArts(), conversion, 10));
	skillsContents.append(" (");
	skillsContents.append(itoa(pPlayer->GetMSMartialArtsExpPercent(), conversion, 10));
	skillsContents.append("%)\nSmall Arms: ");
	skillsContents.append(itoa(pPlayer->GetMSSmallArms(), conversion, 10));
	skillsContents.append(" (");
	skillsContents.append(itoa(pPlayer->GetMSSmallArmsExpPercent(), conversion, 10));
	skillsContents.append("%)\nArchery: ");
	skillsContents.append(itoa(pPlayer->GetMSArchery(), conversion, 10));
	skillsContents.append(" (");
	skillsContents.append(itoa(pPlayer->GetMSArcheryExpPercent(), conversion, 10));
	skillsContents.append("%)\nSpell Casting: ");
	skillsContents.append(itoa(pPlayer->GetMSSpellCasting(), conversion, 10));
	skillsContents.append(" (");
	skillsContents.append(itoa(pPlayer->GetMSSpellCastingExpPercent(), conversion, 10));
	skillsContents.append("%)\nParry: ");
	skillsContents.append(itoa(pPlayer->GetMSParry(), conversion, 10));
	skillsContents.append(" (");
	skillsContents.append(itoa(pPlayer->GetMSParryExpPercent(), conversion, 10));
	skillsContents.append("%)");
	skills->SetText(skillsContents.c_str());
	skills->SetPos(230, 65);
	skills->SetSize(170, 150);
	skills->SetFont(fieldFont);
	skills->SetFgColor(*white);
	skills->Repaint();

}

//-----------------------------------------------------------------------------
// Purpose: Displays the window
//-----------------------------------------------------------------------------
void CMSPlayerStatsMenu::ShowPanel(bool bShow)
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	m_pViewPort->ShowBackGround( bShow );

	if ( bShow )
	{
		Activate();
		SetMouseInputEnabled( true );
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

bool CMSPlayerStatsMenu::NeedsUpdate( void )
{
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSPlayerStatsMenu::Update( void )
{
//	SetPos(15, 15);
	//MoveToCenterOfScreen();
}
*/