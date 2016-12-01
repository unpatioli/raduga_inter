#include <random>
#include "Interpolator.h"

Interpolator::Interpolator(std::shared_ptr<ValCollectionT> funcTable, double step) :
        _funcTable(funcTable), _step(step)
{}

Interpolator::~Interpolator()
{}

std::shared_ptr<ValCollectionT> Interpolator::generate(size_t count)
{
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-10, 10);
    auto funcTable = std::make_shared<ValCollectionT>(count + 1);
    for (size_t i = 0; i < count; ++i) {
        funcTable->at(i) = distribution(generator);
    }
    return funcTable;
}
