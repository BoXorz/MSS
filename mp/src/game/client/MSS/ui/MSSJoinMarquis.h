
#ifndef MSSJOINPANEL_H
#define MSSJOINPANEL_H
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


class CMSJoinMarquis : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CMSJoinMarquis, vgui::Frame );

//	Color					 m_bgColor;
//	Color					 m_borderColor;

public:
	CMSJoinMarquis(IViewPort *pViewPort);
	virtual ~CMSJoinMarquis();

	virtual const char *GetName( void ) { return PANEL_JOIN; }
	virtual void SetData(KeyValues *data);
	virtual void SetData( int type, const char *title, const char *message, const char *command, bool bJustJoined ); // BOXBOX added boolean
	virtual void Reset();
	virtual void Update();
//	virtual void MoveToCenterOfScreen();

	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
  	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

	virtual void ShowFile( const char *filename);
	virtual void ShowText( const char *text);
	virtual void ShowURL( const char *URL);
	virtual void ShowIndex( const char *entry);

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

protected:	
	// vgui overrides
	virtual void OnCommand( const char *command);

//	virtual void PaintBackground();
//	virtual void PaintBorder();

	IViewPort	*m_pViewPort;
	char		m_szTitle[255];
	char		m_szMessage[2048];
	char		m_szExitCommand[255];
	int			m_nContentType;

	vgui::Label		*m_pServerNameLabel;
	vgui::TextEntry	*m_pTextMessage;
//	vgui::HTML		*m_pHTMLMessage;
//	vgui::Button	*m_pOK;

	bool m_bJustJoined; // BOXBOX added
};


#endif // MSSJOINPANEL_H
