
// BOXBOX The MSS Server Information (MOTD) Panel

#include "cbase.h"
#include "MSSJoinMarquis.h"
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

CON_COMMAND( serverinfo, "Shows the Join Marquis" ) // BOXBOX In case a player wants to see the server info again during play
{
	if ( !gViewPortInterface )
		return;
	
//	if ( args.ArgC() != 2 )
//		return;
		
	 gViewPortInterface->ShowPanel( PANEL_JOIN, true );
}

CMSJoinMarquis::CMSJoinMarquis(IViewPort *pViewPort) : Frame( NULL, PANEL_JOIN )
{
	// initialize dialog
	m_pViewPort = pViewPort;

//	SetTitle("", true);

	m_szTitle[0] = '\0';
	m_szMessage[0] = '\0';
	m_szExitCommand[0] = '\0';
	
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/MSScheme.res", "MSScheme");
	SetScheme(scheme);

	SetMoveable(false);
	SetSizeable(false);
	SetProportional(true);
	SetTitleBarVisible( false );

	m_pServerNameLabel	= new Label( this, "ServerName", "Server Name" );
	m_pTextMessage		= new TextEntry(this, "TextMessage");
//	m_pHTMLMessage		= new HTML(this,"HTMLMessage");;

//	m_pOK = new Button(this, "ok", "#MSS_TURNPAGE");
//	m_pOK->SetCommand("JoinClose");

	m_pTextMessage->SetMultiline( true );
	m_nContentType = TYPE_TEXT;
	m_bJustJoined = false; // BOXBOX added
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMSJoinMarquis::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/MSJoinMarquis.res");

//	m_bgColor = GetSchemeColor("BgColor", GetBgColor(), pScheme);
//	m_borderColor = pScheme->GetColor( "MSBorderColor", Color( 0, 0, 0, 0 ) );

	m_pServerNameLabel->SetFont( pScheme->GetFont( "HeaderFont" ) );
	m_pServerNameLabel->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
	m_pTextMessage->SetFont( pScheme->GetFont( "WritingFont" ) );
	m_pTextMessage->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
//	m_pOK->SetFont( pScheme->GetFont( "WritingFont" ) );
//	m_pOK->SetFgColor( pScheme->GetColor( "InkWell", Color(0, 0, 0, 0) ) );
//	m_pOK->SetBorder( pScheme->GetBorder( "ButtonBorder" ) );

	Reset();
}

CMSJoinMarquis::~CMSJoinMarquis()
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
void CMSJoinMarquis::Reset( void )
{
	Q_strcpy( m_szTitle, "DEFAULT TITLE" );
	Q_strcpy( m_szMessage, "DEFAULT MESSAGE" );
	m_szExitCommand[0] = 0;
	m_nContentType = TYPE_TEXT;
	Update();
}

void CMSJoinMarquis::ShowText( const char *text)
{
	m_pTextMessage->SetVisible( true );
	m_pTextMessage->SetText( text );
	m_pTextMessage->GotoTextStart();
}

void CMSJoinMarquis::ShowURL( const char *URL)
{
//	m_pHTMLMessage->SetVisible( true );
//	m_pHTMLMessage->OpenURL( URL, NULL, false );
}

void CMSJoinMarquis::ShowIndex( const char *entry)
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

void CMSJoinMarquis::ShowFile( const char *filename )
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

void CMSJoinMarquis::Update( void )
{
	SetTitle( m_szTitle, false );

	m_pServerNameLabel->SetText( m_szTitle );

//	m_pHTMLMessage->SetVisible( false );
	m_pTextMessage->SetVisible( false );

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
void CMSJoinMarquis::OnCommand( const char *command )
{
    if (!Q_strcmp(command, "JoinClose"))
    {
		if ( m_szExitCommand[0] )
		{
			engine->ClientCmd( m_szExitCommand );
		}
		
		m_pViewPort->ShowPanel( this, false );
		vgui::surface()->PlaySound( "UI/pageturn.wav" );

		if( m_bJustJoined )
			m_pViewPort->ShowPanel( PANEL_CHARSELECT, true ); // BOXBOX Open character selection menu upon closing this.
		m_bJustJoined = false;
	}

	BaseClass::OnCommand(command);
}

void CMSJoinMarquis::SetData(KeyValues *data)
{
	SetData( data->GetInt( "type" ), data->GetString( "title"), data->GetString( "msg" ), data->GetString( "cmd" ), data->GetBool( "justjoined" ) );
}

void CMSJoinMarquis::SetData( int type, const char *title, const char *message, const char *command, bool bJustJoined )
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

void CMSJoinMarquis::ShowPanel( bool bShow )
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

