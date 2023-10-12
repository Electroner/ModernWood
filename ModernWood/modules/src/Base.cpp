#include "Base.h"

void __attribute__((optimize("O0"))) noneFN(void){
    //Just Exit Module
    //All modules must have this function to end the module and return to the main menu (executed one time)
    //If you don't have this function, the module will be stuck in a loop until FN KEY is pressed or the board is reset
    exitModule();
}