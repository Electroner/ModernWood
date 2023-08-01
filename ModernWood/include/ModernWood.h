#pragma once

#include <Arduino.h>
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

#define COD0 4  //Asignacino del pin de salida X0 (GPIO4)
#define COD1 5  //Asignacino del pin de salida X1 (GPIO5)
#define COD2 6  //Asignacino del pin de salida X2 (GPIO6)
#define COD3 7  //Asignacino del pin de salida X3 (GPIO7)

#define E0 15   //Asignacino del pin de Entrada E0 
#define E1 16   //Asignacino del pin de Entrada E1 
#define E2 17   //Asignacino del pin de Entrada E2 
#define E3 18   //Asignacino del pin de Entrada E3
#define E4 8    //Asignacino del pin de Entrada E4
#define E5 9    //Asignacino del pin de Entrada E5

// Array de lectura de Fila
const unsigned int ESwitch[ALTURATECLADO] = {E0, E1, E2, E3, E4, E5}; 

const long TiempoDebounce = 5;
extern bool SwitchEstado[ALTURATECLADO][ANCHURATECLADO];
extern bool SwitchEstadoAntiguo[ALTURATECLADO][ANCHURATECLADO];
extern unsigned long Debounce[ALTURATECLADO][ANCHURATECLADO];
extern int option_choose;
extern int option_selected;

const bool nums0_15[NUMSCANTIDAD][NUMSANCHO] = {{0, 0, 0, 0},
                                                {0, 0, 0, 1},
                                                {0, 0, 1, 0},
                                                {0, 0, 1, 1},
                                                {0, 1, 0, 0},
                                                {0, 1, 0, 1},
                                                {0, 1, 1, 0},
                                                {0, 1, 1, 1},
                                                {1, 0, 0, 0},
                                                {1, 0, 0, 1},
                                                {1, 0, 1, 0},
                                                {1, 0, 1, 1},
                                                {1, 1, 0, 0},
                                                {1, 1, 0, 1},
                                                {1, 1, 1, 0},
                                                {1, 1, 1, 1}};

void WorkingModeKeyboard(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected);

void WorkingModeDisplay(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected);

//Poisition of the configuration icon
void printMenuConfigDisplay(TFT_eSPI &tft);

	//Poisition of the Brightness icon
void printMenuBrightnessDisplay(TFT_eSPI &tfttft);

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
void printMenuBrightnessDisplayInverted(TFT_eSPI &tfttft);

	//Poisition of the Leds icon
void printMenuLedsDisplayInverted(TFT_eSPI &tft);
				
	//Poisition of the Energy icon
void printMenuEnergyDisplayInverted(TFT_eSPI &tft);

	//Poisition of the Connection icon
void printMenuConnectionDisplayInverted(TFT_eSPI &tft);

	//Poisition of the InfoHelp icon
void printMenuInfoHelpDisplayInverted(TFT_eSPI &tft);
