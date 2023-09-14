#include <ModernWood.h>

#define DEBUG

// ################################################## MAIN ##################################################

void setup()
{
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
	//Attach onTimer function to our timer.
	//0 : timer number
	//80 : 80 divider. 1 tick = 1 micro second. So this will make our timer run at 1Mhz
	//true : Count up
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &checkBatteryLevel, true);
	timerAlarmWrite(timer, BATTERY_CHECK_INTERVAL*1000, true);
	//Enable our timer.
	timerAlarmEnable(timer);
#endif

	//USB 
#ifdef USB_CHECK
	pinMode(PIN_USB_CONNECTED, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(PIN_USB_CONNECTED), USBConnected, RISING);
	attachInterrupt(digitalPinToInterrupt(PIN_USB_CONNECTED), USBDisconnected, FALLING);
#endif

	//Keyboard
	pinMode(KEYBOARD_DISPLAY_SWITCH, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(KEYBOARD_DISPLAY_SWITCH), FNKeyboardDisplay, FALLING); //Function KEY Pushed (PullUp)

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

	//Led Configuration
	RgbLED.begin();
	RgbLED.setBrightness(50);
	RgbLED.show(); // Initialize all pixels to 'off'

	//Display
#ifdef DISPLAY_ENABLED
	//Dissable the display Image for noise with BLK_SCREEN pin
	pinMode(BLK_SCREEN, OUTPUT);

	digitalWrite(BLK_SCREEN, LOW);
	tft.begin();
	tft.fillScreen(TFT_BLACK);

	tft.setRotation(3);
	tft.setFreeFont(&FreeSansBold9pt7b); //Altura de la fuente 12
	tft.setTextColor(TFT_WHITE);
	delay(50);
	digitalWrite(BLK_SCREEN, HIGH);
	
	//Show in the screen the Image of the keyboard icon in the center 80x80 pixels in the Images.h
	tft.pushImage(40, 0, 80, 80, image_Icon, 0);
	delay(1000);
	tft.fillScreen(TFT_BLACK);
	
	//Draw a line from 0,24 to 160,24
	tft.drawLine(0, SPLIT_LINE, DISPLAY_HEIGHT, SPLIT_LINE, TFT_WHITE);

	//Show the image_USB in the screen int the position 1,1 33x15 pixels
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

	//Show the FNKeyboard
	tft.pushImage(KeyboardFN_display_image.x,
			  	  KeyboardFN_display_image.y, 
				  KeyboardFN_display_image.width, 
				  KeyboardFN_display_image.height, 
				  image_KeyboardFN, 0);

	//Position the cursor in the position 80,12 and print the text "100%"
	tft.setCursor(BATTERY_LEVEL_X, BATTERY_LEVEL_Y);
	tft.print("100%");

	//Show the image_Battery in the screen int the position 1,127 34x19 pixels
	tft.pushImage(Battery_display_image.x,
				  Battery_display_image.y, 
				  Battery_display_image.width, 
				  Battery_display_image.height, 
				  image_Battery, 0);

	//MENU
	//configuration icons
	for(int i = 0;i < 6; i++){
		printMenuOptionNumber(tft, i, false);
	}

	//Reset the font to the default
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setFreeFont(NULL);
	tft.setTextSize(1);

#endif
}

void loop()
{
	// Keyboards USB/BLE
	BleKeyboard bleKeyboard("ModernWood", "Electroner", 100);
	USBHIDKeyboard Keyboard;

	//Start USB and BLE Keyboards
	bleKeyboard.begin();
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
		//for debug purposes show how many times the loop is executed per second
		loop_counter++;
		if (millis() - last_loop_time > 1000)
		{
			Serial.print("Loop executed ");
			Serial.print(loop_counter);
			Serial.println(" times per second");
			loop_counter = 0;
			last_loop_time = millis();
		}
	#endif

		//Check if keyboard is working as Display Mode or Keyboard Mode
		if(WorkingAsKeyboard)
		{
			if(interrupted_FN){
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

				//Erase all the screen and print the general display
				printGeneralDisplay(tft);

				//Repaint all the menu icons
				for(int i = 0;i < 6; i++){
					printMenuOptionNumber(tft, i, false);
				}
			}
			WorkingModeKeyboard(tft, bleKeyboard, Keyboard, isBLEConnected, isUSBConnected);
		}
		else
		{
			if(interrupted_FN){
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
				if(isUSBConnected){
					Keyboard.releaseAll();
				}
				else
				{
					bleKeyboard.releaseAll();
				}

				//Erase all the screen and print the general display
				printGeneralDisplay(tft);

				//Repaint all the menu icons
				for(int i = 0;i < 6; i++){
					printMenuOptionNumber(tft, i, false);
				}
				//Set option_selected to 0
				option_selected = 0;
				option_choose = 0;
				InMenu = false;
				//Print the option selected
				printMenuOptionNumber(tft, option_selected, true);
			}
			WorkingModeDisplay(tft, bleKeyboard, Keyboard, isBLEConnected, isUSBConnected);
		}
	}
}

/*
-> Menu
	-> Configuration (Dissable Display, Dissable Keyboard, Dissable BLE, Dissable USB)
	-> Brightness (0-100%)
	-> Leds (On/Off, color, brightness)
	-> Energy (Is using battery or not)
	-> Connectivty
		-> Bluetooth (Only BLE)
		-> USB	(Only USB)
	-> About/Help (Type URL of the help page)
*/

//TODO: review if bluethoot disconnect and connect wih usb wired