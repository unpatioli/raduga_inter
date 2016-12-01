#include <QApplication>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QMainWindow>
#include <QScatterSeries>
#include <random>
#include <iostream>
#include "Interpolation/Interpolator.h"
#include "Interpolation/LinearInterpolator.h"
#include "Interpolation/SquareInterpolator.h"
#include "Interpolation/CubicSplineInterpolator.h"

QT_CHARTS_USE_NAMESPACE

typedef std::vector<double> ValCollectionT;

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // Generate points
    const int VAL_COUNT = 41;
    const float STEP = 5;
    const float PLOT_STEP = 0.1;
    auto funcTable = Interpolator::generate(VAL_COUNT);

    // Prepare chart view
    auto chart = new QChart();
    chart->legend()->hide();

    // Show generated points
    auto scatterSeries = new QScatterSeries();
    scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    scatterSeries->setMarkerSize(10.0);
    for (size_t i = 0; i < funcTable->size(); ++i) {
        scatterSeries->append(STEP * i, funcTable->at(i));
    }
    chart->addSeries(scatterSeries);

    // Generate linear interpolation
    LinearInterpolator li(funcTable, STEP);
    auto linearSeries = new QLineSeries();
    for (double x=0; x < VAL_COUNT * STEP; x += PLOT_STEP) {
        linearSeries->append(x, li(x));
    }
    chart->addSeries(linearSeries);

    // Generate square interpolation
    SquareInterpolator si(funcTable, STEP);
    auto squareSeries = new QLineSeries();
    for (double x = STEP; x < (VAL_COUNT - 1) * STEP; x += PLOT_STEP) {
        try {
            squareSeries->append(x, si(x));
        } catch (std::invalid_argument e) {
            std::cout << "x = " << x << " (" << e.what() << ")" << std::endl;
        }
    }
    chart->addSeries(squareSeries);

    // Generate cubic spline interpolation
    CubicSplineInterpolator csi(funcTable, STEP);
    auto cubicSeries = new QLineSeries();
    for (double x = STEP; x < (VAL_COUNT - 1) * STEP; x += PLOT_STEP) {
        cubicSeries->append(x, csi(x));
    }
    chart->addSeries(cubicSeries);

    // Prepare chart
    chart->createDefaultAxes();
    chart->setDropShadowEnabled(false);
    chart->setTitle("Random function interpolation");

    // Draw chart
    auto chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Draw window
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(800, 600);
    window.show();

    // Exec application
    return application.exec();
}
