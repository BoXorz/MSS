//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef CMSSClientScoreBoardDialog_H
#define CMSSClientScoreBoardDialog_H
#ifdef _WIN32
#pragma once
#endif

#include <clientscoreboarddialog.h>

//-----------------------------------------------------------------------------
// Purpose: Game ScoreBoard
//-----------------------------------------------------------------------------
class CMSSClientScoreBoardDialog : public CClientScoreBoardDialog
{
private:
	DECLARE_CLASS_SIMPLE(CMSSClientScoreBoardDialog, CClientScoreBoardDialog);
	
public:
	CMSSClientScoreBoardDialog(IViewPort *pViewPort);
	~CMSSClientScoreBoardDialog();


protected:
	// scoreboard overrides
	virtual void InitScoreboardSections();
//	virtual void UpdateTeamInfo();
	virtual bool GetPlayerScoreInfo(int playerIndex, KeyValues *outPlayerInfo);
	virtual void UpdatePlayerInfo();

	// vgui overrides for rounded corner background
	virtual void PaintBackground();
	virtual void PaintBorder();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
	virtual void AddHeader(); // add the start header of the scoreboard
//	virtual void AddSection(int teamType, int teamNumber); // add a new section header for a team

//	int GetSectionFromTeamNumber( int teamNumber );

	enum 
	{ 
		CSTRIKE_NAME_WIDTH = 320,
		CSTRIKE_CLASS_WIDTH = 56,
		CSTRIKE_SCORE_WIDTH = 40,
		CSTRIKE_DEATH_WIDTH = 46,
		CSTRIKE_PING_WIDTH = 46,
//		CSTRIKE_VOICE_WIDTH = 40, 
//		CSTRIKE_FRIENDS_WIDTH = 24,
	};

	// rounded corners
	Color					 m_bgColor;
	Color					 m_borderColor;
};


#endif // CMSSClientScoreBoardDialog_H
