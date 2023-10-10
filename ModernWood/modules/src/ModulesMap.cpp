#include "ModulesMap.h"

String (*fun_ptr)(void) = ModuleGetVersion; //Function pointer to the function that will be called when the key is pressed (In which module should be activated)