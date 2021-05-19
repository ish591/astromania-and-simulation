#include "PRG.h"

PRG::PRG(int s)
{
    seed = s;
    x = s;
    a = 48271;
    c = 0;
    m = 2147483647;
}

int PRG::generate()
{
    x = (a * x + c) % m;
    return x;
}