#include <Arduino.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <BleKeyboard.h>
#include <Keys.h>
// For wifi disable
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel RgbLED = Adafruit_NeoPixel(1, 48, NEO_GRB + NEO_KHZ800);

#define DEBUG

// BATTERY
#define BATTERY_CHECK
#define PIN_BATTERY 8
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
	Serial.println(batteryLevel);
	portEXIT_CRITICAL_ISR(&timerMux);
}

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

	//Pins Configuration
	RgbLED.begin();
	RgbLED.setBrightness(50);
	RgbLED.show(); // Initialize all pixels to 'off'

}

uint32_t Wheel(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if(WheelPos < 85) {
		return RgbLED.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if(WheelPos < 170) {
		WheelPos -= 85;
		return RgbLED.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return RgbLED.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
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

	while (true)
	{
		//Program
		uint16_t i, j;

		for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
			for(i=0; i< RgbLED.numPixels(); i++) {
			RgbLED.setPixelColor(i, Wheel(((i * 256 / RgbLED.numPixels()) + j) & 255));
			}
			RgbLED.show();
			delay(20);
		}
	}
}