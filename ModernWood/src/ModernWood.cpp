#include <ModernWood.h>

bool SwitchState[ALTURATECLADO][ANCHURATECLADO] = {false};
bool SwitchLastState[ALTURATECLADO][ANCHURATECLADO] = {false};
unsigned long Debounce[ALTURATECLADO][ANCHURATECLADO] = {0};
int option_choose = 0;
int option_selected = 0;

bool MenuPressed[6] = {false}; 						// Enter, Up, Left, Down, Right
bool KeysPressedConfig[6] = {false};			  	// Enter, Up, Left, Down, Right
bool KeysPressedConfigLast[6] = {false};		  	// Enter, Up, Left, Down, Right
unsigned long KeysPressedConfigDebounce[6] = {0}; 	// Enter, Up, Left, Down, Right

void WorkingModeKeyboard(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected)
{

	bool SwicthFastState;
	if (isUSBConnected)
	{
		for (int i = 0; i < ANCHURATECLADO; i++)
		{
			digitalWrite(COD0, nums0_15[i][0]);
			digitalWrite(COD1, nums0_15[i][1]);
			digitalWrite(COD2, nums0_15[i][2]);
			digitalWrite(COD3, nums0_15[i][3]);
			for (int k = 0; k < ALTURATECLADO; k++)
			{
				SwicthFastState = !(digitalRead(ESwitch[k]));
				if (SwicthFastState && !SwitchLastState[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
				{
					Debounce[k][i] = millis();
					SwitchLastState[k][i] = SwicthFastState;
					Keyboard.press(TECLASTECLADO[k][i]);
				}
				else if (!SwicthFastState && SwitchLastState[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
				{
					Debounce[k][i] = millis();
					SwitchLastState[k][i] = SwicthFastState;
					Keyboard.release(TECLASTECLADO[k][i]);
				}
				SwitchState[k][i] = SwicthFastState;
			}
		}
	}
	else
	{
		for (int i = 0; i < ANCHURATECLADO; i++)
		{
			digitalWrite(COD0, nums0_15[i][0]);
			digitalWrite(COD1, nums0_15[i][1]);
			digitalWrite(COD2, nums0_15[i][2]);
			digitalWrite(COD3, nums0_15[i][3]);
			for (int k = 0; k < ALTURATECLADO; k++)
			{
				SwicthFastState = !(digitalRead(ESwitch[k]));
				if (SwicthFastState && !SwitchLastState[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
				{
					Debounce[k][i] = millis();
					SwitchLastState[k][i] = SwicthFastState;
					bleKeyboard.press(TECLASTECLADO[k][i]);
				}
				else if (!SwicthFastState && SwitchLastState[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
				{
					Debounce[k][i] = millis();
					SwitchLastState[k][i] = SwicthFastState;
					bleKeyboard.release(TECLASTECLADO[k][i]);
				}
				SwitchState[k][i] = SwicthFastState;
			}
		}
	}
}

void WorkingModeDisplay(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected)
{
	bool SwicthFastState;

	// Arrow keys
	for (int i = KeyMenuLeftCol; i < ANCHURATECLADO; i++)
	{
		digitalWrite(COD0, nums0_15[i][0]);
		digitalWrite(COD1, nums0_15[i][1]);
		digitalWrite(COD2, nums0_15[i][2]);
		digitalWrite(COD3, nums0_15[i][3]);
		for (int k = KeyMenuUpRow; k < ALTURATECLADO; k++)
		{
			int auxindex = 0;
			// Up arrow key
			if (i == KeyMenuUpCol && k == KeyMenuUpRow)
			{
				auxindex = ArrUp;
			}
			// Left arrow key
			if (i == KeyMenuLeftCol && k == KeyMenuLeftRow)
			{
				auxindex = ArrLeft;
			}
			// Down arrow key
			if (i == KeyMenuDownCol && k == KeyMenuDownRow)
			{
				auxindex = ArrDown;
			}
			// Right arrow key
			if (i == KeyMenuRightCol && k == KeyMenuRightRow)
			{
				auxindex = ArrRight;
			}

			SwicthFastState = !(digitalRead(ESwitch[k]));
			if (SwicthFastState && !KeysPressedConfigLast[auxindex] && (millis() - KeysPressedConfigDebounce[auxindex]) > TiempoDebounce)
			{
				KeysPressedConfigDebounce[auxindex] = millis();
				KeysPressedConfigLast[auxindex] = SwicthFastState;
				MenuPressed[auxindex] = true;
			}
			else if (!SwicthFastState && KeysPressedConfigLast[auxindex] && (millis() - KeysPressedConfigDebounce[auxindex]) > TiempoDebounce)
			{
				KeysPressedConfigDebounce[auxindex] = millis();
				KeysPressedConfigLast[auxindex] = SwicthFastState;
				MenuPressed[auxindex] = false;
			}
			KeysPressedConfig[auxindex] = SwicthFastState;
		}
	}

	//Enter key
	digitalWrite(COD0, nums0_15[KeyMenuEnterCol][0]);
	digitalWrite(COD1, nums0_15[KeyMenuEnterCol][1]);
	digitalWrite(COD2, nums0_15[KeyMenuEnterCol][2]);
	digitalWrite(COD3, nums0_15[KeyMenuEnterCol][3]);
	SwicthFastState = !(digitalRead(ESwitch[KeyMenuEnterRow]));
	if (SwicthFastState && !KeysPressedConfigLast[ArrEnter] && (millis() - KeysPressedConfigDebounce[ArrEnter]) > TiempoDebounce)
	{
		KeysPressedConfigDebounce[ArrEnter] = millis();
		KeysPressedConfigLast[ArrEnter] = SwicthFastState;
		MenuPressed[ArrEnter] = true;
	}
	else if (!SwicthFastState && KeysPressedConfigLast[ArrEnter] && (millis() - KeysPressedConfigDebounce[ArrEnter]) > TiempoDebounce)
	{
		KeysPressedConfigDebounce[ArrEnter] = millis();
		KeysPressedConfigLast[ArrEnter] = SwicthFastState;
		MenuPressed[ArrEnter] = false;
	}
	KeysPressedConfig[ArrEnter] = SwicthFastState;

	//ESC key
	digitalWrite(COD0, nums0_15[KeyMenuEscCol][0]);
	digitalWrite(COD1, nums0_15[KeyMenuEscCol][1]);
	digitalWrite(COD2, nums0_15[KeyMenuEscCol][2]);
	digitalWrite(COD3, nums0_15[KeyMenuEscCol][3]);
	SwicthFastState = !(digitalRead(ESwitch[KeyMenuEscRow]));
	if (SwicthFastState && !KeysPressedConfigLast[ArrEsc] && (millis() - KeysPressedConfigDebounce[ArrEsc]) > TiempoDebounce)
	{
		KeysPressedConfigDebounce[ArrEsc] = millis();
		KeysPressedConfigLast[ArrEsc] = SwicthFastState;
		MenuPressed[ArrEsc] = true;
	}
	else if (!SwicthFastState && KeysPressedConfigLast[ArrEsc] && (millis() - KeysPressedConfigDebounce[ArrEsc]) > TiempoDebounce)
	{
		KeysPressedConfigDebounce[ArrEsc] = millis();
		KeysPressedConfigLast[ArrEsc] = SwicthFastState;
		MenuPressed[ArrEsc] = false;
	}
	KeysPressedConfig[ArrEsc] = SwicthFastState;

	//OPTION SELECTION

	bool changed_option = false;
	int old_option_selected = option_selected;

	//if right arrow key is pressed
	if (MenuPressed[ArrRight])
	{
		option_selected = modulo_p((option_selected + 1),6);
		changed_option = true;
		MenuPressed[ArrRight] = false;
		Serial.println(option_selected);
	}

	//if left arrow key is pressed
	if (MenuPressed[ArrLeft])
	{
		option_selected = modulo_p((option_selected - 1),6);
		changed_option = true;
		MenuPressed[ArrLeft] = false;
		Serial.println(option_selected);
	}

	//if down arrow key is pressed
	if (MenuPressed[ArrDown])
	{
		option_selected = modulo_p((option_selected + 3),6);
		changed_option = true;
		MenuPressed[ArrDown] = false;
		Serial.println(option_selected);
	}

	//if up arrow key is pressed
	if (MenuPressed[ArrUp])
	{
		option_selected = modulo_p((option_selected - 3),6);
		changed_option = true;
		MenuPressed[ArrUp] = false;
		Serial.println(option_selected);
	}

	//if up enter key is pressed
	if (MenuPressed[ArrEnter])
	{
		//Erase the screen of the menu only (Config menu is the first one)
		printGeneralDisplay(tft);

		//Print a text in the screen to indicate that the option is selected
		tft.setCursor(General_Screen_display.x+2, General_Screen_display.y+2);
		tft.print("Option ");
		tft.print(option_selected);
		tft.print(" selected");

		option_selected = 10;
		MenuPressed[ArrEnter] = false;
	}

	//if up Esc key is pressed
	if (MenuPressed[ArrEsc])
	{
		printGeneralDisplay(tft);
		//Repaint all the menu icons
		for(int i = 0;i < 6; i++){
			printMenuOptionNumber(tft, i, false);
		}

		//Reset the menu
		option_selected = 0;
		MenuPressed[ArrEsc] = false;
	}

	//Print the last option in normal mode
	if(changed_option)
	{
		// Print the menu with the new option selected
		old_option_selected = printMenuOptionNumber(tft, old_option_selected, false);
		option_selected = printMenuOptionNumber(tft, option_selected, true);
	}
}

int printMenuOptionNumber(TFT_eSPI &tft, int _option_selected, bool is_inverted)
{
	int ret_option_selected = -1;
	switch (_option_selected)
	{
	// Push the inverted image of the option 1
	case MenuConfig:
		is_inverted ? printMenuConfigDisplayInverted(tft) : printMenuConfigDisplay(tft);
		ret_option_selected = MenuConfig;
		_option_selected = -1;
		break;

	case MenuBrightness:
		is_inverted ? printMenuBrightnessDisplayInverted(tft) : printMenuBrightnessDisplay(tft);
		ret_option_selected = MenuBrightness;
		_option_selected = -1;
		break;

	case MenuLeds:
		is_inverted ? printMenuLedsDisplayInverted(tft) : printMenuLedsDisplay(tft);
		ret_option_selected = MenuLeds;
		_option_selected = -1;
		break;

	case MenuEnergy:
		is_inverted ? printMenuEnergyDisplayInverted(tft) : printMenuEnergyDisplay(tft);
		ret_option_selected = MenuEnergy;
		_option_selected = -1;
		break;

	case MenuConnection:
		is_inverted ? printMenuConnectionDisplayInverted(tft) : printMenuConnectionDisplay(tft);
		ret_option_selected = MenuConnection;
		_option_selected = -1;
		break;

	case MenuInfoHelp:
		is_inverted ? printMenuInfoHelpDisplayInverted(tft) : printMenuInfoHelpDisplay(tft);
		ret_option_selected = MenuInfoHelp;
		_option_selected = -1;
		break;

	default:
		break;
	}
	return ret_option_selected;
}

// Option 1
void printMenuConfigDisplay(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Config_display_image.x, Menu_Config_display_image.y);
	tft.pushImage(Menu_Config_display_image.x,
				  Menu_Config_display_image.y,
				  Menu_Config_display_image.width,
				  Menu_Config_display_image.height,
				  image_Menu_Config, 0);
}

// Inverted image of the option 1
void printMenuConfigDisplayInverted(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Config_display_image.x, Menu_Config_display_image.y);
	tft.pushImage(Menu_Config_display_image.x,
				  Menu_Config_display_image.y,
				  Menu_Config_display_image.width,
				  Menu_Config_display_image.height,
				  image_Menu_Config_Inverted, 0);
}

// Option 2
void printMenuBrightnessDisplay(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Brightness_display_image.x, Menu_Brightness_display_image.y);
	tft.pushImage(Menu_Brightness_display_image.x,
				  Menu_Brightness_display_image.y,
				  Menu_Brightness_display_image.width,
				  Menu_Brightness_display_image.height,
				  image_Menu_Brightness, 0);
}

// Inverted image of the option 2
void printMenuBrightnessDisplayInverted(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Brightness_display_image.x, Menu_Brightness_display_image.y);
	tft.pushImage(Menu_Brightness_display_image.x,
				  Menu_Brightness_display_image.y,
				  Menu_Brightness_display_image.width,
				  Menu_Brightness_display_image.height,
				  image_Menu_Brightness_Inverted, 0);
}

// Option 3
void printMenuLedsDisplay(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Leds_display_image.x, Menu_Leds_display_image.y);
	tft.pushImage(Menu_Leds_display_image.x,
				  Menu_Leds_display_image.y,
				  Menu_Leds_display_image.width,
				  Menu_Leds_display_image.height,
				  image_Menu_Leds, 0);
}

// Inverted image of the option 3
void printMenuLedsDisplayInverted(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Leds_display_image.x, Menu_Leds_display_image.y);
	tft.pushImage(Menu_Leds_display_image.x,
				  Menu_Leds_display_image.y,
				  Menu_Leds_display_image.width,
				  Menu_Leds_display_image.height,
				  image_Menu_Leds_Inverted, 0);
}

// Option 4
void printMenuEnergyDisplay(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Energy_display_image.x, Menu_Energy_display_image.y);
	tft.pushImage(Menu_Energy_display_image.x,
				  Menu_Energy_display_image.y,
				  Menu_Energy_display_image.width,
				  Menu_Energy_display_image.height,
				  image_Menu_Energy, 0);
}

// Inverted image of the option 4
void printMenuEnergyDisplayInverted(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Energy_display_image.x, Menu_Energy_display_image.y);
	tft.pushImage(Menu_Energy_display_image.x,
				  Menu_Energy_display_image.y,
				  Menu_Energy_display_image.width,
				  Menu_Energy_display_image.height,
				  image_Menu_Energy_Inverted, 0);
}

