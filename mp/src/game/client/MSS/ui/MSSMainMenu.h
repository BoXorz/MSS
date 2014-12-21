
#ifndef MSSMAINMENU_H
#define MSSMAINMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>
//#include <vgui_controls/HTML.h>

#include "MSSPlayerStatsMenu.h"

#include <game/client/iviewport.h>
#include "shareddefs.h"

/*namespace vgui
{
	class TextEntry;
}
*/

class CMSMainMenu : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CMSMainMenu, vgui::Frame );

//	Color					 m_bgColor;
//	Color					 m_borderColor;

public:
	CMSMainMenu(IViewPort *pViewPort);
	virtual ~CMSMainMenu();

	virtual const char *GetName( void ) { return PANEL_MAINMENU; }
	virtual void SetData(KeyValues *data) { return; }
//	virtual void SetData( int type, const char *title, const char *message, const char *command);
	virtual void Reset();
	virtual void Update();
//	virtual void MoveToCenterOfScreen();

	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return false; }	// BOXBOX someof the child panels will though
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

	virtual void OnCommand( const char *command);

	void PlayPageTurnSound( void );
	void HideAllChildControls( void );

	IViewPort	*m_pViewPort;
	int m_nCurPage; // BOXBOX what page are we on now (1-2, 3-4, etc)

	vgui::Label			*m_pLeftPageNumLabel;
	vgui::Label			*m_pRightPageNumLabel;

	vgui::Label			*m_pLeftPageTitleLabel;
	vgui::Label			*m_pRightPageTitleLabel;

	vgui::Label			*m_pSexLabel;
	vgui::Label			*m_pRaceLabel;
	vgui::Label			*m_pTotalExpText;
	vgui::Label			*m_pTotalExpLabel;

};


#endif // MSSMAINMENU_H
