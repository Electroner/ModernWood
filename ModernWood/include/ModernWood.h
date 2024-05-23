#pragma once

#include <Arduino.h>
#include <math.h>
#include <algorithm>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <BleKeyboard.h>
#include <SPI.h>
#include <TFT_eSPI.h>
// For wifi disable
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#include <Keys.h>
#include <Images.h>

#include <Extra.h>
#include <customRGB.h>

#include <Preferences.h>
#include <esp_sleep.h>

//Modules
#include "ModulesMap.h"

// ################################################## CONFIGURATION PREFERENCES ##################################################

extern Preferences configurationModernWood;
extern bool ChangedConfig;

void loadUserConfiguration(int Menu, int Option);
void loadUserConfiguration();

void saveUserConfiguration(int Menu, int Option);
void saveUserConfiguration();

// ################################################## LED INDICATOR ##################################################

#define LED_INDICATOR_ENABLED
#define PIN_LED_INDICATOR 48 //Pin used to control the 
#define NUMBER_OF_LEDS 1
extern Adafruit_NeoPixel RgbLED;

extern int LedsBrightness;
extern int LedsActive;
extern RGB LedsColor;
extern int LedsMode;
extern int LedsSpeed;
const float HueStep = 0.01;
const float Saturation = 1.0;
const float ValueBrightness = 1.0;

extern int interval;
extern unsigned long lastUpdateTimeRainbow;

// ################################################## BATTERY ##################################################

#define BATTERY_CHECK //Define if we want to check the battery level
#define PIN_BATTERY 1 //Pin used to read the battery level
#define BATTERY_DIVIDER_VOLTAGE 3.7
#define BATTERY_DIVIDER_VOLTAGE_DISCHARGED 3.3

#define BATTERY_DIVIDER_RESISTOR1 270000
#define BATTERY_DIVIDER_RESISTOR2 1000000
#define BATTERY_CHECK_INTERVAL 60000 // ms

extern int BatteryEnabled;
extern uint8_t volatile batteryLevel;
extern uint8_t volatile batteryLevelLast;
extern bool batteryLevelChanged;
extern portMUX_TYPE timerMux;

void IRAM_ATTR checkBatteryLevel();

//Energy Save Mode
extern int EnergySaveMode;
extern bool timerSetupDone;
extern hw_timer_t *timer;
extern unsigned long lastKeyPressTime;

void IRAM_ATTR onKeyPress();
void setupTimer();
void checkEnergySaveMode();
void enterEnergySaveMode();
void wakeupHandler();

// ################################################## USB HID ##################################################

#define USB_CHECK
#define PIN_USB_CONNECTED 2 //Pin used by the tp4056 to indicate that the battery is charging and the USB is connected

extern int BLEEnabled;
extern int isBLEPreferred;
extern int isUSBPreferred;
extern bool connectionChanged;

extern bool volatile isUSBConnected;
extern bool volatile isBLEConnected;

void IRAM_ATTR USBConnected();

void IRAM_ATTR USBDisconnected();

// ################################################## DISPLAY ##################################################

//#define TFT_SCL 42  	// Pin SPI Clock (SCLK)
//#define TFT_SDA 41  	// Pin SPI out data (MOSI)
//#define TFT_RES 40    // Pin restart (RST)
//#define TFT_DC 39     // Pin data selection (DC)
//#define TFT_CS 38    	// Pin SPI selection chip (CS)
#define BLK_SCREEN 47   // GND -> OFF

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
extern TFT_eSPI tft;
extern int DisplayEnabled;
extern int LanguageMenu;
extern int DisplayBrightness;
extern bool displayChanged;

// ################################################## KEYBOARD ##################################################

#define KEYBOARD_DISPLAY_SWITCH 10
#define DEBOUNCE_DELAY_FN 200

extern bool volatile WorkingAsKeyboard;
extern bool volatile interrupted_FN;
//millis var to debounce
extern unsigned int volatile last_interrupt_FN_time;

extern int KeyboardEnabled;

void IRAM_ATTR FNKeyboardDisplay();

// ################################################## OTHER ##################################################

extern bool inExternalFunctionMode; //To know if we are in the external function mode
extern bool executingCustomFunction; //To know if we are executing a custom function
//If there is a actual function begin exectued, which is the row and col of the key
extern int actualFunctionRow; 
extern int actualFunctionCol;

extern bool configurationReseted;

#define COD0 4  //Output pin X0 (GPIO4)
#define COD1 5  //Output pin X1 (GPIO5)
#define COD2 6  //Output pin X2 (GPIO6)
#define COD3 7  //Output pin X3 (GPIO7)

