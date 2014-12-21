
#ifndef MSS_SHAREDDEFS_H
#define MSS_SHAREDDEFS_H

#ifdef _WIN32
#pragma once
#endif

#include "const.h"


#define MAX_CHAR_SLOTS			3
#define MAX_CHAR_NAME_LENGTH	32

#define SEX_MALE		0
#define SEX_FEMALE		1

enum MSSRaces_t
{
	RACE_HUMAN,
	RACE_DWARF,
	RACE_ELF,
};

#endif // MSS_SHAREDDEFS_H
