#include "common/mathTypes.h"

/**
 * @param initVal - the value at the start of the transition (0 to 1)
 * @param targVal - the value at the end to the transition (0 to 1)
 * @param currStep - the current step of the transition
 * @param totalSteps - the number of steps in the transition
 * @return nextVal
 */
double updateVal(double initVal, double targVal, int currStep, int totalSteps) {
    assert(initVal >= 0.0 && initVal <= 1.0);
    assert(targVal >= 0.0 && targVal <= 1.0);
    double totalDiff = targVal - initVal;
    double stepSize = totalDiff / totalSteps;
    return initVal + ((currStep + 1) * stepSize);
}

// I think this would be computationally easier
double stepSize(int n, double initVal, double targVal) {
    assert(initVal >= 0.0 && initVal <= 1.0);
    assert(targVal >= 0.0 && targVal <= 1.0);
    double totalDiff = targVal - initVal;
    return totalDiff / n;
}

double updateVal(double currVal, double stepSize) {
    return currVal + stepSize;
}

// here with vecs
Vec4 stepSizes(int n, Vec4 initVals, Vec4 targVals) {
    Vec4 totalDiff = targVals - initVals;
    return totalDiff / n;
}

Vec4 updateVals(Vec4 currVals, Vec4 stepSize) {
    return currVals + stepSize;
}
