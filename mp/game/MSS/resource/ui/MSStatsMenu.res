"Resource/UI/MSStatsMenu.res"
{
	"StatsMenu"
	{
		"ControlName"		"CMSStatsMenu"
		"fieldName"		"StatsMenu"
		"xpos"			"0"
		"ypos"			"0"
		"wide"			"855"
		"tall"			"480"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
	}
	"TitleBGImg"
	{
		"ControlName"		"ImagePanel"
		"xpos"			"c-415"
		"ypos"			"c-230"
		"zpos"			"0"
		"wide"			"1400"
		"tall"			"540"
		"tileImage"		"0"
		"scaleImage"		"1"
		"image"			"bookmenu"
		"border"			"border"
	}
	"LPageTitleLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"LPageTitleLabel"
		"xpos"		"c-290"
		"ypos"		"25"
		"zpos"		"1"		
		"wide"		"260"
		"tall"		"28"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"0"
		"tabPosition"		"0"
		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"center"
		"labelText"		"#MSS_PLAYERSTATS"
		"font"			"HeaderFont22"		
	}	
	"RPageTitleLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"RPageTitleLabel"
		"xpos"		"c12"
		"ypos"		"25"
		"zpos"		"1"		
		"wide"		"340"
		"tall"		"28"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"0"
		"tabPosition"		"0"
		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"center"
		"labelText"		"#ERROR"
		"font"			"HeaderFont22"		
	}	
	"LPageNumLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"LPageNumLabel"
		"xpos"		"c-190"
		"ypos"		"424"
		"zpos"		"1"
		"wide"		"60"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"0"
		"tabPosition"		"0"
		"disabledfgcolor2_override"	"InkWellLight"
		"labelText"		"1"
		"font"			"HeaderFont34"			
		"textAlignment"		"center"
	}	
	"RPageNumLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"RPageNumLabel"
		"xpos"		"c160"
		"ypos"		"424"
		"zpos"		"1"
		"wide"		"60"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"0"
		"tabPosition"		"0"
		"disabledfgcolor2_override"	"InkWellLight"
		"labelText"		"2"	
		"font"			"HeaderFont34"		
		"textAlignment"		"center"
	}
	"RPageTurnButton"
	{
		"ControlName"		"Button"
		"fieldName"		"RPageTurnButton"
		"xpos"		"c210"
		"ypos"		"424"
		"wide"		"160"
		"tall"		"28"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
		"font"			"WritingFontVerySmall"
		"defaultFgColor_override"	"InkWellLight"
		"armedBgColor_override"	"Blank"
		"labelText"		"#MSS_TURNPAGE"
		"textAlignment"		"center"
		"command"		"TurnPageRight"
		"Default"		"1"
	}
	"GenderLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"GenderLabel"
		"xpos"		"c-260"
		"ypos"		"60"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"center"
		"labelText"		"#ERROR"
		"font"			"WritingFont24"
//		"TextColor"		"RedInk"
	}
	"RaceLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"RaceLabel"
		"xpos"		"c-160"
		"ypos"		"60"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"center"
		"labelText"		"#ERROR"
		"font"			"WritingFont24"
//		"TextColor"		"RedInk"
	}
	"TotalExpText"
	{
		"ControlName"		"Label"
		"fieldName"		"TotalExpText"
		"xpos"		"c-310"
		"ypos"		"84"
		"zpos"		"1"		
		"wide"		"160"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_TOTALEXP"
		"font"			"WritingFontSmall"
//		"TextColor"		"RedInk"
	}
	"TotalExpLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"TotalExpLabel"
		"xpos"		"c-140"
		"ypos"		"83"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"west"
		"labelText"		"#ERROR"
		"font"			"WritingFont24"
//		"TextColor"		"RedInk"
	}
	"WeaponSkillsLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"WeaponSkillsLabel"
		"xpos"		"c-335"
		"ypos"		"120"
		"zpos"		"1"		
		"wide"		"140"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_WEAPONSKILLS"
		"font"			"WritingFontSmaller"
