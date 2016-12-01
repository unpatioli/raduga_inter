#ifndef INTERPOLATION_SQUAREINTERPOLATOR_H
#define INTERPOLATION_SQUAREINTERPOLATOR_H

#include "Interpolator.h"

class SquareInterpolator : public Interpolator
{
public:
    SquareInterpolator(std::shared_ptr<ValCollectionT> funcTable, double step);

    double operator()(double x);
};


#endif //INTERPOLATION_SQUAREINTERPOLATOR_H