#define E0 15   //Input pin E0 
#define E1 16   //Input pin E1 
#define E2 17   //Input pin E2 
#define E3 18   //Input pin E3
#define E4 8    //Input pin E4
#define E5 9    //Input pin E5

// Array de lectura de Fila
const unsigned int ESwitch[KEYBOARDHEIGHT] = {E0, E1, E2, E3, E4, E5}; 

extern int TiempoDebounce;
extern bool SwitchState[KEYBOARDHEIGHT][KEYBOARDWIDTH];
extern bool SwitchLastState[KEYBOARDHEIGHT][KEYBOARDWIDTH];
extern unsigned long Debounce[KEYBOARDHEIGHT][KEYBOARDWIDTH];

//Config And menu
enum KeysDistribution {ArrEnter, ArrUp, ArrLeft, ArrDown, ArrRight, ArrEsc};
enum Menus {MenuConfig, MenuBrightness, MenuLeds, MenuEnergy, MenuConnection, MenuInfoHelp};

//Size of the Submenus (number of options, the last one is the size of the submenu, and has to be the last one)
enum SubMenuConfig {_EnableDisplayOption, _EnableKeyboardOption, _DebounceOption, _LanguageMenuOption, SizeSubMenuConfig};
const String SubMenuConfigText[SizeSubMenuConfig] = {"Enable Display", "Enable Keyboard", "Debounce (ms)", "Language"};
const String SubMenuConfigKeys[SizeSubMenuConfig] = {"ENADIS", "ENAKEY", "DEBNCE", "LANGUA"};
const String SubMenuConfigVarType[SizeSubMenuConfig] = {"bool", "bool", "int", "language"};
extern int* SubMenuConfigVar[SizeSubMenuConfig];

enum SubMenuBrightness {_BrightnessLeds, _BrightnessDisplay, SizeSubMenuBrightness}; 
const String SubMenuBrightnessText[SizeSubMenuBrightness] = {"Brightness Leds", "Brightness Display"};
const String SubMenuBrightnessKeys[SizeSubMenuBrightness] = {"BRILED", "BRIDIS"};
const String SubMenuBrightnessVarType[SizeSubMenuBrightness] = {"int", "int"};
extern int* SubMenuBrightnessVar[SizeSubMenuBrightness];

enum SubMenuLeds {_EnableLeds, _LedsColor, _LedsMode, _LedsSpeed, SizeSubMenuLeds};
const String SubMenuLedsText[SizeSubMenuLeds] = {"Enable Leds", "Leds Color", "Leds Mode", "Leds Speed"};
const String SubMenuLedsKeys[SizeSubMenuLeds] = {"ENALED", "LEDCOL", "LEDMOD", "LEDSPE"};
const String SubMenuLedsVarType[SizeSubMenuLeds] = {"bool", "rgb", "int", "int"};
extern int* SubMenuLedsVar[SizeSubMenuLeds];

enum SubMenuEnergy {_EnableBattery, _DisplayMode, SizeSubMenuEnergy};
const String SubMenuEnergyText[SizeSubMenuEnergy] = {"Enable Battery", "Energy Save"};
const String SubMenuEnergyKeys[SizeSubMenuEnergy] = {"ENABAT", "ENGSAV"};
const String SubMenuEnergyVarType[SizeSubMenuEnergy] = {"bool", "int"};
extern int* SubMenuEnergyVar[SizeSubMenuEnergy];

enum SubMenuConnection {_EnableBle, _PreferenceBle, _PreferenceUSB, SizeSubMenuConnection};
const String SubMenuConnectionText[SizeSubMenuConnection] = {"Enable Ble", "Preference Ble", "Preference USB"};
const String SubMenuConnectionKeys[SizeSubMenuConnection] = {"ENABLE", "PREBLE", "PREUSB"};
const String SubMenuConnectionVarType[SizeSubMenuConnection] = {"bool", "bool", "bool"};
extern int* SubMenuConnectionVar[SizeSubMenuConnection];

enum SubMenuInfoHelp {_DefaultConfig, _EspecialMode, _Info, _Help, SizeSubMenuInfoHelp};
const String SubMenuInfoHelpText[SizeSubMenuInfoHelp] = {"Default Config", "Especial Modes", "Info", "Help"};
const String SubMenuInfoHelpKeys[SizeSubMenuInfoHelp] = {"DEFCON", "ESPMODE", "KSINFO", "KSHELP"};
const String SubMenuInfoHelpVarType[SizeSubMenuInfoHelp] = {"none", "none", "none", "none"};

