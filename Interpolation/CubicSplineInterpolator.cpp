#include "CubicSplineInterpolator.h"

CubicSplineInterpolator::CubicSplineInterpolator(std::shared_ptr<ValCollectionT> funcTable, double step) :
        Interpolator(funcTable, step)
{
    size_t n = _funcTable->size();
    _splines.resize(n);

    for (size_t i = 0; i < n; ++i) {
        _splines[i].x = i * step;
        _splines[i].a = _funcTable->at(i);
    }
    _splines[0].c = 0.;

    std::vector<double> alpha(n - 1);
    std::vector<double> beta(n - 1);
    double A, B, C, F, h_i, h_i1, z;
    alpha[0] = beta[0] = 0.;
    for (size_t i = 1; i < n - 1; ++i) {
        h_i = i * step - (i - 1) * step;
        h_i1 = (i + 1) * step - i * step;
        A = h_i;
        C = 2. * (h_i + h_i1);
        B = h_i1;
        F = 6. *
            ((_funcTable->at(i + 1) - _funcTable->at(i)) / h_i1 - (_funcTable->at(i) - _funcTable->at(i - 1)) / h_i);
        z = (A * alpha[i - 1] + C);
        alpha[i] = -B / z;
        beta[i] = (F - A * beta[i - 1]) / z;
    }
    _splines[n - 1].c = (F - A * beta[n - 2]) / (C + A * alpha[n - 2]);

    for (size_t i = n - 2; i > 0; --i) {
        _splines[i].c = alpha[i] * _splines[i + 1].c + beta[i];
    }

    for (size_t i = n - 1; i > 0; --i) {
        h_i = i * step - (i - 1) * step;
        _splines[i].d = (_splines[i].c - _splines[i - 1].c) / h_i;
        _splines[i].b =
                h_i * (2. * _splines[i].c + _splines[i - 1].c) / 6. + (_funcTable->at(i) - _funcTable->at(i - 1)) / h_i;
    }
}

double CubicSplineInterpolator::operator()(double x)
{
    size_t n = _splines.size();
    Spline s;
    if (x <= _splines[0].x) {
        s = _splines[1];
    } else if (x >= _splines[n - 1].x) {
        s = _splines[n - 1];
    } else {
        size_t i = 0, j = n - 1;
        while (i + 1 < j) {
            size_t k = i + (j - i) / 2;
            if (x <= _splines[k].x) {
                j = k;
            } else {
                i = k;
            }
        }
        s = _splines[j];
    }

    double dx = (x - s.x);
    return s.a + (s.b + (s.c / 2. + s.d * dx / 6.) * dx) * dx;
}
