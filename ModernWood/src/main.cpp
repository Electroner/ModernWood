#include <ModernWood.h>

#define DEBUG

// ################################################## LED INDICATOR ##################################################

#define LED_INDICATOR_ENABLED
#define PIN_LED_INDICATOR 48
Adafruit_NeoPixel RgbLED = Adafruit_NeoPixel(1, PIN_LED_INDICATOR, NEO_GRB + NEO_KHZ800);

// ################################################## BATTERY ##################################################

#define BATTERY_CHECK
#define PIN_BATTERY 1
#define BATTERY_DIVIDER_VOLTAGE 4.2
#define BATTERY_DIVIDER_VOLTAGE_DISCHARGED 2.6
#define BATTERY_DIVIDER_RESISTOR1 270000
#define BATTERY_DIVIDER_RESISTOR2 1000000
#define BATTERY_CHECK_INTERVAL 10000 // ms

uint8_t volatile batteryLevel = 100;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR checkBatteryLevel()
{
	portENTER_CRITICAL_ISR(&timerMux);
	// We are going to use a voltage divider to measure the battery with a resistance of 1M and another of 270K and the voltage is 4.2V
	// Vout = (4.2*1M)/(270k + 1M) = 3.3V , 3.3V -> 4095 , 2.6V -> 3250
	batteryLevel = map(static_cast<long>(analogRead(PIN_BATTERY)), 3250.0f, 4095.0f, 0.0, 100.0);
	//Serial.println(batteryLevel);
	portEXIT_CRITICAL_ISR(&timerMux);
}

// ################################################## USB HID ##################################################

#define USB_CHECK
#define PIN_USB_CONNECTED 2 //Pin used by the tp4056 to indicate that the battery is charging and the USB is connected

bool volatile isUSBConnected = true;
bool volatile isBLEConnected = false;

void IRAM_ATTR USBConnected()
{
	isUSBConnected = true;
	isBLEConnected = false;
}

void IRAM_ATTR USBDisconnected()
{
	isUSBConnected = false;
	isBLEConnected = true;
}

// ################################################## DISPLAY ##################################################

//#define TFT_SCL 42  	// Pin de reloj SPI (SCLK)
//#define TFT_SDA 41  	// Pin de salida de datos SPI (MOSI)
//#define TFT_RES 40    // Pin de reinicio (RST)
//#define TFT_DC 39     // Pin de selección de comando/datos (DC)
//#define TFT_CS 38    	// Pin de selección de chip SPI (CS)

//0,0							160,0
//X---------------------------X
//|							|
//|							|
//|							|
//|							|
//X---------------------------X
//0,80						160,80


#define DISPLAY_ENABLED
#define DISPLAY_WIDTH 80
#define DISPLAY_HEIGHT 160
#define DISPLAY_ROTATION 0
TFT_eSPI tft = TFT_eSPI(DISPLAY_WIDTH, DISPLAY_HEIGHT); // Invoke custom library

// ################################################## KEYBOARD ##################################################

#define KEYBOARD_DISPLAY_SWITCH 10
#define DEBOUNCE_DELAY_FN 200

bool volatile WorkingAsKeyboard = true;
bool volatile interrupted_FN = false;
//millis var to debounce
unsigned int volatile last_interrupt_FN_time = 0;

void IRAM_ATTR FNKeyboardDisplay()
{
	unsigned int interrupt_time = millis();
	if (interrupt_time - last_interrupt_FN_time > DEBOUNCE_DELAY_FN){
		WorkingAsKeyboard = !WorkingAsKeyboard;
		interrupted_FN = true;
		last_interrupt_FN_time = interrupt_time;
	}
}

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
	tft.begin();
	tft.setRotation(3);
	tft.setFreeFont(&FreeSansBold9pt7b); //Altura de la fuente 12
	tft.setTextColor(TFT_WHITE);

	tft.fillScreen(TFT_BLACK);
	
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
	//configuration icon
	printMenuConfigDisplay(tft);

	//Brightness icon
	printMenuBrightnessDisplay(tft);

	//Leds icon
	printMenuLedsDisplay(tft);
				
	//Energy icon
	printMenuEnergyDisplay(tft);

	//Connection icon
	printMenuConnectionDisplay(tft);

	//InfoHelp icon
	printMenuInfoHelpDisplay(tft);

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