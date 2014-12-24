
Scheme
{
	Colors
	{
		"Blank"			"0 0 0 0"

		"Bg"			"0 0 0 210"
		"BgArmed"		"150 90 0 30"

		"Text"			"255 176 100 200"
		"TextArmed"		"255 176 100 255"
		
		"InkWell"		"90 40 0 255" // BOXBOX MSS
		"InkWellLight"	"120 70 0 255"
		"RedInk"			"140 20 0 255"
	}

	BaseSettings
	{
		"FgColor"			"Text"
		"BgColor"			"Bg"

		Menu
		{
			"FgColor"			"Text"
			"BgColor"			"Blank"
			"ArmedFgColor"		"TextArmed"
			"ArmedBgColor"		"BgArmed"
			"DividerColor"		"InkWell"
			"TextInset"			"6"
		}

		Panel.BgColor			"Blank"
		Label.BgColor			"Blank"	
		Label.TextColor		"InkWell"
		
		// BOXBOX BUTTONS
//		Button.Border			"TFBorder"
//		Button.Border1	 	  	"TFBorder"
//		Button.Border2	 	  	"TFBorder"		
//		"ButtonBorder1"   		"TFBorder"
//		"ButtonBorder2"   		"TFBorder"
		
		Button.BgColor				"Blank"
		Button.TextColor				"Text"
		Button.ArmedTextColor			"TextArmed"
		Button.ArmedBgColor			"BgArmed"
		Button.DepressedTextColor		"TextArmed"
		Button.DepressedBgColor		"BgArmed"
		
		Button.DisabledFgColor			"InkWell"

		// text edit windows
		"WindowFgColor"				"Text"
		"WindowBgColor"				"Blank" // BOXBOX no extra tint behind text
		"WindowDisabledFgColor"		"Text"
		"WindowDisabledBgColor"		"Blank"		// background of chat conversation

		"SelectionFgColor"			"255 255 255 255"		// fg color of selected text
		"SelectionBgColor"			"Blank"
		"ListSelectionFgColor"		"255 255 255 255"			// 

		"ListBgColor"				"Blank"	// background of server browser control, etc
		"BuddyListBgColor"		"Blank"	// background of buddy list pane
		
		// App-specific stuff
		"ChatBgColor"				"Blank"

		// status selection
		"StatusSelectFgColor"		"Text"
		"StatusSelectFgColor2"		"Text"		// this is the color of the friends status

		// checkboxes
		"CheckButtonBorder1"   	"InkWell" 		// the left checkbutton border
		"CheckButtonBorder2"   	"InkWellLight"		// the right checkbutton border
		"CheckButtonCheck"		"0 0 0 255"				// color of the check itself
		"CheckBgColor"			"Text"

	// BOXBOX for the scoreboard
		SectionedListPanel.HeaderTextColor	"Text"
		SectionedListPanel.HeaderBgColor	"Blank"
		SectionedListPanel.DividerColor		"Black"
		SectionedListPanel.TextColor		"Text"
		SectionedListPanel.BrightTextColor	"TextArmed"
		SectionedListPanel.BgColor			"Blank" // BOXBOX ?
		SectionedListPanel.SelectedTextColor				"TextArmed"
		SectionedListPanel.SelectedBgColor				"Bg"
		SectionedListPanel.OutOfFocusSelectedTextColor	"Text"
		SectionedListPanel.OutOfFocusSelectedBgColor		"Blank"		
		
//		"ButtonBgColor"			"Blank"
//		"ButtonArmedFgColor"		"Blank"
//		"ButtonArmedBgColor"		"Blank"
//		"ButtonDepressedFgColor"	"Blank"
//		"ButtonDepressedBgColor"	"Blank"

		ScrollBarSlider.FgColor	"Blank"
		ScrollBarSlider.BgColor	"Blank"
		
		// buddy buttons
		BuddyButton
		{
			"FgColor1"		"ControlText"
			"FgColor2"		"117 134 102 255"

			"ArmedFgColor1"	"BrightBaseText"
			"ArmedFgColor2"	"BrightBaseText"
			"ArmedBgColor"	"SelectionBG"
		}

		Chat
		{
			"TextColor"				"BrightControlText"
			"SelfTextColor"			"BaseText"
			"SeperatorTextColor"	"DimBaseText"
		}
		
		InGameDesktop
		{
			"MenuColor"			"200 200 200 255"
			"ArmedMenuColor"	"255 255 255 255"
			"DepressedMenuColor" "192 186 80 255"
			"WidescreenBarColor" "0 0 0 0"
			"MenuItemVisibilityRate" "0.03"  // time it takes for one menu item to appear
			"MenuItemHeight"	"28"
			"GameMenuInset"		"32"
		}

		"SectionTextColor"		"BrightControlText"		// text color for IN-GAME, ONLINE, OFFLINE sections of buddy list
		"SectionDividerColor"	"BorderDark"		// color of line that runs under section name in buddy list

		"HudStatusBgColor"			"0 0 0 64"
		"HudStatusSelectedBgColor"	"0 0 0 192"
	}

	Fonts
	{
		Default
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"9"
				"weight"	"700"
				"antialias" "1"
				"yres"	"1 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"700"
				"antialias" "1"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"900"
				"antialias" "1"
				"yres"	"768 1023"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"900"
				"antialias" "1"
				"yres"	"1024 1199"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"900"
				"antialias" "1"
				"yres"	"1200 10000"
				"additive"	"1"
			}
		}
		"DefaultSmall"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"0"
				"range"		"0x0000 0x017F"
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"
				"weight"	"0"
				"range"		"0x0000 0x017F"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"0"
				"range"		"0x0000 0x017F"
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"0"
				"range"		"0x0000 0x017F"
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"0"
				"range"		"0x0000 0x017F"
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Arial"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
			}
		}
		"DefaultVerySmall"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Verdana"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
			}
			"7"
			{
				"name"		"Arial"
				"tall"		"11"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
			}
		}
		"HeaderFont" // BOXBOX MSS fonts.
		{
			"1"
			{
				"name"		"Stonehenge"
				"tall"		"72"
				"weight"		"1000"
			}
		}
		"HeaderFontSmall"
		{
			"1"
			{
				"name"		"Stonehenge"
				"tall"		"45"
				"weight"		"1000"
			}
		}
		"HeaderFontSmaller"
		{
			"1"
			{
				"name"		"Stonehenge"
				"tall"		"40"
				"weight"		"1000"
			}
		}		
		"CaligFont"
		{
			"1"
			{
				"name"		"Endor" 
				"tall"		"72"
				"weight"		"1000"
			}
		}		
		"WritingFont"
		{
			"1"
			{
				"name"		"Bilbo"
				"tall"		"48"
				"weight"		"1000"
			}
		}
		"WritingFont24"
		{
			"1"
			{
				"name"		"Bilbo"
				"tall"		"24"
				"weight"		"1000"
			}
		}		
		"WritingFontSmall"
		{
			"1"
			{
				"name"		"Bilbo"
				"tall"		"20"
				"weight"		"1000"
			}
		}
		"WritingFontSmaller"
		{
			"1"
			{
				"name"		"Bilbo"
				"tall"		"16"
				"weight"		"1000"
			}
		}		
		"WritingFontVerySmall"
		{
			"1"
			{
				"name"		"Bilbo"
				"tall"		"12"
				"weight"		"1000"
			}
		}		
		"DefaultVerySmallFallBack"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"10"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"		"480 599"
				"antialias"	"1"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"		"600 1199"
				"antialias"	"1"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"15"
				"weight"	"0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"		"1200 6000"
				"antialias"	"1"
			}
		}		
		"ButtonText"
		{
			"1"
			{
				"name"		"Arial"   // BOXBOX
				"tall"		"12"
				"weight"		"0"
			}
		}		
		"ButtonBold"
		{
			"1"
			{
				"name"		"Marlett"   // BOXBOX
				"tall"		"14"
				"weight"		"500"
			}
		}			
	}
	
	Borders
	{
		BaseBorder
		{
			"inset" "0 0 0 0"
			Left
			{
				"1"
				{
					"color" "Blank"
					"offset" "0 0"
				}
			}

			Right
			{
				"1"
				{
					"color" "Blank"
					"offset" "0 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "Blank"
					"offset" "0 0"
				}
			}

			Bottom
			{
				"1"
				{
					"color" "Blank"
					"offset" "0 0"
				}
			}
		}
		MSBorder
		{
			"inset" "0 0 0 0"
			Left
			{
				"1"
				{
					"color" "InkWell"
					"offset" "0 0"
				}
			}

			Right
			{
				"1"
				{
					"color" "InkWell"
					"offset" "0 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "InkWell"
					"offset" "0 0"
				}
			}

			Bottom
			{
				"1"
				{
					"color" "InkWell"
					"offset" "0 0"
				}
			}
		}
		NoBorder
		{
			"inset" "0 0 0 0"
			Left
			{
				"1"
				{
					"color" "Blank"
					"offset" "0 0"
				}
			}

			Right
			{
				"1"
				{
					"color" "Blank"
					"offset" "0 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "Blank"
					"offset" "0 0"
				}
			}

			Bottom
			{
				"1"
				{
					"color" "Blank"
					"offset" "0 0"
				}
			}
		}
	}

	CustomFontFiles
	{
		"1"		"resource/MSHeader.ttf"
		"2"		"resource/MSScript.ttf"
		"3"		"resource/MSWriting.ttf"		
	}
}