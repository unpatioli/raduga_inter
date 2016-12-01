#ifndef INTERPOLATION_LINEARINTERPOLATOR_H
#define INTERPOLATION_LINEARINTERPOLATOR_H

#include "Interpolator.h"

class LinearInterpolator : public Interpolator
{
public:
    LinearInterpolator(const std::shared_ptr<ValCollectionT> &funcTable, double step);

    double operator()(double x);
};


#endif //INTERPOLATION_LINEARINTERPOLATOR_H
