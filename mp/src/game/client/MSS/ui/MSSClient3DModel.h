//3d model panel
#if !defined( VGUI_CLIENT3DMODEL_H )
#define VGUI_CLIENT3DMODEL_H

#include <vgui_controls/ImagePanel.h>
#include "vcollide_parse.h"
#include "c_MSS_player.h"
#include "view.h"
#include "takedamageinfo.h"
#include "MSS_gamerules.h"
#include "in_buttons.h"
#include "iviewrender_beams.h"			// flashlight beam
#include "r_efx.h"
#include "dlight.h"

#include "view_shared.h"
#include "model_types.h"

class CClientSidePlayerModel : public CBaseAnimatingOverlay
{
public:

	CBaseAnimating *m_pWeapon;
	char m_ModelName[256];
	int m_SlotIndex;				//Char Slot
	bool m_Hidden;

	static CUtlVector<EHANDLE> g_ClientSideModels;
	static void UpdateEntities( );
	static void UpdateEntity( CClientSidePlayerModel *pPlayerModel );
};

#endif
