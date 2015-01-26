
#ifndef MSSITEMENFO_H
#define MSSITEMINFO_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
//#include <vgui_controls/Button.h>
//#include <vgui_controls/HTML.h>

#include <game/client/iviewport.h>
#include "shareddefs.h"
#include "basemodelpanel.h"

class CMSItemInfoSheet : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CMSItemInfoSheet, vgui::Frame );

public:
	CMSItemInfoSheet(IViewPort *pViewPort);
	virtual ~CMSItemInfoSheet();

	virtual const char *GetName( void ) { return PANEL_ITEMINFO; }
	virtual void SetData(KeyValues *data) { return; }
//	virtual void SetData( int type, const char *title, const char *message, const char *command);
	virtual void Reset();
	virtual void Update();
//	virtual void MoveToCenterOfScreen();

	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return false; }

//	const FileItemInfo_t	&GetItemData( ITEM_FILE_INFO_HANDLE item ) const;
	void SetItemToDisplay( ITEM_FILE_INFO_HANDLE item );
	virtual void ShowPanel( bool bShow );

	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
  	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

protected:	

//	virtual void OnCommand( const char *command);

	IViewPort	*m_pViewPort;

//	ITEM_FILE_INFO_HANDLE	m_nItem; // BOXBOX Index into item to display

	vgui::Label			*m_pItemNameLabel;
	vgui::TextEntry		*m_pDescription;
//	vgui::Label			*m_pItemDescriptionLabel;

	vgui::Label			*m_pItemTypeLabel;
	vgui::Label			*m_pPropertyLabel;
	vgui::Label			*m_pValueLabel;
//	vgui::Label			*m_pGoldValueLabel;


//	vgui::Label			*m_pMinDamageLabel;
//	vgui::Label			*m_pDashLabel;
//	vgui::Label			*m_pMaxDamageLabel;

	CModelPanel			*m_pItemModel;
};


#endif // MSSITEMINFO_H
