//Master Sword
//Created: 12/28/08
#include "cbase.h"
#include "MSSClient3DModel.h"
#include "viewrender.h"
#include "MSSCharacterSelect.h"

CUtlVector<EHANDLE> CClientSidePlayerModel::g_ClientSideModels;

void CClientSidePlayerModel::UpdateEntities( )
{
	int count = g_ClientSideModels.Count();

	for (int i=0; i< count; i++ )
	{	
		CBaseEntity *pEntity = g_ClientSideModels[i].Get( );
		if( pEntity )
		{
			CClientSidePlayerModel *pPlayerModel = (CClientSidePlayerModel *)pEntity;

			if( !pPlayerModel->m_Hidden )
				//Update the entity
				UpdateEntity( pPlayerModel );
		}

		else
		{
			//Entity no longer exists.  Remove it from list
			g_ClientSideModels.Remove( i );
			i--;
			count--; 
		}
	}
}
extern CViewRender g_DefaultViewRender;

void CClientSidePlayerModel::UpdateEntity( CClientSidePlayerModel *pPlayerModel )
{
    C_BasePlayer *pLocalPlayer = C_BasePlayer::GetLocalPlayer();

    if ( !pLocalPlayer )
        return;

	// set the weapon model and upper animation to use
    //const char *pWeaponName = "models/weapons/f2000/w_f2000.mdl";
    //const char *pWeaponSequence = "Idle_Upper_Aug";

	//C_BaseAnimatingOverlay *pPlayerModel = CClientSidePlayerModel::g_ClientSideModels[0];

    // Does the entity even exist yet?
    //bool recreatePlayer = ShouldRecreateClassImageEntity( pPlayerModel, pPlayerModel->m_ModelName );

	//if ( recreatePlayer )
    {
        // if the pointer already exists, remove it as we create a new one.
       // if ( pPlayerModel )
        //    pPlayerModel->Remove();

        // create a new instance
		//pPlayerModel = new CClientSidePlayerModel();
        //pPlayerModel->AddEffects( EF_NODRAW ); // don't let the renderer draw the model normally
		//pPlayerModel->ToggleBBoxVisualization( CBaseEntity::VISUALIZE_SURROUNDING_BOUNDS );

		// have the player stand idle
    }

	//C_BaseAnimating *pWeaponModel = g_ClassImageWeapon.Get();

    // Does the entity even exist yet?
    /*if ( pPlayerModel && recreatePlayer || ShouldRecreateClassImageEntity( pWeaponModel, pWeaponName ) )
    {
        if ( pWeaponModel )
            pWeaponModel->Remove();

        pWeaponModel = new C_BaseAnimating();
        pWeaponModel->InitializeAsClientEntity( pWeaponName, RENDER_GROUP_OPAQUE_ENTITY );
        pWeaponModel->AddEffects( EF_NODRAW ); // don't let the renderer draw the model normally
        pWeaponModel->FollowEntity( pPlayerModel ); // attach to player model
        g_ClassImageWeapon = pWeaponModel;
    }*/

	Vector origin = pLocalPlayer->EyePosition();
	
/*	CMSCharSelectMenu *pCurrentMenu = CMSCharSelectMenu::static_pCurrentMenu;
	if( pCurrentMenu && pCurrentMenu->m_CharsFollowCam )
	{
		//Chars always in front of camera
		CMSCharSelectMenu::char_selection_spawnpoint_info_s PosInfo;
		pCurrentMenu->GetCharPos( pPlayerModel->m_SlotIndex, PosInfo );

		pPlayerModel->SetAbsOrigin( PosInfo.Pos );
		pPlayerModel->SetAbsAngles( PosInfo.Rot );
	}
	else
	{
		//Chars at static position in world
	}
*/
    Vector lightOrigin = origin;

    // find a spot inside the world for the dlight's origin, or it won't illuminate the model
    Vector testPos( origin.x - 100, origin.y, origin.z + 100 );
    trace_t tr;
    UTIL_TraceLine( origin, testPos, MASK_OPAQUE, pLocalPlayer, COLLISION_GROUP_NONE, &tr );
    if ( tr.fraction == 1.0f )
    {
        lightOrigin = tr.endpos;
    }
    else
    {
        // Now move the model away so we get the correct illumination
        lightOrigin = tr.endpos + Vector( 1, 0, -1 );	// pull out from the solid
        Vector start = lightOrigin;
        Vector end = lightOrigin + Vector( 100, 0, -100 );
        UTIL_TraceLine( start, end, MASK_OPAQUE, pLocalPlayer, COLLISION_GROUP_NONE, &tr );
        origin = tr.endpos;
    }

   // float ambient = engine->GetLightForPoint( origin, true ).Length();

    // Make a light so the model is well lit.
    // use a non-zero number so we cannibalize ourselves next frame
    /*dlight_t *dl = effects->CL_AllocDlight( LIGHT_INDEX_TE_DYNAMIC+1 );

    dl->flags = DLIGHT_NO_WORLD_ILLUMINATION;
    dl->origin = lightOrigin;
    // Go away immediately so it doesn't light the world too.
    dl->die = gpGlobals->curtime + 0.1f;

    dl->color.r = dl->color.g = dl->color.b = 250;
    if ( ambient < 1.0f )
    {
        dl->color.exponent = 1 + (1 - ambient) * 2;
    }
    dl->radius	= 400;*/

	// move player model in front of our view
    //pPlayerModel->SetAbsOrigin( origin );
    //pPlayerModel->SetAbsAngles( QAngle( 0, 210, 0 ) );

    // set upper body animation
    /*pPlayerModel->m_SequenceTransitioner.UpdateCurrent(
        pPlayerModel->GetModelPtr(),
        pPlayerModel->LookupSequence( "Run_melee" ),
        pPlayerModel->GetCycle(),
        pPlayerModel->GetPlaybackRate(),
        gpGlobals->realtime	
        );*/

    // Now, blend the lower and upper (aim) anims together
   /* pPlayerModel->SetNumAnimOverlays( 2 );
    int numOverlays = pPlayerModel->GetNumAnimOverlays();
    for ( int i=0; i < numOverlays; ++i )
    {
        C_AnimationLayer *layer = pPlayerModel->GetAnimOverlay( i );
        layer->m_flCycle = pPlayerModel->GetCycle();
        if ( i )
            layer->m_nSequence = pPlayerModel->LookupSequence( "Idle_melee" );
        else
            layer->m_nSequence = pPlayerModel->LookupSequence( "Run_melee" );
        layer->m_flPlaybackRate = 1.0;
        layer->m_flWeight = 1.0f;
        layer->SetOrder( i );
    }*/
	

	if( pPlayerModel->GetSequence() > -1 )
		pPlayerModel->FrameAdvance( gpGlobals->frametime );


    Frustum dummyFrustum;
    render->Push3DView( *g_DefaultViewRender.GetViewSetup( ), 0, NULL, dummyFrustum );
	   
    pPlayerModel->DrawModel( STUDIO_RENDER );
	render->PopView( dummyFrustum );


	 // Now draw it.
    /*CViewSetup view;
    // setup the views location, size and fov (amongst others)
   // view.x = x;
   // view.y = y;
    //view.width = width;
    //view.height = height;

    view.x = 200;
    view.y = 200;
    view.width = 200;
    view.height = 200;

    view.m_bOrtho = false;
    view.fov = 54;

    view.origin = origin + Vector( -110, -5, -5 );

    // make sure that we see all of the player model
    Vector vMins, vMaxs;
    pPlayerModel->C_BaseAnimating::GetRenderBounds( vMins, vMaxs );
    view.origin.z += ( vMins.z + vMaxs.z ) * 0.55f;

    view.angles.Init();
    //view.m_vUnreflectedOrigin = view.origin;
    view.zNear = VIEW_NEARZ;
    view.zFar = 1000;
    //view.m_bForceAspectRatio1To1 = false;

    // render it out to the new CViewSetup area
    // it's possible that ViewSetup3D will be replaced in future code releases
    Frustum dummyFrustum;
    
    // New Function instead of ViewSetup3D...
    render->Push3DView( view, 0, NULL, dummyFrustum );
    
    pPlayerModel->DrawModel( STUDIO_RENDER );
*/
    //if ( pWeaponModel )
    //{
    //   pWeaponModel->DrawModel( STUDIO_RENDER );
   // }
    
    //render->PopView( dummyFrustum );
}

void MasterSword_PostRenderVGui()
{
    CClientSidePlayerModel::UpdateEntities( );
    // If the team menu is up, then render the model
    /*
	for ( int i=0; i < g_ClassImagePanels.Count(); i++ )
    {
        CClassImagePanel *pPanel = g_ClassImagePanels[i];
        if ( WillPanelBeVisible( pPanel->GetVPanel() ) )
        {
            // Ok, we have a visible class image panel.
            int x, y, w, h;
            pPanel->GetBounds( x, y, w, h );
            pPanel->LocalToScreen( x, y );

            // Allow for the border.
            x += 3;
            y += 5;
            w -= 2;
            h -= 10;

            CClientSidePlayerModel::UpdateEntities( );
            return;
        }
    }
	*/
}