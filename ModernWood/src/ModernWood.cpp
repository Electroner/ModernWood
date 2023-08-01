#include <ModernWood.h>

bool SwitchEstado[ALTURATECLADO][ANCHURATECLADO] = {false};
bool SwitchEstadoAntiguo[ALTURATECLADO][ANCHURATECLADO] = {false};
unsigned long Debounce[ALTURATECLADO][ANCHURATECLADO] = {0};
int option_choose = 0;
int option_selected = 1;

void WorkingModeKeyboard(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected){
    
    bool SwicthEstadoRapido;
    if(isUSBConnected)
    {
        for (int i = 0; i < ANCHURATECLADO; i++)
        {
            digitalWrite(COD0, nums0_15[i][0]);
            digitalWrite(COD1, nums0_15[i][1]);
            digitalWrite(COD2, nums0_15[i][2]);
            digitalWrite(COD3, nums0_15[i][3]);
            for (int k = 0; k < ALTURATECLADO; k++)
            {
                SwicthEstadoRapido = !(digitalRead(ESwitch[k]));
                if (SwicthEstadoRapido && !SwitchEstadoAntiguo[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
                {
                    Debounce[k][i] = millis();
                    SwitchEstadoAntiguo[k][i] = SwicthEstadoRapido;
                    Keyboard.press(TECLASTECLADO[k][i]);
                }
                else if (!SwicthEstadoRapido && SwitchEstadoAntiguo[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
                {
                    Debounce[k][i] = millis();
                    SwitchEstadoAntiguo[k][i] = SwicthEstadoRapido;
                    Keyboard.release(TECLASTECLADO[k][i]);
                }
                SwitchEstado[k][i] = SwicthEstadoRapido;
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
                SwicthEstadoRapido = !(digitalRead(ESwitch[k]));
                if (SwicthEstadoRapido && !SwitchEstadoAntiguo[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
                {
                    Debounce[k][i] = millis();
                    SwitchEstadoAntiguo[k][i] = SwicthEstadoRapido;
                    bleKeyboard.press(TECLASTECLADO[k][i]);
                }
                else if (!SwicthEstadoRapido && SwitchEstadoAntiguo[k][i] && (millis() - Debounce[k][i]) > TiempoDebounce)
                {
                    Debounce[k][i] = millis();
                    SwitchEstadoAntiguo[k][i] = SwicthEstadoRapido;
                    bleKeyboard.release(TECLASTECLADO[k][i]);
                }
                SwitchEstado[k][i] = SwicthEstadoRapido;
            }
        }
    }
}

void WorkingModeDisplay(TFT_eSPI &tft, BleKeyboard &bleKeyboard, USBHIDKeyboard &Keyboard, bool volatile &isBLEConnected, bool volatile &isUSBConnected){

    switch (option_selected)
    {
    case 1:
        //Push the inverted image of the option 1
        printMenuConfigDisplayInverted(tft);
        option_choose = 1;
        option_selected = 0;
        break;
    
    case 2:
        printMenuBrightnessDisplayInverted(tft);
        option_choose = 2;
        option_selected = 0;
        break;
   
    case 3:
        //Print a rectangle gray deep black in the square of the option 3
        printMenuLedsDisplayInverted(tft);
        option_choose = 3;
        option_selected = 0;
        break;
    
    case 4:
        //Print a rectangle gray deep black in the square of the option 4
        printMenuEnergyDisplayInverted(tft);
        option_choose = 4;
        option_selected = 0;
        break;
    
    case 5:
        //Print a rectangle gray deep black in the square of the option 5
        printMenuConnectionDisplayInverted(tft);
        option_choose = 5;
        option_selected = 0;
        break;
    
    case 6:
        //Print a rectangle gray deep black in the square of the option 6
        printMenuInfoHelpDisplayInverted(tft);
        option_choose = 6;
        option_selected = 0;
        break;
    
    default:
        break;
    }

}

//Option 1
void printMenuConfigDisplay(TFT_eSPI &tft){
    tft.setCursor(Menu_Config_display_image.x, Menu_Config_display_image.y);
	tft.pushImage(Menu_Config_display_image.x,
				  Menu_Config_display_image.y, 
				  Menu_Config_display_image.width, 
				  Menu_Config_display_image.height, 
				  image_Menu_Config, 0);
}

//Inverted image of the option 1
void printMenuConfigDisplayInverted(TFT_eSPI &tft){
    tft.setCursor(Menu_Config_display_image.x, Menu_Config_display_image.y);
    tft.pushImage(Menu_Config_display_image.x,
                  Menu_Config_display_image.y, 
                  Menu_Config_display_image.width, 
                  Menu_Config_display_image.height, 
                  image_Menu_Config_Inverted, 0);
}

//Option 2
void printMenuBrightnessDisplay(TFT_eSPI &tft){
    tft.setCursor(Menu_Brightness_display_image.x, Menu_Brightness_display_image.y);
	tft.pushImage(Menu_Brightness_display_image.x,
				  Menu_Brightness_display_image.y, 
				  Menu_Brightness_display_image.width, 
				  Menu_Brightness_display_image.height, 
				  image_Menu_Brightness, 0);
}

//Inverted image of the option 2
void printMenuBrightnessDisplayInverted(TFT_eSPI &tft){
    tft.setCursor(Menu_Brightness_display_image.x, Menu_Brightness_display_image.y);
    tft.pushImage(Menu_Brightness_display_image.x,
                  Menu_Brightness_display_image.y, 
                  Menu_Brightness_display_image.width, 
                  Menu_Brightness_display_image.height, 
                  image_Menu_Brightness_Inverted, 0);
}

//Option 3
void printMenuLedsDisplay(TFT_eSPI &tft){
    tft.setCursor(Menu_Leds_display_image.x, Menu_Leds_display_image.y);
	tft.pushImage(Menu_Leds_display_image.x,
				  Menu_Leds_display_image.y, 
				  Menu_Leds_display_image.width, 
				  Menu_Leds_display_image.height, 
				  image_Menu_Leds, 0);
}

//Inverted image of the option 3
void printMenuLedsDisplayInverted(TFT_eSPI &tft){
    tft.setCursor(Menu_Leds_display_image.x, Menu_Leds_display_image.y);
    tft.pushImage(Menu_Leds_display_image.x,
                  Menu_Leds_display_image.y, 
                  Menu_Leds_display_image.width, 
                  Menu_Leds_display_image.height, 
                  image_Menu_Leds_Inverted, 0);
}

//Option 4
void printMenuEnergyDisplay(TFT_eSPI &tft){
    tft.setCursor(Menu_Energy_display_image.x, Menu_Energy_display_image.y);
	tft.pushImage(Menu_Energy_display_image.x,
				  Menu_Energy_display_image.y, 
				  Menu_Energy_display_image.width, 
				  Menu_Energy_display_image.height, 
				  image_Menu_Energy, 0);
}

//Inverted image of the option 4
void printMenuEnergyDisplayInverted(TFT_eSPI &tft){
    tft.setCursor(Menu_Energy_display_image.x, Menu_Energy_display_image.y);
    tft.pushImage(Menu_Energy_display_image.x,
                  Menu_Energy_display_image.y, 
                  Menu_Energy_display_image.width, 
                  Menu_Energy_display_image.height, 
                  image_Menu_Energy_Inverted, 0);
}

//Option 5
void printMenuConnectionDisplay(TFT_eSPI &tft){
    tft.setCursor(Menu_Connection_display_image.x, Menu_Connection_display_image.y);
	tft.pushImage(Menu_Connection_display_image.x,
				  Menu_Connection_display_image.y, 
				  Menu_Connection_display_image.width, 
				  Menu_Connection_display_image.height, 
				  image_Menu_Connectivity, 0);
}

//Inverted image of the option 5
void printMenuConnectionDisplayInverted(TFT_eSPI &tft){
    tft.setCursor(Menu_Connection_display_image.x, Menu_Connection_display_image.y);
    tft.pushImage(Menu_Connection_display_image.x,
                  Menu_Connection_display_image.y, 
                  Menu_Connection_display_image.width, 
                  Menu_Connection_display_image.height, 
                  image_Menu_Connectivity_Inverted, 0);
}

//Option 6
void printMenuInfoHelpDisplay(TFT_eSPI &tft){
    tft.setCursor(Menu_InfoHelp_display_image.x, Menu_InfoHelp_display_image.y);
	tft.pushImage(Menu_InfoHelp_display_image.x,
				  Menu_InfoHelp_display_image.y, 
				  Menu_InfoHelp_display_image.width, 
				  Menu_InfoHelp_display_image.height, 
				  image_Menu_InfoHelp, 0);
}

//Inverted image of the option 6
void printMenuInfoHelpDisplayInverted(TFT_eSPI &tft){
    tft.setCursor(Menu_InfoHelp_display_image.x, Menu_InfoHelp_display_image.y);
    tft.pushImage(Menu_InfoHelp_display_image.x,
                  Menu_InfoHelp_display_image.y, 
                  Menu_InfoHelp_display_image.width, 
                  Menu_InfoHelp_display_image.height, 
                  image_Menu_InfoHelp_Inverted, 0);
}