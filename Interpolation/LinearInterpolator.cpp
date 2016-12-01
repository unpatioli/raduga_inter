#include <cmath>
#include <stdexcept>
#include "LinearInterpolator.h"

LinearInterpolator::LinearInterpolator(const std::shared_ptr<ValCollectionT> &funcTable, double step) :
        Interpolator(funcTable, step)
{}

double LinearInterpolator::operator()(double x)
{
    int x1_id = static_cast<int>(std::floor(x / _step));
    if (x1_id >= _funcTable->size()) {
        throw std::invalid_argument("x is outside of function domain");
    }
    int x2_id = x1_id + 1;

    double x1 = x1_id * _step;
    double y1 = _funcTable->at(x1_id);
    double x2 = x2_id * _step;
    double y2 = _funcTable->at(x2_id);

    double a = (y2 - y1) / (x2 - x1);
    double b = y1 - a * x1;

    return a * x + b;
}
