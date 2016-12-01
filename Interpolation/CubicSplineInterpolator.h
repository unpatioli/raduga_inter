#ifndef INTERPOLATION_CUBICSPLINEINTERPOLATOR_H
#define INTERPOLATION_CUBICSPLINEINTERPOLATOR_H

#include "Interpolator.h"

struct Spline
{
    double a, b, c, d, x;
};

typedef std::vector<Spline> SplineCollectionT;

class CubicSplineInterpolator : public Interpolator
{
public:
    CubicSplineInterpolator(std::shared_ptr<ValCollectionT> funcTable, double step);

    double operator()(double x);

private:
    SplineCollectionT _splines;
};


#endif //INTERPOLATION_CUBICSPLINEINTERPOLATOR_H
