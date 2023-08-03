#include <Extra.h>

unsigned modulo_p(int value, unsigned m){
    int mod = value % (int)m;
    if (mod < 0) {
        mod += m;
    }
    return mod;
}