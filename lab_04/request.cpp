#include "request.h"
#include "drawing.h"
#include <fstream>
#include <ctime>

void request_handle(request &req)
{
    switch (req.operation)
    {
        case DRAW_CIRCLE:
            drawing_circle(req.scene, req.figure, true);
            break;
        case DRAW_ELLIPSE:
            drawing_ellipse(req.scene, req.figure, true);
            break;
        case DRAW_SPECTRUM_CIRCLE:
            drawing_spectrum_circle(req.scene, req.spectrum);
            break;
        case DRAW_SPECTRUM_ELLIPSE:
            drawing_spectrum_ellispe(req.scene, req.spectrum);
            break;
        case DRAW_ALL:
            drawing_all(req.scene, req.gv, req.data);
            break;
        case MEASURE_TIME:
            measure_time(req.scene, req.spectrum);
            break;
    }
}

std::vector<double> measure_circle_time(canvas_t &scene, const spectrum_t &spectrum)
{
    const int iterations = 10000;
    std::vector<double> data;
    figure_t circle;
    circle.center = spectrum.center;
    circle.color = spectrum.color;
    circle.method = spectrum.method;

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;
    auto end = high_resolution_clock::now();
    auto start = high_resolution_clock::now();

//    clock_t begin, end;
//    double time_spent_circle;
//    double sum_circle = 0;

    for (int i = 0; i < spectrum.n; i++)
    {
        circle.ra = spectrum.ra + i * spectrum.dra;
        start = high_resolution_clock::now();
        for (int j = 0; j < iterations; j++)
        {
//            begin = clock();
            drawing_circle(scene, circle, false);
//            end = clock();
//            time_spent_circle = (double)(end - begin);
//            sum_circle += time_spent_circle;
        }
        end = high_resolution_clock::now();
        double time = (double)duration_cast<microseconds>(end - start).count() / iterations;
//        data.push_back(sum_circle / iterations);
        data.push_back(time);
    }
    return data;
}

std::vector<double> measure_ellipse_time(canvas_t &scene, const spectrum_t &spectrum)
{
    const int iterations = 10000;
    std::vector<double> data;
    figure_t ellipse;
    ellipse.center = spectrum.center;
    ellipse.color = spectrum.color;
    ellipse.method = spectrum.method;

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;
    auto end = high_resolution_clock::now();
    auto start = high_resolution_clock::now();

//    clock_t begin, end;
//    double time_spent_circle;
//    double sum_circle = 0;

    for (int i = 0; i < spectrum.n; i++)
    {
        ellipse.ra = spectrum.ra + i * spectrum.dra;
        ellipse.rb = spectrum.rb + i * spectrum.drb;
        start = high_resolution_clock::now();

        for (int j = 0; j < iterations; j++)
        {
//            begin = clock();
            drawing_ellipse(scene, ellipse, false);
//            end = clock();
//            time_spent_circle = (double)(end - begin);
//            sum_circle += time_spent_circle;
        }
        end = high_resolution_clock::now();
        double time = (double)duration_cast<microseconds>(end - start).count() / iterations;
//        data.push_back(sum_circle / ITERATION);
        data.push_back(time);
    }
    return data;
}



void measure_time(canvas_t &scene, spectrum_t &spectrum)
{
    std::vector<std::vector<double>> times;
    if (spectrum.type == CIRCLE)
    {
        for (int m = CANONICAL; m <= MIDDLE_POINT; m++)
        {
            spectrum.method = (method_t) m;
            std::vector<double> time = measure_circle_time(scene, spectrum);
            times.push_back(time);
        }
    }
    else
    {
        for (int m = CANONICAL; m <= MIDDLE_POINT; m++)
        {
            spectrum.method = (method_t) m;
            std::vector<double> time = measure_ellipse_time(scene, spectrum);
            times.push_back(time);
        }
    }

    std::ofstream out("../lab_04/time_res.txt");
    if (out.is_open())
    {
        if (spectrum.type == CIRCLE)
            out << "0 ";
        else
            out << "1 ";
        out << spectrum.ra << " " << spectrum.dra << " " << spectrum.n << "\n";
        for (size_t i = 0; i < times.size(); i++)
            for (size_t j = 0; j < times[i].size(); j++)
                out << times[i][j] << "\n";
    }
    out.close();
    system("python ../lab_04/time.py");


}
