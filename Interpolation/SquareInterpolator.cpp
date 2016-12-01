#include <cmath>
#include "SquareInterpolator.h"

SquareInterpolator::SquareInterpolator(std::shared_ptr<ValCollectionT> funcTable, double step) :
        Interpolator(funcTable, step)
{}

double SquareInterpolator::operator()(double x)
{
    int x_curr_id = static_cast<int>(std::floor(x / _step));
    if (x_curr_id >= _funcTable->size() - 1 || x_curr_id < 1) {
        throw std::invalid_argument("x is outside of function domain");
    }
    int x_prev_id = x_curr_id - 1;
    int x_next_id = x_curr_id + 1;

    double x_curr = x_curr_id * _step;
    double x_prev = x_prev_id * _step;
    double x_next = x_next_id * _step;

    double a2 = ((_funcTable->at(x_next_id) - _funcTable->at(x_prev_id)) / ((x_next - x_prev) * (x_next - x_curr))) -
                ((_funcTable->at(x_curr_id) - _funcTable->at(x_prev_id)) / ((x_curr - x_prev) * (x_next - x_curr)));
    double a1 = (_funcTable->at(x_curr_id) - _funcTable->at(x_prev_id)) / (x_curr - x_prev) - a2 * (x_curr + x_prev);
    double a0 = _funcTable->at(x_prev_id) - a1 * x_prev - a2 * x_prev * x_prev;

    return a0 + a1 * x + a2 * x * x;
}