//What are the positions of the keys that control the menu
#define KeyMenuEnterRow 2 //index
#define KeyMenuEnterCol 13 //index
#define KeyMenuUpRow 4 //index (-1 to position), lowest index of the 4 keys
#define KeyMenuUpCol 14 //index (-1 to position)
#define KeyMenuLeftRow 5 //index (-1 to position)
#define KeyMenuLeftCol 13 //index (-1 to position) Lowest index of the 4 keys
#define KeyMenuDownRow 5 //index (-1 to position)
#define KeyMenuDownCol 14 //index (-1 to position)
#define KeyMenuRightRow 5 //index (-1 to position)
#define KeyMenuRightCol 15 //index (-1 to position)
#define KeyMenuEscRow 0 //index (-1 to position)
#define KeyMenuEscCol 0 //index (-1 to position)

extern int option_selected;
extern int option_selected_submenu;

//Registers for the keys
extern bool MenuPressed[6]; //Enter, Up, Left, Down, Right
extern bool KeysPressedConfig[6]; //Enter, Up, Left, Down, Right
extern bool KeysPressedConfigLast[6]; //Enter, Up, Left, Down, Right
extern unsigned long KeysPressedConfigDebounce[6]; //Enter, Up, Left, Down, Right
extern bool InMenu;

const bool nums0_15[NUMSDEMUSIZE][NUMSDEMUWIDTH] = {{0, 0, 0, 0},
                                                {1, 0, 0, 0},
                                                {0, 1, 0, 0},
                                                {1, 1, 0, 0},
                                                {0, 0, 1, 0},
                                                {1, 0, 1, 0},
                                                {0, 1, 1, 0},
                                                {1, 1, 1, 0},
                                                {0, 0, 0, 1},
                                                {1, 0, 0, 1},
                                                {0, 1, 0, 1},
                                                {1, 1, 0, 1},
                                                {0, 0, 1, 1},
                                                {1, 0, 1, 1},
                                                {0, 1, 1, 1},
                                                {1, 1, 1, 1}};

void WorkingInExternalFunctionMode(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected);

void WorkingModeKeyboard(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected);

void WorkingModeDisplay(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected);

//Prints the Main menu options in the normal mode or inverted mode
int printMenuOptionNumber(TFT_eSPI &tft, int _option_selected, bool is_inverted);

//Prints the Sub menu options in the normal mode or inverted mode
int printSubMenuOptionNumber(TFT_eSPI &tft, int _option_selected,int _option_selected_submenu, bool is_inverted);

//Poisition of the configuration icon
void printMenuConfigDisplay(TFT_eSPI &tft);

//Poisition of the Brightness icon
void printMenuBrightnessDisplay(TFT_eSPI &tft);

//Poisition of the Leds icon
void printMenuLedsDisplay(TFT_eSPI &tft);
				
//Poisition of the Energy icon
void printMenuEnergyDisplay(TFT_eSPI &tft);

//Poisition of the Connection icon
void printMenuConnectionDisplay(TFT_eSPI &tft);

//Poisition of the InfoHelp icon
void printMenuInfoHelpDisplay(TFT_eSPI &tft);

//INVERTED ICONS
//Poisition of the configuration icon
void printMenuConfigDisplayInverted(TFT_eSPI &tft);

//Poisition of the Brightness icon
void printMenuBrightnessDisplayInverted(TFT_eSPI &tft);

//Poisition of the Leds icon
void printMenuLedsDisplayInverted(TFT_eSPI &tft);
				
//Poisition of the Energy icon
void printMenuEnergyDisplayInverted(TFT_eSPI &tft);

//Poisition of the Connection icon
void printMenuConnectionDisplayInverted(TFT_eSPI &tft);

//Poisition of the InfoHelp icon
void printMenuInfoHelpDisplayInverted(TFT_eSPI &tft);

//Print the menus and general part of the display black
void printGeneralDisplay(TFT_eSPI &tft);

// ################################################## UTILS ##################################################
//Transform the variables into Text
String varToText(String varType, int *var);

//Get the size of the submenu
int GetSizeSubMenu(int Menu);

//Change the config
void ChangeConfig(int Menu, int SubMenu, bool &changed_option_subMenu, bool right);

//Change the var depending on the type
void ChangeVar(String varType, int *var, bool &changed_option_subMenu, bool right);

//Apply the changes
void ApplyChanges(int Menu, int SubMenu);

// Leds Functions for rainbow effect
void rainbowEffect(Adafruit_NeoPixel& leds);

//Exit the special Function Mode (Macros, Media, etc)
void exitModule();