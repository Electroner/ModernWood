#include <ModernWood.h>

int TiempoDebounce = 8; // Debounce time
bool SwitchState[KEYBOARDHEIGHT][KEYBOARDWIDTH] = {false};
bool SwitchLastState[KEYBOARDHEIGHT][KEYBOARDWIDTH] = {false};
unsigned long Debounce[KEYBOARDHEIGHT][KEYBOARDWIDTH] = {0};

int option_selected = 0;		 // MAIN MENU OPTION SELECTED (0-5)
int option_selected_submenu = 0; // SUBMENU OPTION SELECTED (0-n)

bool MenuPressed[6] = {false};					  // Enter, Up, Left, Down, Right
bool KeysPressedConfig[6] = {false};			  // Enter, Up, Left, Down, Right
bool KeysPressedConfigLast[6] = {false};		  // Enter, Up, Left, Down, Right
unsigned long KeysPressedConfigDebounce[6] = {0}; // Enter, Up, Left, Down, Right
bool InMenu = true;								  // Check if the user is in the Menu (True)
bool InSubConfig = false;						  // Check if the user is in the Sub Menu Config Option (True)

// ################################################## USER CONFIGURATION (PREFERENCES) ##################################################

int *SubMenuConfigVar[SizeSubMenuConfig] = {&DisplayEnabled, &KeyboardEnabled, &TiempoDebounce, &LanguageMenu};
int *SubMenuBrightnessVar[SizeSubMenuBrightness] = {&LedsBrightness, &DisplayBrightness};
int *SubMenuLedsVar[SizeSubMenuLeds] = {&LedsActive, &LedsColor.color, &LedsMode, &LedsSpeed};
int *SubMenuEnergyVar[SizeSubMenuEnergy] = {&BatteryEnabled, &EnergySaveMode};
int *SubMenuConnectionVar[SizeSubMenuConnection] = {&BLEEnabled, &isBLEPreferred, &isUSBPreferred};

Preferences configurationModernWood = Preferences();
bool ChangedConfig = false;

void loadUserConfiguration(int Menu, int Option, bool OpenConfig = true)
{
	if (OpenConfig)
	{
		configurationModernWood.begin("MWConfig", false);
	}

	// if the menu is not valid we return (keeping default values)
	if (GetSizeSubMenu(Menu) < 1)
	{
		return;
	}

	switch (Menu)
	{
	case 0:
		*SubMenuConfigVar[Option] = configurationModernWood.getInt(SubMenuConfigKeys[Option].c_str(), *SubMenuConfigVar[Option]);
		break;

	case 1:
		*SubMenuBrightnessVar[Option] = configurationModernWood.getInt(SubMenuBrightnessKeys[Option].c_str(), *SubMenuBrightnessVar[Option]);
		break;

	case 2:
		*SubMenuLedsVar[Option] = configurationModernWood.getInt(SubMenuLedsKeys[Option].c_str(), *SubMenuLedsVar[Option]);
		break;

	case 3:
		*SubMenuEnergyVar[Option] = configurationModernWood.getInt(SubMenuEnergyKeys[Option].c_str(), *SubMenuEnergyVar[Option]);
		break;

	case 4:
		// No configuration to load
		break;

	case 5:
		// No configuration to load
		break;

	default:
		break;
	}

	if (OpenConfig)
	{
		configurationModernWood.end();
	}
}

void loadUserConfiguration()
{
	configurationModernWood.begin("MWConfig", false);

	// Load all the configuration
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < GetSizeSubMenu(i); j++)
		{
			loadUserConfiguration(i, j, false);
		}
	}

	configurationModernWood.end();
}

void saveUserConfiguration(int Menu, int Option, bool OpenConfig = true)
{
	if (OpenConfig)
	{
		configurationModernWood.begin("MWConfig", false);
	}

	switch (Menu)
	{
	case 0:
		configurationModernWood.putInt(SubMenuConfigKeys[Option].c_str(), *SubMenuConfigVar[Option]);
		break;
	case 1:
		configurationModernWood.putInt(SubMenuBrightnessKeys[Option].c_str(), *SubMenuBrightnessVar[Option]);
		break;
	case 2:
		configurationModernWood.putInt(SubMenuLedsKeys[Option].c_str(), *SubMenuLedsVar[Option]);
		break;
	case 3:
		configurationModernWood.putInt(SubMenuEnergyKeys[Option].c_str(), *SubMenuEnergyVar[Option]);
		break;
	case 4:
		// No configuration to save
		break;
	case 5:
		// No configuration to save
		break;
	default:
		break;
	}

	if (OpenConfig)
	{
		configurationModernWood.end();
	}
}