//		"TextColor"		"RedInk"
	}
	"UnarmedText"
	{
		"ControlName"		"Label"
		"fieldName"		"UnarmedText"
		"xpos"		"c-340"
		"ypos"		"135"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_UNARMED"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"UnarmedLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"UnarmedLabel"
		"xpos"		"c-235"
		"ypos"		"135"
		"zpos"		"1"		
		"wide"		"50"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"west"
		"labelText"		"#ERROR"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"OneHandPiercingText"
	{
		"ControlName"		"Label"
		"fieldName"		"OneHandPiercingText"
		"xpos"		"c-340"
		"ypos"		"146"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_ONEHANDPIERCING"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"OneHandPiercingLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"OneHandPiercingLabel"
		"xpos"		"c-235"
		"ypos"		"146"
		"zpos"		"1"		
		"wide"		"50"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"west"
		"labelText"		"#ERROR"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"OneHandSlashingText"
	{
		"ControlName"		"Label"
		"fieldName"		"OneHandSlashingText"
		"xpos"		"c-340"
		"ypos"		"157"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_ONEHANDSLASHING"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"OneHandSlashingLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"OneHandSlashingLabel"
		"xpos"		"c-235"
		"ypos"		"157"
		"zpos"		"1"		
		"wide"		"50"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"west"
		"labelText"		"#ERROR"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"OneHandBashingText"
	{
		"ControlName"		"Label"
		"fieldName"		"OneHandBashingText"
		"xpos"		"c-340"
		"ypos"		"168"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_ONEHANDBASHING"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"OneHandBashingLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"OneHandBashingLabel"
		"xpos"		"c-235"
		"ypos"		"168"
		"zpos"		"1"		
		"wide"		"50"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"west"
		"labelText"		"#ERROR"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"TwoHandPiercingText"
	{
		"ControlName"		"Label"
		"fieldName"		"TwoHandPiercingText"
		"xpos"		"c-340"
		"ypos"		"179"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_TWOHANDPIERCING"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"TwoHandPiercingLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"TwoHandPiercingLabel"
		"xpos"		"c-235"
		"ypos"		"179"
		"zpos"		"1"		
		"wide"		"50"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"west"
		"labelText"		"#ERROR"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"TwoHandSlashingText"
	{
		"ControlName"		"Label"
		"fieldName"		"TwoHandSlashingText"
		"xpos"		"c-340"
		"ypos"		"190"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_TWOHANDSLASHING"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"TwoHandSlashingLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"TwoHandSlashingLabel"
		"xpos"		"c-235"
		"ypos"		"190"
		"zpos"		"1"		
		"wide"		"50"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"west"
		"labelText"		"#ERROR"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"TwoHandBashingText"
	{
		"ControlName"		"Label"
		"fieldName"		"TwoHandBashingText"
		"xpos"		"c-340"
		"ypos"		"201"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_TWOHANDBASHING"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"TwoHandBashingLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"TwoHandBashingLabel"
		"xpos"		"c-235"
		"ypos"		"201"
		"zpos"		"1"		
		"wide"		"50"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"west"
		"labelText"		"#ERROR"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"ArcheryText"
	{
		"ControlName"		"Label"
		"fieldName"		"ArcheryText"
		"xpos"		"c-340"
		"ypos"		"212"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_ARCHERY"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"ArcheryLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"ArcheryLabel"
		"xpos"		"c-235"
		"ypos"		"212"
		"zpos"		"1"		
		"wide"		"50"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"west"
		"labelText"		"#ERROR"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"ThrownWeaponsText"
	{
		"ControlName"		"Label"
		"fieldName"		"ThrownWeaponsText"
		"xpos"		"c-340"
		"ypos"		"223"
		"zpos"		"1"		
		"wide"		"100"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"east"
		"labelText"		"#MSS_THROWNWEAPONS"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}
	"ThrownWeaponsLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"ThrownWeaponsLabel"
		"xpos"		"c-235"
		"ypos"		"223"
		"zpos"		"1"		
		"wide"		"50"
		"tall"		"12"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
//		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"west"
		"labelText"		"#ERROR"
		"font"			"WritingFontVerySmall"
//		"TextColor"		"RedInk"
	}	
}
