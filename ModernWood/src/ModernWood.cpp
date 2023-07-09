#include <ModernWood.h>

bool SwitchEstado[ALTURATECLADO][ANCHURATECLADO] = {false};
bool SwitchEstadoAntiguo[ALTURATECLADO][ANCHURATECLADO] = {false};
unsigned long Debounce[ALTURATECLADO][ANCHURATECLADO] = {0};

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

    //Check if E4 is pressed
    if(!digitalRead(E4)){
        Serial.println("E4 pressed");
    }

}