
#ifndef MSS_SHAREDDEFS_H
#define MSS_SHAREDDEFS_H

#ifdef _WIN32
#pragma once
#endif

#include "const.h"

#define MAX_CHAR_NAME_LENGTH	32

#define MAX_CHAR_SLOTS			3
enum MSSCharSlot_t
{
	CHARSLOT_INVALID,
	CHARSLOT_ONE,
	CHARSLOT_TWO,
	CHARSLOT_THREE,
};

enum MSSGenderIndex_t
{
	MSS_GENDER_INVALID,
	MSS_GENDER_MALE,
	MSS_GENDER_FEMALE,
};
#define GENDER_NONE		GENDER_INVALID

enum MSSRaceIndex_t
{
	RACE_INVALID,
	RACE_HUMAN,
	RACE_DWARF,
	RACE_ELF,
};

enum MSSModelIndex_t
{
	MODEL_INVALID,
	MODEL_HUMANMALE,
	MODEL_HUMANFEMALE,
	MODEL_DWARFMALE,
	MODEL_DWARFFEMALE,
	MODEL_ELFMALE,
	MODEL_ELFFEMALE,
};

#define	MAX_WEAPON_SKILLS	9

enum MSSWeaponType_t
{
	WEAPONTYPE_UNARMED,			// No weapon. .. or Martial Arts weapons like knuckles
	WEAPONTYPE_ONEHANDPIERCING,	// Small piercing weapons like daggers
	WEAPONTYPE_ONEHANDSLASHING,	// One handed slashing weapons like swords/axes
	WEAPONTYPE_ONEHANDBASHING,	// One handed bashing weapons like hammers/maces
	WEAPONTYPE_TWOHANDPIERCING,	// Large piercing weapons like spears
	WEAPONTYPE_TWOHANDSLASHING,	// Two handed slashing weapons like greatswords/greataxes
	WEAPONTYPE_TWOHANDBASHING,	// Two handed bashing weapons like warhammers/morning stars
	WEAPONTYPE_ARCHERY,
};

#define WEAPONTYPE_THROWN	8 // BOXBOX when a player throws a weapon, any weapon, this exp increases


enum MSSWeaponSkillLevels_t
{
	SKILL_INVALID,		// 0
	SKILL_NOVICE,
	SKILL_AWKWARD,
	SKILL_ABLE,
	SKILL_ADEPT,
	SKILL_SKILLED,		// 5
	SKILL_JOURNEYMAN,
	SKILL_EXPERT,
	SKILL_MASTER,
	SKILL_GRANDMASTER,
	SKILL_LUMINARY,		// 10
};
#define SKILL_NUM_LEVELS	10

// BOXBOX how much exp is required for each skill level
#define	EXP_NOVICE		0
#define	EXP_AWKWARD		100
#define	EXP_ABLE		500
#define	EXP_ADEPT		2500
#define	EXP_SKILLED		9000
#define	EXP_JOURNEYMAN	21000
#define	EXP_EXPERT		35000
#define	EXP_MASTER		50000
#define	EXP_GRANDMASTER	70000
#define EXP_LUMINARY	100000	








#endif // MSS_SHAREDDEFS_H
