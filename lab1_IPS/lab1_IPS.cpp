#define _USE_MATH_DEFINES

#include <iostream>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>

using namespace std::chrono;
using namespace std;

inline double func(double x) {
    return  8 / (2 + 2 * pow(x, 2));
}

void defineLeftRectangleIntegral(double a, double b, int n) {
    double sum = 0.0; 
    double h = (b - a) / n; // размерность шага
    double* y = new double[n];
    double x;

    //#pragma loop(ivdep) // ignore vector dependencies
    //#pragma loop(no_vector)
    for (int i = 0; i < n; i++) {
        x = a + i * h;
        y[i] = func(x);
    }
    for (int i = 0; i < n; i++) {
        sum += y[i];
    }
    double integral = sum * h;
    cout << "Left integral = " << integral << endl;
    cout << "Count of intervals = " << n << endl;
    cout << "Accuracy (%) = " << 100 - (abs(2 * M_PI - integral) / 2 * M_PI * 100) << endl;
}

void defineRightRectangleIntegral(double a, double b, int n) {
    double sum = 0.0;
    double h = (b - a) / n; // размерность шага
    double* y = new double[n];
    double x;

    //#pragma loop(ivdep) // ignore vector dependencies
    //#pragma loop(no_vector)
    for (int i = 1; i <= n; i++) {
        x = a + h * i;
        y[i - 1] = func(x);
    }
    for (int i = 0; i < n; i++) {
        sum += y[i];
    }
    double integral = sum * h;
    cout << "Right integral = " << integral << endl;
    cout << "Count of intervals = " << n << endl;
    cout << "Accuracy (%) = " << 100 - (abs(2 * M_PI - integral) / 2 * M_PI * 100) << endl;
}

void serialCode(double a, double b, int intervals[], int arrSize) {
    int n;
    for (int i = 0; i < arrSize; i++) {
        n = intervals[i];
        cout << fixed;
        cout << setprecision(10);
        high_resolution_clock::time_point tp1 = high_resolution_clock::now();
        defineLeftRectangleIntegral(a, b, n);
        defineRightRectangleIntegral(a, b, n);
        high_resolution_clock::time_point tp2 = high_resolution_clock::now();
        duration<double> duration = (tp2 - tp1);
        cout << "Duration is: " << duration.count() << " seconds" << "\n\n";
    }
}

void threadCode(double a, double b, int intervals[], int arrSize) {
    int n;
    for (int i = 0; i < arrSize; i++) {
        n = intervals[i];
        cout << fixed;
        cout << setprecision(10);
        high_resolution_clock::time_point tp1 = high_resolution_clock::now();
        thread defineLeftRectangleIntegral(defineLeftRectangleIntegral, a, b, n);
        thread defineRightRectangleIntegral(defineRightRectangleIntegral, a, b, n);
        defineLeftRectangleIntegral.join();
        defineRightRectangleIntegral.join();
        high_resolution_clock::time_point tp2 = high_resolution_clock::now();
        duration<double> duration = (tp2 - tp1);
        cout << "Duration is: " << duration.count() << " seconds" << "\n\n";
    }
}

int main() {
    double a = -1, b = 1;
    int intervals[] = { 100, 1000, 10000, 100000, 1000000 };
    int arrSize = sizeof(intervals) / sizeof(int);
    serialCode(a, b, intervals, arrSize);
    //threadCode(a, b, intervals, arrSize);
}