// Option 5
void printMenuConnectionDisplay(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Connection_display_image.x, Menu_Connection_display_image.y);
	tft.pushImage(Menu_Connection_display_image.x,
				  Menu_Connection_display_image.y,
				  Menu_Connection_display_image.width,
				  Menu_Connection_display_image.height,
				  image_Menu_Connectivity, 0);
}

// Inverted image of the option 5
void printMenuConnectionDisplayInverted(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Connection_display_image.x, Menu_Connection_display_image.y);
	tft.pushImage(Menu_Connection_display_image.x,
				  Menu_Connection_display_image.y,
				  Menu_Connection_display_image.width,
				  Menu_Connection_display_image.height,
				  image_Menu_Connectivity_Inverted, 0);
}

// Option 6
void printMenuInfoHelpDisplay(TFT_eSPI &tft)
{
	tft.setCursor(Menu_InfoHelp_display_image.x, Menu_InfoHelp_display_image.y);
	tft.pushImage(Menu_InfoHelp_display_image.x,
				  Menu_InfoHelp_display_image.y,
				  Menu_InfoHelp_display_image.width,
				  Menu_InfoHelp_display_image.height,
				  image_Menu_InfoHelp, 0);
}

// Inverted image of the option 6
void printMenuInfoHelpDisplayInverted(TFT_eSPI &tft)
{
	tft.setCursor(Menu_InfoHelp_display_image.x, Menu_InfoHelp_display_image.y);
	tft.pushImage(Menu_InfoHelp_display_image.x,
				  Menu_InfoHelp_display_image.y,
				  Menu_InfoHelp_display_image.width,
				  Menu_InfoHelp_display_image.height,
				  image_Menu_InfoHelp_Inverted, 0);
}

void printGeneralDisplay(TFT_eSPI &tft)
{
	tft.setCursor(Menu_Config_display_image.x, Menu_Config_display_image.y);
	tft.fillRect(General_Screen_display.x, General_Screen_display.y, General_Screen_display.width, General_Screen_display.height, TFT_BLACK);
}