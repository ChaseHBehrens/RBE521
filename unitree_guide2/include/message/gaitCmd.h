#include "common/mathTypes.h"
#include "common/mathTools.h"
#include <memory>
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
    using SharedPtr = std::shared_ptr<GaitCmd>;
};

#endif