void saveUserConfiguration()
{
	configurationModernWood.begin("MWConfig", false);

	// Save all the configuration
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < GetSizeSubMenu(i); j++)
		{
			saveUserConfiguration(i, j, false);
		}
	}

	configurationModernWood.end();
}

// MAIN FUNCTIONS
//  ################################################## LED INDICATOR ##################################################

Adafruit_NeoPixel RgbLED = Adafruit_NeoPixel(NUMBER_OF_LEDS, PIN_LED_INDICATOR, NEO_GRB + NEO_KHZ800);
int LedsBrightness = 100;
int LedsActive = 1;
RGB LedsColor = RGB();
int LedsMode = 0;
int LedsSpeed = 50;

int interval = 250;
unsigned long lastUpdateTimeRainbow = 0;

// ################################################## BATTERY ##################################################

int BatteryEnabled = 0;
uint8_t volatile batteryLevel = 100;
uint8_t volatile batteryLevelLast = 100;
bool batteryLevelChanged = true;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Energy Save Mode
int EnergySaveMode = 0;
bool goingToSleep = false;
bool Sleeping = false;
bool timerSetupDone = false;
hw_timer_t *EnergyModetimer = NULL;
unsigned long lastKeyPressTime = 0;

void IRAM_ATTR checkBatteryLevel()
{
	portENTER_CRITICAL_ISR(&timerMux);
	// We are going to use a voltage divider to measure the battery with a resistance of 1M and another of 1M and the voltage is 4.2V (100%) and 3.7V (0%)
	// Ranges from 2296 to 2606 (1.85V to 2.1V)
	batteryLevel = static_cast<int>(map(analogRead(PIN_BATTERY), 2296, 2606, 0.0, 100.0));

	// If the battery level is less than 0% set it to 0%
	if (batteryLevel < 0)
	{
		batteryLevel = 0;
	}
	// If the battery level is more than 100% set it to 100%
	if (batteryLevel > 100)
	{
		batteryLevel = 100;
	}

	if (batteryLevel != batteryLevelLast)
	{
		batteryLevelLast = batteryLevel;
		batteryLevelChanged = true;
	}
	portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR onKeyPress()
{
	portENTER_CRITICAL_ISR(&timerMux);
	lastKeyPressTime = millis();

	if(Sleeping)
	{
		goingToSleep = false;
	}
	portEXIT_CRITICAL_ISR(&timerMux);
}

void setupTimerEnergySave()
{
	// Set and activate the timer to check the power saving mode every 1 second
	EnergyModetimer = timerBegin(1, 80, true); // 80 prescaler for 1 MHz clock
	timerAttachInterrupt(EnergyModetimer, &checkEnergySaveMode, true);
	timerAlarmWrite(EnergyModetimer, 10000000, true); // 10 sec
	timerAlarmEnable(EnergyModetimer);
}

void IRAM_ATTR checkEnergySaveMode()
{
	portENTER_CRITICAL_ISR(&timerMux);
	// If no key has been pressed in 5 minutes, activate power saving mode
	if (millis() - lastKeyPressTime >= ENERGY_SAVE_MODE_TIME * 1000)
	{
		goingToSleep = true;
	}
	portEXIT_CRITICAL_ISR(&timerMux);
}

void enterEnergySaveMode()
{
	// Turn off the RGB LEDs
	RgbLED.clear();
	RgbLED.show();

	// Turn off the screen
	analogWrite(BLK_SCREEN, LOW);
	displayChanged = true;

	// Turn on the power saving mode flag
	Sleeping = true;
}

void wakeupHandler()
{
	// To ensure that the device does not enter power saving mode again
	lastKeyPressTime = millis();

	// Turn on the screen
	analogWrite(BLK_SCREEN, *SubMenuBrightnessVar[_BrightnessDisplay] * 2.55);

	// Turn on the RGB LEDs
	float brightness = (*SubMenuBrightnessVar[_BrightnessLeds] / 100.0f);
	for(int i = 0; i < NUMBER_OF_LEDS; i++) {
        RgbLED.setPixelColor(i, (int)(LedsColor.r * brightness),
                                (int)(LedsColor.g * brightness),
                                (int)(LedsColor.b * brightness));
    }
	if (*SubMenuLedsVar[_EnableLeds] == 0)
	{
		RgbLED.clear();
	}
	RgbLED.show();

	// Turn on flags for the modes
	connectionChanged = true;

	// Turn off the power saving mode flag
	goingToSleep = false;
	Sleeping = false;
}

// ################################################## USB HID ##################################################

int BLEEnabled = 0; // 1 if BLE is enabled 0 if not
int isBLEPreferred = 0;
int isUSBPreferred = 1;
bool connectionChanged = false;

bool volatile isUSBConnected = true;
bool volatile isBLEConnected = false;

// Set the USB and BLE connected flags when Pin <PIN_USB_CONNECTED> is triggered RISSING
void IRAM_ATTR USBConnected()
{
	isUSBConnected = true;
	isBLEConnected = false;
}

// Set the USB and BLE connected flags when Pin <PIN_USB_CONNECTED> is triggered FALLING
void IRAM_ATTR USBDisconnected()
{
	isUSBConnected = false;
	isBLEConnected = true;
}

// ################################################## DISPLAY ##################################################

TFT_eSPI tft = TFT_eSPI(DISPLAY_WIDTH, DISPLAY_HEIGHT); // Invoke custom library
int DisplayEnabled = 1;
int LanguageMenu = 0;
int DisplayBrightness = 100;
bool displayChanged = true;

// ################################################## KEYBOARD ##################################################

bool volatile WorkingAsKeyboard = true;
bool volatile interrupted_FN = false;
// millis var to debounce
unsigned int volatile last_interrupt_FN_time = 0;

int KeyboardEnabled = 1;

// Swap bettwen the keyboard and the display mode when the FN key is pressed
void IRAM_ATTR FNKeyboardDisplay()
{
	unsigned int interrupt_time = millis();
	if (interrupt_time - last_interrupt_FN_time > DEBOUNCE_DELAY_FN)
	{
		// Changes the mode of the keyboard and set the interrupt Flag
		WorkingAsKeyboard = !WorkingAsKeyboard;
		interrupted_FN = true;
		lastKeyPressTime = interrupt_time;
		last_interrupt_FN_time = interrupt_time;
	}
}

// ################################################## OTHERS ##################################################

bool inExternalFunctionMode = false;  // To know if we are in the external function mode
bool executingCustomFunction = false; // To know if we are executing a custom function
// If there is a actual function begin exectued, which is the row and col of the key
int actualFunctionRow = 0;
int actualFunctionCol = 0;

bool configurationReseted = false; // To know if the configuration has been reseted

// ################################################## MODES ##################################################

void WorkingInExternalFunctionMode(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected)
{
	bool SwicthFastState;
	for (int i = 0; i < KEYBOARDWIDTH; i++)
	{
		digitalWrite(COD0, nums0_15[i][0]);
		digitalWrite(COD1, nums0_15[i][1]);
		digitalWrite(COD2, nums0_15[i][2]);
		digitalWrite(COD3, nums0_15[i][3]);
		for (int k = 0; k < KEYBOARDHEIGHT; k++)
		{
			SwicthFastState = !(digitalRead(ESwitch[k]));

			// If any switch is pressed we set the actualFunctionRow and actualFunctionCol
			if (SwicthFastState)
			{
				actualFunctionRow = k;
				actualFunctionCol = i;

				// And set the executingCustomFunction to true
				executingCustomFunction = true;

				// We exit the loop
				i = KEYBOARDWIDTH;
				k = KEYBOARDHEIGHT;
			}
		}
	}
}

void WorkingModeKeyboard(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected)
{
	bool SwicthFastState;
	if (isUSBConnected && !isBLEPreferred)
	{
		for (int i = 0; i < KEYBOARDWIDTH; i++)
		{
			digitalWrite(COD0, nums0_15[i][0]);
			digitalWrite(COD1, nums0_15[i][1]);
			digitalWrite(COD2, nums0_15[i][2]);
			digitalWrite(COD3, nums0_15[i][3]);
			delayMicroseconds(10); // Short delay for signal stabilization
			for (int k = 0; k < KEYBOARDHEIGHT; k++)
			{
				SwicthFastState = !(digitalRead(ESwitch[k]));
				if (SwicthFastState && !SwitchLastState[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
				{
					Debounce[k][i] = millis();
					SwitchLastState[k][i] = SwicthFastState;
					Keyboard.press(KEYSKEYBOARD[k][i]);
				}
				else if (!SwicthFastState && SwitchLastState[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
				{
					Debounce[k][i] = millis();
					SwitchLastState[k][i] = SwicthFastState;
					Keyboard.release(KEYSKEYBOARD[k][i]);
				}
				SwitchState[k][i] = SwicthFastState;
			}
		}
	}
	else if (!isUSBPreferred)
	{
		for (int i = 0; i < KEYBOARDWIDTH; i++)
		{
			digitalWrite(COD0, nums0_15[i][0]);
			digitalWrite(COD1, nums0_15[i][1]);
			digitalWrite(COD2, nums0_15[i][2]);
			digitalWrite(COD3, nums0_15[i][3]);
			delayMicroseconds(10); // Short delay for signal stabilization
			for (int k = 0; k < KEYBOARDHEIGHT; k++)
			{
				SwicthFastState = !(digitalRead(ESwitch[k]));
				if (SwicthFastState && !SwitchLastState[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
				{
					Debounce[k][i] = millis();
					SwitchLastState[k][i] = SwicthFastState;
					bleKeyboard.press(KEYSKEYBOARD[k][i]);
				}
				else if (!SwicthFastState && SwitchLastState[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
				{
					Debounce[k][i] = millis();
					SwitchLastState[k][i] = SwicthFastState;
					bleKeyboard.release(KEYSKEYBOARD[k][i]);
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
	for (int i = KeyMenuLeftCol; i < KEYBOARDWIDTH; i++)
	{
		digitalWrite(COD0, nums0_15[i][0]);
		digitalWrite(COD1, nums0_15[i][1]);
		digitalWrite(COD2, nums0_15[i][2]);
		digitalWrite(COD3, nums0_15[i][3]);
		delayMicroseconds(10); // Short delay for signal stabilization
		for (int k = KeyMenuUpRow; k < KEYBOARDHEIGHT; k++)
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

	// Enter key
	digitalWrite(COD0, nums0_15[KeyMenuEnterCol][0]);
	digitalWrite(COD1, nums0_15[KeyMenuEnterCol][1]);
	digitalWrite(COD2, nums0_15[KeyMenuEnterCol][2]);
	digitalWrite(COD3, nums0_15[KeyMenuEnterCol][3]);
	delayMicroseconds(10); // Short delay for signal stabilization
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

	// ESC key
	digitalWrite(COD0, nums0_15[KeyMenuEscCol][0]);
	digitalWrite(COD1, nums0_15[KeyMenuEscCol][1]);
	digitalWrite(COD2, nums0_15[KeyMenuEscCol][2]);
	digitalWrite(COD3, nums0_15[KeyMenuEscCol][3]);
	delayMicroseconds(10); // Short delay for signal stabilization
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

	// OPTION SELECTION

	bool changed_option = false;		 // Check if the option has changed (Main Menu)
	bool changed_option_subMenu = false; // Check if the option has changed (Sub Menu)
	bool config_option_selected = false; // Check if the option is selected to be configured
	int old_option_selected = option_selected;

	// if right arrow key is pressed
	if (MenuPressed[ArrRight])
	{
		if (InMenu)
		{
			option_selected = modulo_p((option_selected + 1), 6);
			changed_option = true;
		}
		else
		{
			ChangeConfig(option_selected, option_selected_submenu, changed_option_subMenu, true);
		}
		MenuPressed[ArrRight] = false;
	}

	// if left arrow key is pressed
	if (MenuPressed[ArrLeft])
	{
		if (InMenu)
		{
			option_selected = modulo_p((option_selected - 1), 6);
			changed_option = true;
		}
		else
		{
			ChangeConfig(option_selected, option_selected_submenu, changed_option_subMenu, false);
		}
		MenuPressed[ArrLeft] = false;
	}

	// if down arrow key is pressed
	if (MenuPressed[ArrDown])
	{
		if (InMenu)
		{
			option_selected = modulo_p((option_selected + 3), 6);
			changed_option = true;
		}
		else
		{
			option_selected_submenu = modulo_p((option_selected_submenu + 1), GetSizeSubMenu(option_selected));
			changed_option_subMenu = true;
		}
		MenuPressed[ArrDown] = false;
	}

	// if up arrow key is pressed
	if (MenuPressed[ArrUp])
	{
		if (InMenu)
		{
			option_selected = modulo_p((option_selected - 3), 6);
			changed_option = true;
		}
		else
		{
			option_selected_submenu = modulo_p((option_selected_submenu - 1), GetSizeSubMenu(option_selected));
			changed_option_subMenu = true;
		}
		MenuPressed[ArrUp] = false;
	}

	//  if up enter key is pressed
	if (MenuPressed[ArrEnter])
	{
		if (InMenu)
		{
			// Erase the screen of the menu only (Config menu is the first one)
			printGeneralDisplay(tft);
			changed_option_subMenu = true;
		}

		InMenu = false;
		MenuPressed[ArrEnter] = false;
	}

	// if up Esc key is pressed
	if (MenuPressed[ArrEsc])
	{
		if (!InMenu && !config_option_selected)
		{
			printGeneralDisplay(tft);
			// Repaint all the menu icons
			for (int i = 0; i < 6; i++)
			{
				printMenuOptionNumber(tft, i, false);
			}

			// Reset the menu
			option_selected = 0;
			option_selected_submenu = 0;
			changed_option = true;
			if (InSubConfig)
			{
				InSubConfig = false;
				option_selected_submenu = 0;
			}
			else
			{
				InMenu = true;
			}
		}
		else if (config_option_selected)
		{
			// A Option is selected and we need to go back to the menu
			config_option_selected = false;
		}
		MenuPressed[ArrEsc] = false;
	}

	// Print the last option in normal mode
	if (changed_option || changed_option_subMenu)
	{
		if (InMenu)
		{
			// Print the menu with the new option selected
			old_option_selected = printMenuOptionNumber(tft, old_option_selected, false);
			option_selected = printMenuOptionNumber(tft, option_selected, true);
		}
		else
		{
			// Print the menu with the new option selected
			old_option_selected = printSubMenuOptionNumber(tft, old_option_selected, option_selected_submenu, false);
			option_selected = printSubMenuOptionNumber(tft, option_selected, option_selected_submenu, true);

			// Set the configuration option selected
			ChangedConfig = true;
		}
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
		break;

	case MenuBrightness:
		is_inverted ? printMenuBrightnessDisplayInverted(tft) : printMenuBrightnessDisplay(tft);
		ret_option_selected = MenuBrightness;
		break;

	case MenuLeds:
		is_inverted ? printMenuLedsDisplayInverted(tft) : printMenuLedsDisplay(tft);
		ret_option_selected = MenuLeds;
		break;

	case MenuEnergy:
		is_inverted ? printMenuEnergyDisplayInverted(tft) : printMenuEnergyDisplay(tft);
		ret_option_selected = MenuEnergy;
		break;

	case MenuConnection:
		is_inverted ? printMenuConnectionDisplayInverted(tft) : printMenuConnectionDisplay(tft);
		ret_option_selected = MenuConnection;
		break;

	case MenuInfoHelp:
		is_inverted ? printMenuInfoHelpDisplayInverted(tft) : printMenuInfoHelpDisplay(tft);
		ret_option_selected = MenuInfoHelp;
		break;

	default:
		break;
	}
	return ret_option_selected;
}

int printSubMenuOptionNumber(TFT_eSPI &tft, int _option_selected, int _option_selected_submenu, bool is_inverted)
{
	switch (_option_selected)
	{
	case 0:
		for (int i = 0; i < SizeSubMenuConfig; i++)
		{
			if (i == _option_selected_submenu && is_inverted)
			{
				// That option is selected and we need to print it in inverted mode
				//  Set the cursor in the position of the option and draw a rectangle white and set the text in black
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_WHITE);
				tft.setTextColor(TFT_BLACK);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuConfigText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(varToText(SubMenuConfigVarType[i], SubMenuConfigVar[i]));
				tft.setTextColor(TFT_WHITE);
			}
			else
			{
				// That option is selected and we need to print it in normal mode
				//  Set the cursor in the position of the option and draw a rectangle black and set the text in white
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_BLACK);
				tft.setTextColor(TFT_WHITE);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuConfigText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(varToText(SubMenuConfigVarType[i], SubMenuConfigVar[i]));
			}
		}
		break;

	case 1:
		for (int i = 0; i < SizeSubMenuBrightness; i++)
		{
			if (i == _option_selected_submenu && is_inverted)
			{
				// That option is selected and we need to print it in inverted mode
				// Set the cursor in the position of the option and draw a rectangle white and set the text in black
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_WHITE);
				tft.setTextColor(TFT_BLACK);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuBrightnessText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(varToText(SubMenuBrightnessVarType[i], SubMenuBrightnessVar[i]));
				tft.setTextColor(TFT_WHITE);
			}
			else
			{
				// That option is selected and we need to print it in normal mode
				// Set the cursor in the position of the option and draw a rectangle black and set the text in white
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_BLACK);
				tft.setTextColor(TFT_WHITE);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuBrightnessText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(varToText(SubMenuBrightnessVarType[i], SubMenuBrightnessVar[i]));
			}
		}
		break;

	case 2:
		for (int i = 0; i < SizeSubMenuLeds; i++)
		{
			if (i == _option_selected_submenu && is_inverted)
			{
				// That option is selected and we need to print it in inverted mode
				// Set the cursor in the position of the option and draw a rectangle white and set the text in black
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_WHITE);
				tft.setTextColor(TFT_BLACK);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuLedsText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				if (SubMenuLedsVarType[i] != "rgb")
				{
					tft.print(varToText(SubMenuLedsVarType[i], SubMenuLedsVar[i]));
				}
				else
				{
					tft.fillRect(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10, tft.width() - Option_Value_Position_display.x - 2, 7, *SubMenuLedsVar[i]);
				}
				tft.setTextColor(TFT_WHITE);
			}
			else
			{
				// That option is selected and we need to print it in normal mode
				// Set the cursor in the position of the option and draw a rectangle black and set the text in white
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_BLACK);
				tft.setTextColor(TFT_WHITE);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuLedsText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				if (SubMenuLedsVarType[i] != "rgb")
				{
					tft.print(varToText(SubMenuLedsVarType[i], SubMenuLedsVar[i]));
				}
				else
				{
					tft.fillRect(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10, tft.width() - Option_Value_Position_display.x - 2, 7, *SubMenuLedsVar[i]);
				}
			}
		}
		break;

	case 3:
		for (int i = 0; i < SizeSubMenuEnergy; i++)
		{
			if (i == _option_selected_submenu && is_inverted)
			{
				// That option is selected and we need to print it in inverted mode
				// Set the cursor in the position of the option and draw a rectangle white and set the text in black
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_WHITE);
				tft.setTextColor(TFT_BLACK);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuEnergyText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(varToText(SubMenuEnergyVarType[i], SubMenuEnergyVar[i]));
				tft.setTextColor(TFT_WHITE);
			}
			else
			{
				// That option is selected and we need to print it in normal mode
				// Set the cursor in the position of the option and draw a rectangle black and set the text in white
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_BLACK);
				tft.setTextColor(TFT_WHITE);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuEnergyText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(varToText(SubMenuEnergyVarType[i], SubMenuEnergyVar[i]));
			}
		}
		break;

	case 4:
		for (int i = 0; i < SizeSubMenuConnection; i++)
		{
			if (i == _option_selected_submenu && is_inverted)
			{
				// That option is selected and we need to print it in inverted mode
				// Set the cursor in the position of the option and draw a rectangle white and set the text in black
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_WHITE);
				tft.setTextColor(TFT_BLACK);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuConnectionText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(varToText(SubMenuConnectionVarType[i], SubMenuConnectionVar[i]));
				tft.setTextColor(TFT_WHITE);
			}
			else
			{
				// That option is selected and we need to print it in normal mode
				// Set the cursor in the position of the option and draw a rectangle black and set the text in white
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_BLACK);
				tft.setTextColor(TFT_WHITE);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuConnectionText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(varToText(SubMenuConnectionVarType[i], SubMenuConnectionVar[i]));
			}
		}
		break;

	case 5:
		for (int i = 0; i < SizeSubMenuInfoHelp; i++)
		{
			if (i == _option_selected_submenu && is_inverted)
			{
				// That option is selected and we need to print it in inverted mode
				// Set the cursor in the position of the option and draw a rectangle white and set the text in black
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_WHITE);
				tft.setTextColor(TFT_BLACK);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuInfoHelpText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(varToText(SubMenuInfoHelpVarType[i], nullptr));
				tft.setTextColor(TFT_WHITE);
			}
			else
			{
				// That option is selected and we need to print it in normal mode
				// Set the cursor in the position of the option and draw a rectangle black and set the text in white
				tft.fillRect(General_Screen_display.x, Option_Value_Position_display.y + i * 10, tft.width() - General_Screen_display.x - 2, 7, TFT_BLACK);
				tft.setTextColor(TFT_WHITE);
				tft.setCursor(General_Screen_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(SubMenuInfoHelpText[i]);

				tft.setCursor(Option_Value_Position_display.x, Option_Value_Position_display.y + i * 10);
				tft.print(varToText(SubMenuInfoHelpVarType[i], nullptr));
			}
		}
		break;

	default:
		break;
	}

	return _option_selected;
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

// ################################################## UTILS ##################################################
// Transform the variables into Text, Char to String, int to String, RGB to String and bool to True or False String
String varToText(String varType, int *var)
{
	String ret = "";
	// Check the type of the variable it can be "bool" or "int" or "rgb" or "none"
	if (varType == "bool")
	{
		// Check if var is true or false
		if (*var)
		{
			ret = "True";
		}
		else
		{
			ret = "False";
		}
	}
	else if (varType == "int")
	{
		ret = String(*var);
	}
	else if (varType == "rgb")
	{
		RGB temp;
		temp.color = *var;
		temp.CalculateRGB();
		ret = String(temp.r) + "," + String(temp.g) + "," + String(temp.b);
	}
	else if (varType == "none")
	{
		ret = "+";
	}
	else if (varType == "language")
	{
		if (*var == 0)
		{
			ret = "ENG";
		}
		else
		{
			ret = "ES";
		}
	}

	return ret;
}

// Returns the size of the submenu depending on the menu selected
int GetSizeSubMenu(int Menu)
{
	int ret = 0;
	switch (Menu)
	{
	case 0:
		ret = SizeSubMenuConfig;
		break;

	case 1:
		ret = SizeSubMenuBrightness;
		break;

	case 2:
		ret = SizeSubMenuLeds;
		break;

	case 3:
		ret = SizeSubMenuEnergy;
		break;

	case 4:
		ret = SizeSubMenuConnection;
		break;

	case 5:
		ret = SizeSubMenuInfoHelp;
		break;

	default:
		ret = 0;
		break;
	}
	return ret;
}

// Function to change the parameters of the configuration
void ChangeConfig(int Menu, int SubMenu, bool &changed_option_subMenu, bool right)
{
	switch (Menu)
	{
	case 0:
		ChangeVar(SubMenuConfigVarType[SubMenu], SubMenuConfigVar[SubMenu], changed_option_subMenu, right);
		break;

	case 1:
		ChangeVar(SubMenuBrightnessVarType[SubMenu], SubMenuBrightnessVar[SubMenu], changed_option_subMenu, right);
		displayChanged = true;
		break;

	case 2:
		ChangeVar(SubMenuLedsVarType[SubMenu], SubMenuLedsVar[SubMenu], changed_option_subMenu, right);
		break;

	case 3:
		ChangeVar(SubMenuEnergyVarType[SubMenu], SubMenuEnergyVar[SubMenu], changed_option_subMenu, right);
		break;

	case 4:
		ChangeVar(SubMenuConnectionVarType[SubMenu], SubMenuConnectionVar[SubMenu], changed_option_subMenu, right);
		connectionChanged = true;
		break;

	case 5:
		ChangeVar(SubMenuInfoHelpVarType[SubMenu], nullptr, changed_option_subMenu, right);
		break;

	default:
		break;
	}

	// Apply the changes
	ApplyChanges(Menu, SubMenu);
}

void ChangeVar(String varType, int *var, bool &changed_option_subMenu, bool right)
{
	// Bool variable: True to False and False to True
	if (varType == "bool")
	{
		*var = !*var;
		changed_option_subMenu = true;
	}

	// Int variable: Increase the value
	else if (varType == "int")
	{
		if (right)
		{
			*var = modulo_p((*var + 1), 101);
		}
		else
		{
			*var = modulo_p((*var - 1), 101);
		}
		changed_option_subMenu = true;
	}

	// RGB variable: Increase the value
	else if (varType == "rgb")
	{
		if (right)
		{
			// Using HSV
			LedsColor.h += 0.01;
			if (LedsColor.h >= 1.0)
			{
				LedsColor.h -= 1.0;
			}
			LedsColor.hsvToRgb();
			LedsColor.CalculateColor();
		}
		else
		{
			// Using HSV
			LedsColor.h -= 0.01;
			if (LedsColor.h <= -1.0)
			{
				LedsColor.h += 1.0;
			}
			LedsColor.hsvToRgb();
			LedsColor.CalculateColor();
		}
		changed_option_subMenu = true;
	}

	// language variable : Change the language
	else if (varType == "language")
	{
		if (right)
		{
			*var = modulo_p((*var + 1), 2);
		}
		else
		{
			*var = modulo_p((*var - 1), 2);
		}
		changed_option_subMenu = true;
	}
}

// Apply the changes of the configuration selected that have been changed
void ApplyChanges(int Menu, int SubMenu)
{
	if ((Menu == 1 && SubMenu == _BrightnessLeds) || (Menu == 2))
	{
		float brightness = (*SubMenuBrightnessVar[_BrightnessLeds] / 100.0f);
		for (int i = 0; i < NUMBER_OF_LEDS; i++)
		{
			RgbLED.setPixelColor(i, (int)(LedsColor.r * brightness),
								 (int)(LedsColor.g * brightness),
								 (int)(LedsColor.b * brightness));
		}
		// If led is on
		if (*SubMenuLedsVar[_EnableLeds] == 0)
		{
			RgbLED.clear();
		}

		RgbLED.show();
	}

	if (Menu == 5 && SubMenu == _DefaultConfig && configurationReseted)
	{
		// Reset the configurationReseted variable
		configurationModernWood.begin("MWConfig", false);
		configurationModernWood.clear();
		configurationModernWood.end();
		// Reset the board
		ESP.restart();
	}
	else if (Menu == 5 && SubMenu == _DefaultConfig)
	{
		configurationReseted = true;
	}
}

void rainbowEffect(Adafruit_NeoPixel &leds)
{
	static unsigned long lastUpdate = 0;
	unsigned long now = millis();
	float hueStep = 1.0 / NUMBER_OF_LEDS;
	float speed = LedsSpeed / 50000.0;		   // Ajusta la velocidad
	float brightness = LedsBrightness / 100.0; // Ajusta el brillo

	if (now - lastUpdate > 20)
	{ // Actualizar cada 20ms
		lastUpdate = now;
		for (int i = 0; i < leds.numPixels(); i++)
		{
			float hue = fmod(now * speed + i * hueStep, 1.0);
			RGB color(hue, 1.0, brightness); // Hue, Saturation, Brightness
			leds.setPixelColor(i, leds.Color(color.r, color.g, color.b));
		}
		leds.show();
	}
}

void exitModule()
{
	inExternalFunctionMode = false;
	executingCustomFunction = false;
	actualFunctionRow = 0;
	actualFunctionCol = 0;
}