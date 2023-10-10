#include "ModulesMap.h"

//BASE MODULE
#define FN_NONE noneFN
#define FN_BASE ModuleGetVersion

//MODULES
#define FN_KEY_BACKSPACE noneFN
#define FN_KEY_TAB noneFN
#define FN_KEY_ENTER noneFN
#define FN_KEY_ESPACIO noneFN
#define FN_KEY_ACENTO noneFN 
#define FN_KEY_COMA noneFN
#define FN_KEY_APOSTROFE noneFN
#define FN_KEY_PUNTO noneFN
#define FN_KEY_MENOS noneFN
#define FN_KEY_CERO noneFN
#define FN_KEY_UNO noneFN
#define FN_KEY_DOS noneFN
#define FN_KEY_TRES noneFN
#define FN_KEY_CUATRO noneFN
#define FN_KEY_CINCO noneFN
#define FN_KEY_SEIS noneFN
#define FN_KEY_SIETE noneFN
#define FN_KEY_OCHO noneFN
#define FN_KEY_NUEVE noneFN
#define FN_KEY_ENIE noneFN
#define FN_KEY_EXCLAMACION noneFN
#define FN_KEY_ACENTOAGUDO noneFN
#define FN_KEY_CEDILLA noneFN
#define FN_KEY_SIGNOMAS noneFN
#define FN_KEY_ANGULO noneFN
#define FN_KEY_A noneFN
#define FN_KEY_B noneFN
#define FN_KEY_C noneFN
#define FN_KEY_D noneFN
#define FN_KEY_E noneFN
#define FN_KEY_F noneFN
#define FN_KEY_G noneFN
#define FN_KEY_H noneFN
#define FN_KEY_I noneFN
#define FN_KEY_J noneFN
#define FN_KEY_K noneFN
#define FN_KEY_L noneFN
#define FN_KEY_M noneFN
#define FN_KEY_N noneFN
#define FN_KEY_O noneFN
#define FN_KEY_P noneFN
#define FN_KEY_Q noneFN
#define FN_KEY_R noneFN
#define FN_KEY_S noneFN
#define FN_KEY_T noneFN
#define FN_KEY_U noneFN
#define FN_KEY_V noneFN
#define FN_KEY_W noneFN
#define FN_KEY_X noneFN
#define FN_KEY_Y noneFN
#define FN_KEY_Z noneFN
#define FN_KEY_LSHIFT noneFN
#define FN_KEY_LCONTROL noneFN
#define FN_KEY_LALT noneFN
#define FN_KEY_WINDOWS noneFN
#define FN_KEY_RCONTROL noneFN
#define FN_KEY_RSHIFT noneFN
#define FN_KEY_ALTGR noneFN
#define FN_KEY_ESC noneFN
#define FN_KEY_CAPSLOCK noneFN
#define FN_KEY_F1 noneFN
#define FN_KEY_F2 noneFN
#define FN_KEY_F3 noneFN
#define FN_KEY_F4 noneFN
#define FN_KEY_F5 noneFN
#define FN_KEY_F6 noneFN
#define FN_KEY_F7 noneFN
#define FN_KEY_F8 noneFN
#define FN_KEY_F9 noneFN
#define FN_KEY_F10 noneFN
#define FN_KEY_F11 noneFN
#define FN_KEY_F12 noneFN
#define FN_KEY_PRINTSCREEN noneFN
#define FN_KEY_BLOQ noneFN
#define FN_KEY_PAUSE noneFN
#define FN_KEY_INS noneFN
#define FN_KEY_HOME noneFN
#define FN_KEY_PAGEUP noneFN
#define FN_KEY_DEL noneFN
#define FN_KEY_END noneFN
#define FN_KEY_PAGEDOWN noneFN
#define FN_KEY_FLECHADERECHA noneFN
#define FN_KEY_FLECHAIZQUIERDA noneFN
#define FN_KEY_FLECHAABAJO noneFN
#define FN_KEY_FLECHAARRIBA noneFN
#define FN_KEY_NUMLOCK noneFN
#define FN_KEY_DIVISIONPAD noneFN
#define FN_KEY_PORPAD noneFN
#define FN_KEY_MENOSPAD noneFN
#define FN_KEY_SUMAPAD noneFN
#define FN_KEY_ENTERPAD noneFN
#define FN_KEY_UNOPAD noneFN
#define FN_KEY_DOSPAD noneFN
#define FN_KEY_TRESPAD noneFN
#define FN_KEY_CUATROPAD noneFN
#define FN_KEY_CINCOPAD noneFN
#define FN_KEY_SEISPAD noneFN
#define FN_KEY_SIETEPAD noneFN
#define FN_KEY_OCHOPAD noneFN
#define FN_KEY_NUEVEPAD noneFN
#define FN_KEY_CEROPAD noneFN
#define FN_KEY_PUNTOPAD noneFN
#define FN_KEY_MENOSMAYOR noneFN
#define FN_KEY_CONTEXTMENU noneFN

