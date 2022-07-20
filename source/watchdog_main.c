/*============================LIBRARIES && MACROS =================================*/
#include "wd.h"

#define WD 1
/*=========================== FUNCTION DEFINITION =================================*/
int main(int argc, char *argv[])
{
    is_wd = WD;
    WDStart(argv);
    (void)argc;
    return 0;
}
