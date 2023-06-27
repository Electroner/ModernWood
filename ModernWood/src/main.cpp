#include <Arduino.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <BleKeyboard.h>
#include <Keys.h>
//For Disabling the wifi
#include <WiFi.h>

BleKeyboard bleKeyboard("ModernWood", "Carlos Lopez", 100);
USBHIDKeyboard Keyboard;

void setup() {
	Serial.begin(115200);
	Serial.println("Starting BLE work!");
	bleKeyboard.begin();
	Keyboard.begin();
	USB.begin();

	//Disable the bluetooth controller on the ESP32
	//btStop();
	//Disable the wifi controller on the ESP32	
	WiFi.mode(WIFI_OFF);
}

void loop()
{

}