#include <ModernWood.h>

#define DEBUG

#ifdef DEBUG
#include "driver/temp_sensor.h"
#endif

// ################################################## MAIN ##################################################

void setup()
{
	// Load the all User Preferences configuration from the EEPROM
	configurationModernWood.begin("MWConfig", false);
	loadUserConfiguration();
	configurationModernWood.end();

#ifdef DEBUG
	//TEMPERATURE SENSOR ON CHIP
	temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor.dac_offset = TSENS_DAC_L2;  // TSENS_DAC_L2 is default; L4(-40°C ~ 20°C), L2(-10°C ~ 80°C), L1(20°C ~ 100°C), L0(50°C ~ 125°C)
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
#endif

	// Disable WiFi
	WiFi.mode(WIFI_OFF);

#ifdef DEBUG
	Serial.begin(115200);
	Serial.println("ModernWood Keyboard");
#endif

	// Disable Bluetooth Function
	// btStop();

	// Battery
#ifdef BATTERY_CHECK
	// Create a timer object using hw_timer_t
	hw_timer_t *timer = NULL;
	// Attach onTimer function to our timer.
	// 0 : timer number
	// 80 : 80 divider. 1 tick = 1 micro second. So this will make our timer run at 1Mhz
	// true : Count up
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &checkBatteryLevel, true);
	timerAlarmWrite(timer, BATTERY_CHECK_INTERVAL * 1000, true); // 60 seconds interval
	// Enable our timer.
	timerAlarmEnable(timer);
#endif

	// USB
#ifdef USB_CHECK
	pinMode(PIN_USB_CONNECTED, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(PIN_USB_CONNECTED), USBConnected, RISING);
	attachInterrupt(digitalPinToInterrupt(PIN_USB_CONNECTED), USBDisconnected, FALLING);
#endif

	// Keyboard
	pinMode(KEYBOARD_DISPLAY_SWITCH, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(KEYBOARD_DISPLAY_SWITCH), FNKeyboardDisplay, FALLING); // Function KEY Pushed (PullUp)

	pinMode(COD0, OUTPUT);
	pinMode(COD1, OUTPUT);
	pinMode(COD2, OUTPUT);
	pinMode(COD3, OUTPUT);

	pinMode(E0, INPUT_PULLUP);
	pinMode(E1, INPUT_PULLUP);
	pinMode(E2, INPUT_PULLUP);
	pinMode(E3, INPUT_PULLUP);
	pinMode(E4, INPUT_PULLUP);
	pinMode(E5, INPUT_PULLUP);

	// Led Configuration
	LedsColor.CalculateRGB(); // Color is in int format from the EEPROM
	RgbLED.begin();
	float brightness = (*SubMenuBrightnessVar[_BrightnessLeds] / 100.0f);
	RgbLED.setPixelColor(0, (int)(LedsColor.r * brightness),
							(int)(LedsColor.g * brightness),
							(int)(LedsColor.b * brightness));
	if (*SubMenuLedsVar[_EnableLeds] == 0)
	{
		RgbLED.clear();
	}
	RgbLED.show();

	// Display
