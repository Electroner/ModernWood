#include "Base.h"

void ModuleGetVersion(){
    Serial.println("1.0.0b");
}

void __attribute__((optimize("O0"))) noneFN(void){
    //Reset the function mode and the function row and col to do nothing
    inExternalFunctionMode = false;
    executingCustomFunction = false;
    actualFunctionRow = 0; 
    actualFunctionCol = 0;
    return;
}