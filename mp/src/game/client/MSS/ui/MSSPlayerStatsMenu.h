
#ifndef MS_PLAYERSTATSMENU_H
#define MS_PLAYERSTATSMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/HTML.h>

#include <game/client/iviewport.h>
#include "shareddefs.h"

namespace vgui
{
	class TextEntry;
}


class CMSPlayerStatsMenu : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CMSPlayerStatsMenu, vgui::Frame );

//	Color					 m_bgColor;
//	Color					 m_borderColor;

public:
	CMSPlayerStatsMenu(IViewPort *pViewPort);
	virtual ~CMSPlayerStatsMenu();

	virtual const char *GetName( void ) { return PANEL_PLAYERSTATS; }
	virtual void SetData(KeyValues *data) { return; }
//	virtual void SetData( int type, const char *title, const char *message, const char *command, bool bJustJoined ); // BOXBOX added boolean
	virtual void Reset();
	virtual void Update();
//	virtual void MoveToCenterOfScreen();

	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return false; }
	virtual void ShowPanel( bool bShow );

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
  	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

//	virtual void ShowFile( const char *filename);
//	virtual void ShowText( const char *text);
//	virtual void ShowURL( const char *URL);
//	virtual void ShowIndex( const char *entry);

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

protected:	
	// vgui overrides
	virtual void OnCommand( const char *command);

//	virtual void PaintBackground();
//	virtual void PaintBorder();

	IViewPort	*m_pViewPort;
//	char		m_szTitle[255];
//	char		m_szMessage[2048];
//	char		m_szExitCommand[255];
//	int			m_nContentType;

//	vgui::Label		*m_pServerNameLabel;
//	vgui::TextEntry	*m_pTextMessage;
//	vgui::HTML		*m_pHTMLMessage;
//	vgui::Button	*m_pOK;

//	bool m_bJustJoined; // BOXBOX added
};


#endif // MS_PLAYERSTATSMENU_H





/*
#ifndef MS_PLAYERSTATSMENU_H
#define MS_PLAYERSTATSMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <game/client/iviewport.h>
#include "ienginevgui.h" // BOXBOX added

//-----------------------------------------------------------------------------
// Purpose: Player Stats Page
//-----------------------------------------------------------------------------
class CMSPlayerStatsMenu : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CMSPlayerStatsMenu, vgui::Frame );

public:
	CMSPlayerStatsMenu( IViewPort *pViewPort );
	virtual ~CMSPlayerStatsMenu();

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme ); // BOXBOX added this

	virtual const char *GetName( void ) { return PANEL_PLAYERSTATS; }
	virtual void SetData(KeyValues *data) {};
	virtual void Reset();
	virtual void Update();
	virtual bool NeedsUpdate( void );
	virtual bool HasInputElements( void ) { return false; }
	virtual void ShowPanel( bool bShow );

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
  	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }		


protected:

	IViewPort	*m_pViewPort;

	//	vgui::Label		*m_pTitleLabel;
	vgui::Label* generalInfoTitle;
	vgui::Label* statsTitle;
	vgui::Label* skillsTitle;
	vgui::Label* generalInfo;
	vgui::Label* stats;
	vgui::Label* skills;
};


#endif // MS_PLAYERSTATSMENU_H
*/