#ifdef DISPLAY_ENABLED
	// Dissable the display Image for noise with BLK_SCREEN pin
	pinMode(BLK_SCREEN, OUTPUT);

	digitalWrite(BLK_SCREEN, LOW);
	tft.begin();
	tft.fillScreen(TFT_BLACK);

	tft.setRotation(3);
	tft.setFreeFont(&FreeSansBold9pt7b); // Altura de la fuente 12
	tft.setTextColor(TFT_WHITE);
	delay(50);
	digitalWrite(BLK_SCREEN, HIGH);

	// Show in the screen the Image of the keyboard icon in the center 80x80 pixels in the Images.h
	tft.pushImage(40, 0, 80, 80, image_Icon, 0);
	delay(1000);
	tft.fillScreen(TFT_BLACK);

	// Draw a line from 0,24 to 160,24
	tft.drawLine(0, SPLIT_LINE, DISPLAY_HEIGHT, SPLIT_LINE, TFT_WHITE);

	// Show the image_USB in the screen int the position 1,1 33x15 pixels
	tft.fillRect(USB_display_image.x,
				 USB_display_image.y,
				 USB_display_image.width,
				 USB_display_image.height,
				 TFT_BLACK);
	tft.pushImage(USB_display_image.x,
				  USB_display_image.y,
				  USB_display_image.width,
				  USB_display_image.height,
				  image_USB, 0);

	// Show the FNKeyboard
	tft.pushImage(KeyboardFN_display_image.x,
				  KeyboardFN_display_image.y,
				  KeyboardFN_display_image.width,
				  KeyboardFN_display_image.height,
				  image_KeyboardFN, 0);

	// Position the cursor in the position 75,22 and print the text "100%"
	tft.setCursor(BATTERY_LEVEL_X, BATTERY_LEVEL_Y);
	tft.print("100%");

	// Show the image_Battery in the screen int the position 1,127 34x19 pixels
	tft.pushImage(Battery_display_image.x,
				  Battery_display_image.y,
				  Battery_display_image.width,
				  Battery_display_image.height,
				  image_Battery, 0);

	tft.fillRect(Battery_Inside_display_image.x, 
						 Battery_Inside_display_image.y,
						 Battery_Inside_display_image.width, 
						 Battery_Inside_display_image.height, TFT_GREEN);

	// MENU
	// configuration icons
	for (int i = 0; i < 6; i++)
	{
		printMenuOptionNumber(tft, i, false);
	}

	// Reset the font to the default
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setFreeFont(NULL);
	tft.setTextSize(1);

#endif

	// Set the keyboard Functions to noneFN and default
	inExternalFunctionMode = false;
	executingCustomFunction = false;
	actualFunctionRow = 0;
	actualFunctionCol = 0;

	// Energy Save mode
	attachInterrupt(digitalPinToInterrupt(E0), onKeyPress, FALLING);
    attachInterrupt(digitalPinToInterrupt(E1), onKeyPress, FALLING);
    attachInterrupt(digitalPinToInterrupt(E2), onKeyPress, FALLING);
    attachInterrupt(digitalPinToInterrupt(E3), onKeyPress, FALLING);
    attachInterrupt(digitalPinToInterrupt(E4), onKeyPress, FALLING);
    attachInterrupt(digitalPinToInterrupt(E5), onKeyPress, FALLING);

	// Set and activate timer only if EnergySaveMode is true
    if (EnergySaveMode)
	{
        setupTimer();
    }
}

