// ME23B002 - Jay Adesara
// Assignment 7 - Programming Problem 1
// Simple Harmonic Motion: y'' + omega^2 * y = 0
// Solved using Leapfrog method (started with Euler explicit for first step)
// Exact solution: y(t) = cos(4t)

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

// -------------------------------------------------------
// Function to compute f(y1, y2) = dy/dt for the system
// y1' = y2                   (position equation)
// y2' = -omega^2 * y1        (velocity equation)
// -------------------------------------------------------
void computeF(double y1, double y2, double omega,
              double &f1, double &f2) {
    f1 = y2;
    f2 = -omega * omega * y1;
}

int main() {
    // ---- Problem parameters ----
    double omega   = 4.0;   // angular frequency
    double h       = 0.1;   // time step size
    double t_start = 0.0;
    double t_end   = 9.0;
    int    n_steps = static_cast<int>((t_end - t_start) / h + 0.5); // total steps

    // ---- Initial conditions ----
    // y1 = y (displacement),  y2 = y' (velocity)
    double y1_prev, y2_prev; // values at time n-1
    double y1_curr, y2_curr; // values at time n
    double y1_next, y2_next; // values at time n+1

    double t = t_start;

    // ---- Open output files ----
    ofstream csvFile("prog1_output.csv");
    csvFile << "iter,t,y2,y1,y_exact\n";

    // ---- Print header ----
    cout << "\nProgramming 1: Simple Harmonic Motion - Leapfrog Method\n";
    cout << "Exact solution: y(t) = cos(4t)\n\n";
    cout << left << setw(10) << "iter no."
         << setw(8)  << "t"
         << setw(16) << "y2"
         << setw(16) << "y1"
         << setw(16) << "y_exact" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    // ---- Step 0: Initial state ----
    y1_curr = 1.0;  // y(0) = y0 = 1
    y2_curr = 0.0;  // y'(0) = 0

    double y_exact = cos(omega * t);

    cout << left << setw(10) << 0
         << setw(8)  << t
         << setw(16) << y2_curr
         << setw(16) << y1_curr
         << setw(16) << y_exact << endl;
    csvFile << 0 << "," << t << "," << y2_curr << "," << y1_curr << "," << y_exact << "\n";

    // ---- Step 1: Euler Explicit (bootstrapping for leapfrog) ----
    // y1^(1) = y1^(0) + h * f1(y^(0))
    // y2^(1) = y2^(0) + h * f2(y^(0))
    y1_prev = y1_curr; // save n=0 as n-1 for leapfrog
    y2_prev = y2_curr;

    double f1, f2;
    computeF(y1_curr, y2_curr, omega, f1, f2);
    y1_curr = y1_prev + h * f1;
    y2_curr = y2_prev + h * f2;
    t += h;

    y_exact = cos(omega * t);
    cout << left << setw(10) << 1
         << setw(8)  << t
         << setw(16) << y2_curr
         << setw(16) << y1_curr
         << setw(16) << y_exact << endl;
    csvFile << 1 << "," << t << "," << y2_curr << "," << y1_curr << "," << y_exact << "\n";

    // ---- Store amplitude errors at peaks ----
    // Peaks of |cos(4t)| = 1 occur at t = n*pi/4
    // Closest time steps: pi/2 ~1.571->1.6, pi ~3.14->3.1, 3pi/2 ~4.71->4.7, 2pi ~6.28->6.3, 5pi/2 ~7.85->7.9
    vector<pair<double,double>> amp_errors; // (time, amplitude_error)

    // Threshold to identify peaks: |y_exact| > 0.98
    double y_exact_prev2 = cos(omega * (t - h));
    double y_exact_prev1 = y_exact;

    // ---- Steps 2 to n_steps: Leapfrog method ----
    // y^(n+1) = y^(n-1) + 2h * f(y^(n), t^(n))
    for (int n = 2; n <= n_steps; n++) {
        // Compute f at current step n
        computeF(y1_curr, y2_curr, omega, f1, f2);

        // Leapfrog update
        y1_next = y1_prev + 2.0 * h * f1;
        y2_next = y2_prev + 2.0 * h * f2;

        // Shift: n-1 <- n, n <- n+1
        y1_prev = y1_curr;
        y2_prev = y2_curr;
        y1_curr = y1_next;
        y2_curr = y2_next;
        t += h;

        y_exact = cos(omega * t);

        cout << left << setw(10) << n
             << setw(8)  << t
             << setw(16) << y2_curr
             << setw(16) << y1_curr
             << setw(16) << y_exact << endl;
        csvFile << n << "," << t << "," << y2_curr << "," << y1_curr << "," << y_exact << "\n";

        // ---- Detect peaks of exact solution (local max of |y_exact|) ----
        // A peak occurs when previous |y_exact| > current and > the one before
        if (fabs(y_exact_prev1) > fabs(y_exact_prev2) &&
            fabs(y_exact_prev1) > fabs(y_exact) &&
            fabs(y_exact_prev1) > 0.9) {
            double t_peak = t - h;
            // y1_curr is n, y1_prev is n-1, so amplitude is y1_prev
            double amp_error = y1_prev - y_exact_prev1;
            amp_errors.push_back({t_peak, amp_error});
        }
        y_exact_prev2 = y_exact_prev1;
        y_exact_prev1 = y_exact;
    }

    csvFile.close();

    // ---- Print amplitude errors at peaks ----
    cout << "\namplitude errors at peaks are:\n";
    cout << "--------------------------------\n";
    for (auto &pe : amp_errors) {
        cout << "at time step " << pe.first
             << " amplitude error is " << pe.second << "\n";
    }

    // ---- Save amplitude errors to file for plotting ----
    ofstream ampFile("prog1_amp_errors.csv");
    ampFile << "time,amplitude_error\n";
    for (auto &pe : amp_errors) {
        ampFile << pe.first << "," << pe.second << "\n";
    }
    ampFile.close();

    cout << "\nOutputs saved to prog1_output.csv and prog1_amp_errors.csv\n";
    return 0;
}
