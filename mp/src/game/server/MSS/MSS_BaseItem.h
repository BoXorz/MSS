
/* BOXBOX well here goes nothing, rebuilding the entire item/weapon code base!

Here's the current(old) class structure, using arozen sword as an example

	CBaseAnimating  - the top of the pyramid in both old and new code structure
		CBaseCombatWeapon
			CWeaponMSSBase
				CBaseMSSCombatWeapon		
					CBaseMSSBludgeonWeapon
						CWeaponArozenSword

damn, that's crazy, ok now here's the structure we want in the restructuring

	CBaseAnimating
		CMSSBaseItem  - this base class will hold the ItemFileInfo_t for all items/weapons, and everything common to all item types.
			CMSSBaseWeapon  - all melee weapons
				CMSSArcheryWeapon

*/

#include "cbase.h"