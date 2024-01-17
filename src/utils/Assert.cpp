#include "Assert.h"

// TODO: Add signal trapping/breakpoint trigger


static bool err = false;
void createBreak()
{
    err = true;
}