void loop()
{
	// Keyboards USB/BLE
	BleKeyboard bleKeyboard("ModernWood", "Electroner", 100);
	USBHIDKeyboard Keyboard;

	// Start USB and BLE Keyboards
	if(BLEEnabled)
	{
		bleKeyboard.begin();
	}
	Keyboard.begin();
	USB.begin();

#ifdef DEBUG
	Serial.println("USB and BLE Keyboards Started");
	unsigned int loop_counter = 0;
	unsigned int last_loop_time = millis();
#endif

	while (true)
	{

#ifdef DEBUG
		// For debug purposes show how many times the loop is executed per second
		loop_counter++;
		if (millis() - last_loop_time > 1000)
		{
			Serial.print("Loop executed ");
			Serial.print(loop_counter);
			Serial.println(" times per second");

			Serial.print("Temperature: ");
			float result = 0;
			temp_sensor_read_celsius(&result);
			Serial.print(result);
			Serial.println(" °C");

			loop_counter = 0;
			last_loop_time = millis();
		}

		//Read from serial
		char c = 'N';
		if (Serial.available())
		{
			c= Serial.read();
			Serial.print("Read from serial: ");
			Serial.println(c);
		}
		//wasd -> ARRIBA ABAJO DERECHA IZQUIERDA
		//Espace -> Enter
		//E -> Escape
		//Q -> MODO ESPECIAL
		switch (c)
		{
		case 'Q':
			WorkingAsKeyboard = !WorkingAsKeyboard;
			interrupted_FN = true;
			break;
		
		case 'E':
			MenuPressed[ArrEsc] = true;
			break;

		case ' ':
			MenuPressed[ArrEnter] = true;
			break;

		case 'W':
			MenuPressed[ArrUp] = true;
			break;

		case 'S':
			MenuPressed[ArrDown] = true;
			MenuPressed[ArrDown] = true;
			break;

		case 'A':	
			MenuPressed[ArrLeft] = true;
			break;

		case 'D':	
			MenuPressed[ArrRight] = true;
			break;

		case '1':
			batteryLevel--;
			batteryLevelChanged = true;
			break;

		case '2':
			batteryLevel++;
			batteryLevelChanged = true;
			break;

		case '3':
			connectionChanged = true;
			isUSBPreferred = !isUSBPreferred;
			isBLEPreferred = !isBLEPreferred;
			break;

		case '4':
			connectionChanged = true;
			isBLEConnected = !isBLEConnected;
			break;

		default:
			break;
		}
		
#endif

		// ################################################## LOOP LOGIC ##################################################
		// Energy Save mode logic
		// If the Energy Save mode is enabled, enable the timer
		if (EnergySaveMode && !timerSetupDone)
		{
			setupTimer();
			timerSetupDone = true;
		}
		else if (!EnergySaveMode && timerSetupDone)
		{
			// If the Energy Save mode is disabled, disable the timer
			if (timer != NULL)
			{
				timerAlarmDisable(timer);
				timerEnd(timer);
				timer = NULL;
				timerSetupDone = false;
			}
		}
		
		//Battery logic to update the battery level
		if(batteryLevelChanged && BatteryEnabled)
		{
			#ifdef DEBUG
			//If the battery level is less than 0% set it to 0%
			if(batteryLevel < 0)
			{
				batteryLevel = 0;
			}
			//If the battery level is more than 100% set it to 100%
			if(batteryLevel > 100)
			{
				batteryLevel = 100;
			}
			#endif

			tft.setFreeFont(&FreeSansBold9pt7b); // Altura de la fuente 12
			tft.setTextColor(TFT_WHITE);

			// Print black the battery level in the screen and the percentage
			tft.fillRect(Battery_Inside_display_image.x, 
						 Battery_Inside_display_image.y,
						 Battery_Inside_display_image.width, 
						 Battery_Inside_display_image.height, TFT_BLACK);
			tft.fillRect(BATTERY_LEVEL_X,
						 BATTERY_LEVEL_Y - BATTERY_LEVEL_HEIGHT - 1,
						 BATTERY_LEVEL_WIDTH, 
						 BATTERY_LEVEL_HEIGHT + 2, TFT_BLACK);
			
			tft.setCursor(BATTERY_LEVEL_X, BATTERY_LEVEL_Y);
			if(batteryLevel == 100)
			{
				tft.print(String(batteryLevel) + "%");
			}
			else if(batteryLevel < 100 && batteryLevel >= 10)
			{
				tft.print(String(batteryLevel) + " %");
			}
			else if(batteryLevel < 10)
			{
				tft.print(String(batteryLevel) + "  %");
			}
			batteryLevelChanged = false;
			
			//Print black rectangle to erase the battery level
			tft.fillRect(Battery_Inside_display_image.x, 
							Battery_Inside_display_image.y,
							Battery_Inside_display_image.width, 
							Battery_Inside_display_image.height, TFT_BLACK);
			RGB percolor;
			percolor.r = 255 - (255 * batteryLevel) / 100;
			percolor.g = (255 * batteryLevel) / 100;
			percolor.b = 0;
			percolor.CalculateColor();
			// Print the battery level in the screen
			tft.fillRect(Battery_Inside_display_image.x, 
							Battery_Inside_display_image.y,
							static_cast<int>(Battery_Inside_display_image.width * (batteryLevel/100.0f)), 
							Battery_Inside_display_image.height, percolor.color);

			// Reset the font to the default
			tft.setFreeFont(NULL);
			tft.setTextSize(1);
		}

		// Check if the leds are enabled
		if(*SubMenuLedsVar[_EnableLeds] != 0)
		{
			//Check for special functions like led control
			if(*SubMenuLedsVar[_LedsMode] != 0)
			{
				//Check if the led mode is white
				if(*SubMenuLedsVar[_LedsMode] == 1)
				{
					//Show white color
					float brightness = (*SubMenuBrightnessVar[_BrightnessLeds] / 100.0f);
					for(int i = 0; i < NUMBER_OF_LEDS; i++)
					{
						RgbLED.setPixelColor(i, (int)(255 * brightness),
												(int)(255 * brightness),
												(int)(255 * brightness));
					}
					if (*SubMenuLedsVar[_EnableLeds] == 0)
					{
						RgbLED.clear();
					}
					RgbLED.show();
				}
				//Check if the led mode is rainbow
				if(*SubMenuLedsVar[_LedsMode] == 2)
				{
					//Check if the led mode is rainbow
					rainbowEffect(RgbLED);
				}
			}
		}
		
		// Connection logic to update the connection status
		if(*SubMenuConnectionVar[_PreferenceUSB] && connectionChanged)
		{
			tft.fillRect(BLE_display_image.x,
				 BLE_display_image.y,
				 BLE_display_image.width,
				 BLE_display_image.height,
				 TFT_BLACK);
			tft.fillRect(Connection_display_image.x,
				 Connection_display_image.y,
				 Connection_display_image.width,
				 Connection_display_image.height,
				 TFT_BLACK);
			tft.pushImage(USB_display_image.x,
				  USB_display_image.y,
				  USB_display_image.width,
				  USB_display_image.height,
				  image_USB, 0);
			connectionChanged = false;
		}
		else if(connectionChanged)
		{
			tft.fillRect(USB_display_image.x,
				 USB_display_image.y,
				 USB_display_image.width,
				 USB_display_image.height,
				 TFT_BLACK);
			tft.pushImage(BLE_display_image.x,
				  BLE_display_image.y,
				  BLE_display_image.width,
				  BLE_display_image.height,
				  image_BLE, 0);
			// If bluetooth has a connection show the bluetooth connection image
			if(isBLEConnected)
			{
				tft.pushImage(Connection_display_image.x,
				  Connection_display_image.y,
				  Connection_display_image.width,
				  Connection_display_image.height,
				  image_Connection, 0);
			}
			else
			{
				tft.fillRect(Connection_display_image.x,
				 Connection_display_image.y,
				 Connection_display_image.width,
				 Connection_display_image.height,
				 TFT_BLACK);
			}
			connectionChanged = false;
		}

		// Display logic to update the display on/off and brightness
		if(*SubMenuConfigVar[_EnableDisplayOption] && displayChanged)
		{
			// Set the brightness of the screen (ON)
			analogWrite(BLK_SCREEN, *SubMenuBrightnessVar[_BrightnessDisplay] * 2.55);
			displayChanged = false;
		}
		else if(displayChanged)
		{
			// Display OFF
			analogWrite(BLK_SCREEN, LOW);
			displayChanged = false;
		}

		// ################################################## KEYBOARD LOGIC ##################################################
		// Check if keyboard is executing a custom function
		if (inExternalFunctionMode)
		{
			if (executingCustomFunction)
			{
				(*MODULESFUNCARRAY[actualFunctionRow][actualFunctionCol])();
			}
			else
			{
				if (interrupted_FN)
				{
					displayChanged = true;

					// Erase all the screen and print the general display
					printGeneralDisplay(tft);

					// Reset the Macro and special function variables
					actualFunctionRow = 0;
					actualFunctionCol = 0;
					inExternalFunctionMode = false;
					executingCustomFunction = false;
					interrupted_FN = false;
				}
				WorkingInExternalFunctionMode(tft, bleKeyboard, Keyboard, isBLEConnected, isUSBConnected);
			}
		}
		else
		{
			// Check if keyboard is working as Display Mode or Keyboard Mode
			// Also check if is USBPreferred or BLEPreferred
			if (WorkingAsKeyboard)
			{
				// From Display to Keyboard Mode
				if (interrupted_FN)
				{
					displayChanged = true;

					tft.fillRect(KeyboardFN_display_image.x,
								 KeyboardFN_display_image.y,
								 KeyboardFN_display_image.width,
								 KeyboardFN_display_image.height,
								 TFT_BLACK);
					tft.pushImage(KeyboardFN_display_image.x,
								  KeyboardFN_display_image.y,
								  KeyboardFN_display_image.width,
								  KeyboardFN_display_image.height,
								  image_KeyboardFN, 0);
					interrupted_FN = false;

					// Erase all the screen and print the general display
					printGeneralDisplay(tft);

					// Repaint all the menu icons
					for (int i = 0; i < 6; i++)
					{
						printMenuOptionNumber(tft, i, false);
					}

					// Function to set configuration of the keyboard (check BLEEnabled)
					if(!BLEEnabled && btStarted())
					{
						#ifdef DEBUG
						Serial.println("BLE Keyboard Stopped");
						#endif
						btStop();
					}
					else if(BLEEnabled && !btStarted())
					{
						#ifdef DEBUG
						Serial.println("BLE Keyboard Started");
						#endif
						bleKeyboard.begin();
						btStart();
					}

					// Here in the back to the Keyboard Mode we save the configuration if changed has been made
					// Save the changes in the EEPROM opening the configuration descriptor
					if(ChangedConfig)
					{
						if(!configurationReseted)
						{
							saveUserConfiguration();
							#ifdef DEBUG
							Serial.println("Configuration Saved");
							#endif
						}
						else
						{
							// If no double press we dont reset the configuration
							configurationReseted = false;
						}

						ChangedConfig = false;
					}
				}
				WorkingModeKeyboard(tft, bleKeyboard, Keyboard, isBLEConnected, isUSBConnected);
			}
			else
			{
				// From Keyboard to Display Mode
				if (interrupted_FN)
				{
					analogWrite(BLK_SCREEN, 255);

					tft.fillRect(DisplaydFN_display_image.x,
								 DisplaydFN_display_image.y,
								 DisplaydFN_display_image.width,
								 DisplaydFN_display_image.height,
								 TFT_BLACK);
					tft.pushImage(DisplaydFN_display_image.x,
								  DisplaydFN_display_image.y,
								  DisplaydFN_display_image.width,
								  DisplaydFN_display_image.height,
								  image_DisplaydFN, 0);
					interrupted_FN = false;
					if (isUSBConnected)
					{
						Keyboard.releaseAll();
					}
					else
					{
						bleKeyboard.releaseAll();
					}

					// Erase all the screen and print the general display
					printGeneralDisplay(tft);

					// Repaint all the menu icons
					for (int i = 0; i < 6; i++)
					{
						printMenuOptionNumber(tft, i, false);
					}
					// Set option_selected to 0
					option_selected = 0;
					option_selected_submenu = 0;
					InMenu = true;
					// Print the option selected
					printMenuOptionNumber(tft, option_selected, true);

					// Function to set configuration of the keyboard (check BLEEnabled)
					if(!BLEEnabled && btStarted())
					{
						#ifdef DEBUG
						Serial.println("BLE Keyboard Stopped");
						#endif
						btStop();
					}
					else if(BLEEnabled && !btStarted())
					{
						#ifdef DEBUG
						Serial.println("BLE Keyboard Started");
						#endif
						bleKeyboard.begin();
						btStart();
					}
				}
				WorkingModeDisplay(tft, bleKeyboard, Keyboard, isBLEConnected, isUSBConnected);
			}
		}
	}
}

// TODO: Energy save mode