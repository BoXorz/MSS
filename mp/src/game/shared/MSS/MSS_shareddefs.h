
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

enum MSSRaceIndex_t
{
	RACE_INVALID,
	RACE_HUMAN,
	RACE_DWARF,
	RACE_ELF,
};

enum MSSPlayerModelIndex_t
{
	MODEL_INVALID,
	MODEL_HUMANMALE,
	MODEL_HUMANFEMALE,
	MODEL_DWARFMALE,
	MODEL_DWARFFEMALE,
	MODEL_ELFMALE,
	MODEL_ELFFEMALE,
};

enum MSSCoreStats_t
{
	STAT_INVALID,
	STAT_MIGHT,		// Physical Strength
	STAT_AGILITY,	// Physical Speed
	STAT_STAMINA,	// Physical Endurance
	STAT_INTELLECT,	// Mental Strength
	STAT_WIT,		// Mental Speed
	STAT_FORTITUDE,	// Mental Endurance
	STAT_LUCK,
};

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
	WEAPONTYPE_THROWN,			// BOXBOX when a player throws a weapon, any weapon, this exp increases
};
#define	MAX_WEAPON_SKILLS	9

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

// BOXBOX how much exp is required for each SKILL level
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

// BOXBOX how much exp is required for each CRAFT level
#define	CRAFT_NOVICE		0
#define	CRAFT_AWKWARD		10
#define	CRAFT_ABLE			50
#define	CRAFT_ADEPT			250
#define	CRAFT_SKILLED		900
#define	CRAFT_JOURNEYMAN	2100
#define	CRAFT_EXPERT		3500
#define	CRAFT_MASTER		5000
#define	CRAFT_GRANDMASTER	7000
#define CRAFT_LUMINARY		10000	

enum MSSCraftSkills_t
{
	CRAFT_INVALID,
	CRAFT_ALCHEMY,
	CRAFT_CLOTHWORK,
	CRAFT_WOODWORK,
	CRAFT_STONEWORK,
	CRAFT_METALWORK,
};
#define	MAX_CRAFT_SKILLS	6

enum MSSMenuPages_t
{
	MENUPAGE_INVALID,		// 0
	MENUPAGE_STATS,
	MENUPAGE_INVENTORY,
	MENUPAGE_POWERUP,
};

#define BACKPACK_SLOTS_X	10
#define BACKPACK_SLOTS_Y	10

enum MSSBackpacks_t
{
	BACKPACK_INVALID,
	BACKPACK_TINY,		// 10 slots
	BACKPACK_MEAGER,	// 20 slots
	BACKPACK_SMALL,		// 30 slots
	BACKPACK_MEDIUM,	// 40
	BACKPACK_NORMAL,	// 50
	BACKPACK_TRAVELERS,	// 60
	BACKPACK_LARGE,		// 70
	BACKPACK_HUGE,		// 80
	BACKPACK_GIANT,		// 90
	BACKPACK_COLOSSAL,	// 100 slots
};

enum MSSItemTypes_t
{
	ITEMTYPE_WEAPON = -1,	// red
	ITEMTYPE_ANY,			// BOXBOX for slots that accept any type of item (backpack, belt)
	ITEMTYPE_ALCHEMY,		// green
	ITEMTYPE_MAGICWORK,		// blue
	ITEMTYPE_CLOTHWORK,		// purple
	ITEMTYPE_WOODWORK,		// brown
	ITEMTYPE_STONEWORK,		// gray
	ITEMTYPE_METALWORK,		// orange
	ITEMTYPE_ARMOR,			// yellow from here down
	ITEMTYPE_HELMET,
	ITEMTYPE_GLOVES,
	ITEMTYPE_BOOTS,
	ITEMTYPE_SHIELD,
};












#endif // MSS_SHAREDDEFS_H
