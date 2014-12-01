
/*
/////////////////////////
//
// BOXBOX MSS WEATHER SYSTEM!

The weather system ideas:
The weather should have the following states:

*** WIND STATE *** - how windy is it?
A. Calm
B. Slight breeze
C. Windy
D. Gale

The system should choose a state on map load. An attempt will be made to make the following things happen
depending on the wind state:

1)-Change the soundscape being played, or play a new one, to reflect the wind and place wind noises
appropriate to the wind state.

2)-Scour the map for detail sprites and change their sway properties to make sense for the wind state.

3)-Scour the map for models that will be affected by wind state, anything from trees to lanterns,
and have them play an idle animation that is appropriate. This means custom models will need idle
animations for all wind states.

4)- A possible alternative or addition to number 3, scour the map for physics systems and apply wind forces
to them, check to see if this is already done in some form, like how func ropes sway in the wind in hl2.


*** PRECIPITATION STATE *** - how much precipitation?  
A. Clear
B. Slight drizzle
C. Rainy/Snowy/etc.
D. Storm/Blizzard/not good outside yo

If we decide to have precipitation amounts vary dynamically in map, they should shift one category at
a time up or down the scale.

*** PRECIPITATION TYPE *** - What type of precipitation is it? Only applies if not clear weather.
A. Rain
B. Snow
C. Hail

Precipitaion type should work with the time system ( the one we haven't made yet, where the game knows
the time of day, and the passing of seasons etc)
*/

#include "cbase.h"
#include "MSS_cvars.h" // BOXBOX put weather system cvars here?



class CMS_Weather : public CBaseEntity
{
public:
	DECLARE_CLASS( CMS_Weather, CBaseEntity );

	CMS_Weather();
//	void Spawn( void );

	int GetWindType( void ) { return m_nWindType; }
	int GetPrecipitationState( void ) { return m_nPrecipitationState; }
	int GetPrecipitationType( void ) { return m_nPrecipitationType; }

	void SetWindType( int type ) { m_nWindType = type; }
	void SetPrecipitationState( int state ) { m_nPrecipitationState = state; }
	void SetPrecipitationType( int type ) { m_nPrecipitationType = type; }

private:

	int m_nWindType;
	int m_nPrecipitationState;
	int m_nPrecipitationType;
};


CMS_Weather::CMS_Weather()
{

}


/*
ConVar ms_weather_wind(
							"ms_weather_wind", 
							"0", 
							FCVAR_CHEAT,
							"wind will become given state (0=calm, 1=breezy, 2=windy, 3=gale" );

ConVar ms_weather_state(
							"ms_weather_state",
							"0",
							FCVAR_CHEAT,
							"precipitation will become given state (0=clear, 1=slight, 2=normal, 3=storm" );

ConVar ms_weather_type(
							"ms_weather_type",
							"0",
							FCVAR_CHEAT,
							"type of precipitation will be set (0=N/A, 1=rain, 2=snow, 3=hail" );
*/

 // BOXBOX change this so it's not creating a new instance every time
void CC_ms_weather_wind_f(const CCommand &args)
{
	if ( !sv_cheats->GetBool() )
		return;

	CMS_Weather *pWeather = new CMS_Weather;
	if( !pWeather ) return;

	if ( args.ArgC() >= 2 )
	{
		pWeather->SetWindType( atoi( args[ 1 ] ) );
	}
}

static ConCommand ms_weather_wind("ms_weather_wind", CC_ms_weather_wind_f, "wind will become given state (0=calm, 1=breezy, 2=windy, 3=gale", FCVAR_CHEAT);

