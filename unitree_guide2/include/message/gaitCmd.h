#include "common/mathTypes.h"
#include "common/mathTools.h"
#ifndef GAITCMD_H
#define GAITCMD_H
struct bias{
    double l1;
    double l2;
    double l3;
    double l4;
};
struct GaitCmd{
    bias b;
    double beta;
    double period;
};

#endif