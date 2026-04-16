#include "common/mathTypes.h"
#include "common/mathTools.h"
#ifndef GAITCMD_H
#define GAITCMD_H
struct GaitCmd{
    Vec4 bias;
    double beta;
    double period;
};

#endif