#include <QApplication>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QMainWindow>
#include <QScatterSeries>
#include <random>
#include <iostream>

QT_CHARTS_USE_NAMESPACE

typedef std::vector<double> ValCollectionT;

void generate(ValCollectionT&, int);
double linear(const ValCollectionT&, double, double);
double square(const ValCollectionT&, double, double);

struct Spline
{
    double a, b, c, d, x;
};

typedef std::vector<Spline> SplineCollectionT;
void buildSplines(SplineCollectionT&, const ValCollectionT&, double);
double cubic(const SplineCollectionT&, double);

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // Generate points
    ValCollectionT F;
    const int VAL_COUNT = 41;
    const float STEP = 5;
    const float PLOT_STEP = 0.1;
    generate(F, VAL_COUNT);

    // Prepare chart view
    auto chart = new QChart();
    chart->legend()->hide();

    // Show generated points
    auto scatterSeries = new QScatterSeries();
    scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    scatterSeries->setMarkerSize(10.0);
    for (int i = 0; i < F.size(); ++i) {
        scatterSeries->append(STEP * i, F[i]);
    }
    chart->addSeries(scatterSeries);

    // Generate linear interpolation
    auto linearSeries = new QLineSeries();
    for (double x=0; x < VAL_COUNT * STEP; x += PLOT_STEP) {
        linearSeries->append(x, linear(F, x, STEP));
    }
    chart->addSeries(linearSeries);

    // Generate square interpolation
    auto squareSeries = new QLineSeries();
    for (double x = STEP; x < (VAL_COUNT - 1) * STEP; x += PLOT_STEP) {
        try {
            squareSeries->append(x, square(F, x, STEP));
        } catch (std::invalid_argument e) {
            std::cout << "x = " << x << " (" << e.what() << ")" << std::endl;
        }
    }
    chart->addSeries(squareSeries);

    // Generate cubic spline interpolation
    auto cubicSeries = new QLineSeries();
    SplineCollectionT splines(VAL_COUNT);
    buildSplines(splines, F, STEP);
    for (double x = STEP; x < (VAL_COUNT - 1) * STEP; x += PLOT_STEP) {
        cubicSeries->append(x, cubic(splines, x));
    }
    chart->addSeries(cubicSeries);

    chart->createDefaultAxes();
    chart->setDropShadowEnabled(false);
    chart->setTitle("Simple chart");

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

void generate(ValCollectionT& F, int count)
{
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-10, 10);
    for (int i = 0; i < count; ++i) {
        F.push_back(distribution(generator));
    }
}

double linear(const ValCollectionT& F, double x, double step)
{
    int x1_id = static_cast<int>(std::floor(x / step));
    if (x1_id >= F.size()) {
        throw std::invalid_argument("x is outside of function domain");
    }
    int x2_id = x1_id + 1;

    double x1 = x1_id * step;
    double y1 = F[x1_id];
    double x2 = x2_id * step;
    double y2 = F[x2_id];

    double a = (y2 - y1) / (x2 - x1);
    double b = y1 - a * x1;

    return a * x + b;
}

double square(const ValCollectionT& F, double x, double step)
{
    int x_curr_id = static_cast<int>(std::floor(x / step));
    if (x_curr_id >= F.size() - 1 || x_curr_id < 1) {
        throw std::invalid_argument("x is outside of function domain");
    }
    int x_prev_id = x_curr_id - 1;
    int x_next_id = x_curr_id + 1;

    double x_curr = x_curr_id * step;
    double x_prev = x_prev_id * step;
    double x_next = x_next_id * step;

    double a2 = ((F[x_next_id] - F[x_prev_id]) / ((x_next - x_prev) * (x_next - x_curr))) -
                ((F[x_curr_id] - F[x_prev_id]) / ((x_curr - x_prev) * (x_next - x_curr)));
    double a1 = (F[x_curr_id] - F[x_prev_id]) / (x_curr - x_prev) - a2 * (x_curr + x_prev);
    double a0 = F[x_prev_id] - a1 * x_prev - a2 * x_prev * x_prev;

    return a0 + a1 * x + a2 * x * x;
}

void buildSplines(SplineCollectionT& splines, const ValCollectionT& Func, double step)
{
    size_t n = Func.size();
    for (size_t i = 0; i < n; ++i) {
        splines[i].x = i * step;
        splines[i].a = Func[i];
    }
    splines[0].c = 0.;

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
        F = 6. * ((Func[i + 1] - Func[i]) / h_i1 - (Func[i] - Func[i - 1]) / h_i);
        z = (A * alpha[i - 1] + C);
        alpha[i] = -B / z;
        beta[i] = (F - A * beta[i - 1]) / z;
    }
    splines[n - 1].c = (F - A * beta[n - 2]) / (C + A * alpha[n - 2]);

    for (size_t i = n - 2; i > 0; --i) {
        splines[i].c = alpha[i] * splines[i + 1].c + beta[i];
    }

    for (size_t i = n - 1; i > 0; --i) {
        h_i = i * step - (i - 1) * step;
        splines[i].d = (splines[i].c - splines[i - 1].c) / h_i;
        splines[i].b = h_i * (2. * splines[i].c + splines[i - 1].c) / 6. + (Func[i] - Func[i - 1]) / h_i;
    }
}

double cubic(const SplineCollectionT& splines, double x)
{
    size_t n = splines.size();
    Spline s;
    if (x <= splines[0].x) {
        s = splines[1];
    } else if (x >= splines[n - 1].x) {
        s = splines[n - 1];
    } else {
        size_t i = 0, j = n - 1;
        while (i + 1 < j) {
            size_t k = i + (j - i) / 2;
            if (x <= splines[k].x) {
                j = k;
            } else {
                i = k;
            }
        }
        s = splines[j];
    }

    double dx = (x - s.x);
    return s.a + (s.b + (s.c / 2. + s.d * dx / 6.) * dx) * dx;
}