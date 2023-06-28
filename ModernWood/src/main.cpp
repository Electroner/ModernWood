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

// ################################################## BATTERY ##################################################

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

// ################################################## KEYBOARD ##################################################

#define COD0 0 		//Asignacino del pin de salida X0
#define COD1 0  	//Asignacino del pin de salida X1
#define COD2 0 		//Asignacino del pin de salida X2 
#define COD3 0  	//Asignacino del pin de salida X3 

#define E0 0 	//Asignacino del pin de Entrada E0 
#define E1 0 	//Asignacino del pin de Entrada E1 
#define E2 0 	//Asignacino del pin de Entrada E2 
#define E3 0 	//Asignacino del pin de Entrada E3
#define E4 0 	//Asignacino del pin de Entrada E4
#define E5 0 	//Asignacino del pin de Entrada E5

// Array de lectura de Fila
const unsigned int ESwitch[ALTURATECLADO] = {E0, E1, E2, E3, E4, E5}; 

const unsigned long TiempoDebounce = 5;                             //Tiempo Debounce en milisegundos
bool SwitchEstado[ALTURATECLADO][ANCHURATECLADO] = {false};         //Estado de la lectura de la fila
bool SwitchEstadoAntiguo[ALTURATECLADO][ANCHURATECLADO] = {false};  //Estado Anterior de la Tecla
unsigned long Debounce[ALTURATECLADO][ANCHURATECLADO] = {0};        //Tiempo de Bounce de la fila

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
	pinMode(PIN_USB_CONNECTED, INPUT);
	attachInterrupt(digitalPinToInterrupt(PIN_USB_CONNECTED), USBConnected, RISING);
	attachInterrupt(digitalPinToInterrupt(PIN_USB_CONNECTED), USBDisconnected, FALLING);
#endif

	//Keyboard
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
		
	}
}