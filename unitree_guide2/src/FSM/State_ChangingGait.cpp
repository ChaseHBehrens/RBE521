#include <cassert>


/**
 * @param initVal - the value at the start of the transition (0 to 1)
 * @param targVal - the value at the end to the transition (0 to 1)
 * @param currStep - the current step of the transition
 * @param totalSteps - the number of steps in the transition
 * @return nextVal
 */
double updateVal(double initVal, double targVal, int currStep, int totalSteps) {
    assert(initVal >= 0.0 && initVal <= 1.0);
    assert(currVal >= 0.0 && currVal <= 1.0);
    assert(targVal >= 0.0 && targVal <= 1.0);
    double totalDiff = targVal - initVal;
    double stepSize = totalDiff / totalSteps;
    return initVal + ((currStep + 1) * stepSize);
}


