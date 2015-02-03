"Resource/UI/MSCharacterSelect.res"
{
	"CharSelect"
	{
		"ControlName"		"Frame"
		"fieldName"		"CharSelect"
		"xpos"		"0"
		"ypos"		"0"
		"wide"		"855"
		"tall"		"480"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
	}
	"TitleBGImg"
	{
		"ControlName"	"ImagePanel"
		"xpos"			"c-450"
		"ypos"			"c-350"
		"wide"			"900"
		"tall"			"700"
		"tileImage"		"0"
		"scaleImage"		"1"
		"image"			"mss_scroll"
		"border"			"border"
	}
	"TitleText"
	{
		"ControlName"		"Label"
		"fieldName"		"TitleText"
		"xpos"		"c-180"
		"ypos"		"70"
		"zpos"		"1"
		"wide"		"360"
		"tall"		"28"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"0"
		"tabPosition"		"0"
		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"center"
		"labelText"		"#MSS_CHARSELECT"
		"font"			"HeaderFont34"	
	}	
	"CharText1"
	{
		"ControlName"		"Label"
		"fieldName"		"CharText1"
		"xpos"		"c-270"
		"ypos"		"110"
		"zpos"		"1"
		"wide"		"180"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"0"
		"tabPosition"		"0"
		"disabledfgcolor2_override"	"InkWell"
//		"labelText"		"#MSS_SLOT1"
		"font"			"WritingFontSmall"
		"textAlignment"		"center"		
	}	
	"CharText2"
	{
		"ControlName"		"Label"
		"fieldName"		"CharText2"
		"xpos"		"c-90"
		"ypos"		"110"
		"zpos"		"1"
		"wide"		"180"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"0"
		"tabPosition"		"0"
		"disabledfgcolor2_override"	"InkWell"
//		"labelText"		"#MSS_SLOT2"
		"font"			"WritingFontSmall"
		"textAlignment"		"center"		
	}	
	"CharText3"
	{
		"ControlName"		"Label"
		"fieldName"		"CharText3"
		"xpos"		"c90"
		"ypos"		"110"
		"zpos"		"1"
		"wide"		"180"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"0"
		"tabPosition"		"0"
		"disabledfgcolor2_override"	"InkWell"
//		"labelText"		"#MSS_SLOT3"
		"font"			"WritingFontSmall"
		"textAlignment"		"center"		
	}	
	"CharButton1"
	{
		"ControlName"		"Button"
		"fieldName"		"CharButton1"
		"xpos"		"c-270"
		"ypos"		"130"
		"zpos"		"1"
		"wide"		"180"
		"tall"		"260"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
		"font"			"WritingFontSmall"
		"defaultFgColor_override"	"InkWellLight"
//		"disabledFgColor_override"	"InkWell" // BOXBOX doesn't work
//		"armedBgColor_override"	"Blank"
		"labelText"		"#MSS_NOCHAR"
		"textAlignment"		"center"
		"command"		"choosechar 1"
		"Default"		"1"
		"border"			"NoBorder"		
	}
	"CharButton2"
	{
		"ControlName"		"Button"
		"fieldName"		"CharButton2"
		"xpos"		"c-90"
		"ypos"		"130"
		"zpos"		"1"		
		"wide"		"180"
		"tall"		"260"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
		"font"			"WritingFontSmall"
		"defaultFgColor_override"	"InkWellLight"
//		"armedBgColor_override"	"Blank"
		"labelText"		"#MSS_NOCHAR"
		"textAlignment"		"center"
		"command"		"choosechar 2"
		"Default"		"1"
		"border"			"NoBorder"		
	}
	"CharButton3"
	{
		"ControlName"		"Button"
		"fieldName"		"CharButton3"
		"xpos"		"c90"
		"ypos"		"130"
		"zpos"		"1"		
		"wide"		"180"
		"tall"		"260"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
		"font"			"WritingFontSmall"
		"defaultFgColor_override"	"InkWellLight"
//		"armedBgColor_override"	"Blank"
		"labelText"		"#MSS_NOCHAR"
		"textAlignment"		"center"
		"command"		"choosechar 3"
		"Default"		"1"
		"border"			"NoBorder"		
	}
	"DeleteCharButton1"
	{
		"ControlName"		"Button"
		"fieldName"		"DeleteCharButton1"
		"xpos"		"c-270"
		"ypos"		"400"
		"wide"		"180"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
		"font"			"WritingFontVerySmall"
		"defaultFgColor_override"	"RedInk"
//		"armedBgColor_override"	"Blank"
		"labelText"		"#MSS_DELETECHAR"
		"textAlignment"		"center"
		"command"		"deletechar 1"
		"Default"		"1"
		"border"			"NoBorder"		
	}
	"DeleteCharButton2"
	{
		"ControlName"		"Button"
		"fieldName"		"DeleteCharButton2"
		"xpos"		"c-90"
		"ypos"		"400"
		"wide"		"180"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
		"font"			"WritingFontVerySmall"
		"defaultFgColor_override"	"RedInk"
//		"armedBgColor_override"	"Blank"
		"labelText"		"#MSS_DELETECHAR"
		"textAlignment"		"center"
		"command"		"deletechar 2"
		"Default"		"1"
		"border"			"NoBorder"		
	}
	"DeleteCharButton3"
	{
		"ControlName"		"Button"
		"fieldName"		"DeleteCharButton3"
		"xpos"		"c90"
		"ypos"		"400"
		"wide"		"180"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"0"
		"font"			"WritingFontVerySmall"
		"defaultFgColor_override"	"RedInk"
//		"armedBgColor_override"	"Blank"
		"labelText"		"#MSS_DELETECHAR"
		"textAlignment"		"center"
		"command"		"deletechar 3"
		"Default"		"1"
		"border"			"NoBorder"		
	}
	
	// BOXBOX the delete confirm controls
	"ConfirmBGImg"
	{
		"ControlName"		"ImagePanel"
		"fieldName"		"ConfirmBGImg"		
		"xpos"			"c-270"
		"ypos"			"c-60"
		"zpos"			"1"		
		"wide"			"540"
		"tall"			"120"
		"visible"		"0"		
		"tileImage"		"0"
		"scaleImage"		"1"
		"image"			"mss_scroll"
//		"border"			"border"
	}
	"ConfirmLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"ConfirmLabel"
		"xpos"		"c-190"
		"ypos"		"c-20"
		"zpos"		"2"
		"wide"		"400"
		"tall"		"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"0"
		"enabled"		"0"
//		"tabPosition"		"0"
		"disabledfgcolor2_override"	"InkWellLight"
		"textAlignment"		"center"
//		"labelText"		"#MSS_DEL_CONFIRM"
	}
	"YesButton"
	{
		"ControlName"		"Button"
		"fieldName"		"YesButton"
		"xpos"		"c-170"
		"ypos"		"c2"
		"zpos"		"2"		
		"wide"		"160"
		"tall"		"24"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"0"
		"enabled"		"0"
		"tabPosition"		"0"
		"font"			"WritingFontSmall"
		"defaultFgColor_override"	"InkWellLight"
//		"armedBgColor_override"	"Blank"
//		"labelText"		"#MSS_YES"
		"textAlignment"		"center"
		"command"		"confirmyes"
		"Default"		"1"
	}	
	"NoButton"
	{
		"ControlName"		"Button"
		"fieldName"		"NoButton"
		"xpos"		"c10"
		"ypos"		"c2"
		"zpos"		"2"		
		"wide"		"160"
		"tall"		"24"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"0"
		"enabled"		"0"
		"tabPosition"		"0"
		"font"			"WritingFontSmall"
		"defaultFgColor_override"	"InkWellLight"
//		"armedBgColor_override"	"Blank"
//		"labelText"		"#MSS_NO"
		"textAlignment"		"center"
		"command"		"confirmno"
		"Default"		"1"
	}	
	"charone" 
	{
		"ControlName"		"CModelPanel"
		"fieldName"		"charone"
		"xpos"		"c-270"
		"ypos"		"130"
		"zpos"		"0"		
		"wide"		"180"
		"tall"		"260"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"0"
		"fov"			"45"

		"model"
		{
			"spotlight"	"1"
			"modelname"	"models/player/humanmale.mdl"
			"origin_z"	"-35"
			"origin_y"	"3"
			"angles_y" "160"

			"animation"
			{
				"sequence"		"idle01"
				"pose_parameters"
				{
					"move_x" "1.0"
				}
			}
			
//			"attached_model"
//			{
//				"modelname" "models/weapons/w_arozensword.mdl"
//			}
		}
	}
	"chartwo"
	{
		"ControlName"		"CModelPanel"
		"fieldName"		"chartwo"
		"xpos"		"c-90"
		"ypos"		"130"
		"zpos"		"0"		
		"wide"		"180"
		"tall"		"260"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"0"
		"fov"			"45"

		"model"
		{
			"spotlight"	"1"
			"modelname"	"models/player/humanmale.mdl"
			"origin_z"	"-35"
			"origin_y"	"3"
			"angles_y" "160"

			"animation"
			{
				"sequence"		"idle01"
				"pose_parameters"
				{
					"move_x" "1.0"
				}
			}
			
//			"attached_model"
//			{
//				"modelname" "models/weapons/w_arozensword.mdl"
//			}
		}
	}	
	"charthree"
	{
		"ControlName"		"CModelPanel"
		"fieldName"		"charthree"
		"xpos"		"c90"
		"ypos"		"130"
		"zpos"		"0"		
		"wide"		"180"
		"tall"		"260"
		"autoResize"		"0"
		"pinCorner"		"2"
		"visible"		"1"
		"enabled"		"0"
		"fov"			"45"

		"model"
		{
			"spotlight"	"1"
			"modelname"	"models/player/humanmale.mdl"
			"origin_z"	"-35"
			"origin_y"	"3"
			"angles_y" "160"

			"animation"
			{
				"sequence"		"idle01"
				"pose_parameters"
				{
					"move_x" "1.0"
				}
			}
			
//			"attached_model"
//			{
//				"modelname" "models/weapons/w_arozensword.mdl"
//			}
		}
	}
}
