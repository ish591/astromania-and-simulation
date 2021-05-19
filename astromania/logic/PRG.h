#pragma once
#include <iostream>

using namespace std;

class PRG
{
public:
    PRG(int);
    int generate();

private:
    long long int seed;
    long long int a, c, x, m;
};