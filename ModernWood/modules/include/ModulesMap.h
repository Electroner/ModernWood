#pragma once

#include <ModernWood.h>

//All modules headers must be included here
#include "Base.h"

//Array of function pointers to the functions that will be called when the key is pressed (In which module should be activated
//Note that the function will be executed in a loop once the key is pressed. So, the exit condition must be implemented in the function itself (if needed)
//Fn Key will interrupt the loop and stop the function so this key MUST NOT be used.
//All the funcctions has to be declared as "void foo(void){}"
extern void (*MODULESFUNCARRAY[KEYBOARDHEIGHT][KEYBOARDWIDTH])(void);