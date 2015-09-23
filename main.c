#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include "WService.c"
#else
    //#include "LDaemon.c"
#endif


int main(int argc, const char * argv[]) {
   #ifdef _WIN32
        WStart();
    #else
        LStart();
    #endif
    return 0;
}