//Array of function pointers to the functions that will be called when the key is pressed (In which module should be activated
//Note that the function will be executed in a loop once the key is pressed. So, the exit condition must be implemented in the function itself (if needed)
//Fn Key will interrupt the loop and stop the function so this key MUST NOT be used.
//All the funcctions has to be declared as "void foo(void){}"
void (*MODULESFUNCARRAY[KEYBOARDHEIGHT][KEYBOARDWIDTH])(void) = {{FN_KEY_ESC,&FN_KEY_F1,&FN_KEY_F2,&FN_KEY_F3,&FN_KEY_F4,&FN_KEY_F5,&FN_KEY_F6,&FN_KEY_F7,&FN_KEY_F8,&FN_KEY_F9,&FN_KEY_F10,&FN_KEY_F11,&FN_KEY_F12,&FN_KEY_PRINTSCREEN,&FN_KEY_BLOQ,&FN_KEY_PAUSE},
                                                                 {FN_KEY_ANGULO,&FN_KEY_UNO,&FN_KEY_DOS,&FN_KEY_TRES,&FN_KEY_CUATRO,&FN_KEY_CINCO,&FN_KEY_SEIS,&FN_KEY_SIETE,&FN_KEY_OCHO,&FN_KEY_NUEVE,&FN_KEY_CERO,&FN_KEY_APOSTROFE,&FN_KEY_EXCLAMACION,&FN_KEY_BACKSPACE,&FN_KEY_HOME,&FN_KEY_PAGEUP},
                                                                 {FN_KEY_TAB,&FN_KEY_Q,&FN_KEY_W,&FN_KEY_E,&FN_KEY_R,&FN_KEY_T,&FN_KEY_Y,&FN_KEY_U,&FN_KEY_I,&FN_KEY_O,&FN_KEY_P,&FN_KEY_ACENTOAGUDO,&FN_KEY_SIGNOMAS,&FN_KEY_ENTER,&FN_KEY_END,&FN_KEY_PAGEDOWN},
                                                                 {FN_KEY_CAPSLOCK,&FN_KEY_A,&FN_KEY_S,&FN_KEY_D,&FN_KEY_F,&FN_KEY_G,&FN_KEY_H,&FN_KEY_J,&FN_KEY_K,&FN_KEY_L,&FN_KEY_ENIE,&FN_KEY_ACENTO,&FN_KEY_CEDILLA,&FN_KEY_INS,&FN_NONE,&FN_NONE},
                                                                 {FN_KEY_LSHIFT,&FN_KEY_MENOSMAYOR,&FN_KEY_Z,&FN_KEY_X,&FN_KEY_C,&FN_KEY_V,&FN_KEY_B,&FN_KEY_N,&FN_KEY_M,&FN_KEY_COMA,&FN_KEY_PUNTO,&FN_KEY_MENOS,&FN_KEY_RSHIFT,&FN_KEY_DEL,&FN_KEY_FLECHAARRIBA,&FN_NONE},
                                                                 {FN_KEY_LCONTROL,&FN_KEY_WINDOWS,&FN_KEY_LALT,&FN_KEY_DIVISIONPAD,&FN_KEY_PORPAD,&FN_KEY_MENOSPAD,&FN_KEY_ESPACIO,&FN_KEY_SUMAPAD,&FN_KEY_RCONTROL,&FN_KEY_NUMLOCK,&FN_KEY_ALTGR,&FN_NONE,&FN_KEY_CONTEXTMENU,&FN_KEY_FLECHAIZQUIERDA,&FN_KEY_FLECHAABAJO,&FN_KEY_FLECHADERECHA}};