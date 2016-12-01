#ifndef INTERPOLATION_INTERPOLATOR_H
#define INTERPOLATION_INTERPOLATOR_H

#include <vector>

typedef std::vector<double> ValCollectionT;

class Interpolator
{
public:
    Interpolator(std::shared_ptr<ValCollectionT> funcTable, double step);
    virtual ~Interpolator();

    virtual double operator()(double x) = 0;

    static std::shared_ptr<ValCollectionT> generate(size_t count);

protected:
    std::shared_ptr<ValCollectionT> _funcTable;
    double _step;
};


#endif //INTERPOLATION_INTERPOLATOR_H
