
#ifndef COMBATWEAPON_H
#define COMBATWEAPON_H
#ifdef _WIN32
#pragma once
#endif

#include "entityoutput.h"
#include "basecombatweapon_shared.h"

struct animevent_t;

extern void	SpawnBlood(Vector vecSpot, const Vector &vecDir, int bloodColor, float flDamage);

#endif // COMBATWEAPON_H
