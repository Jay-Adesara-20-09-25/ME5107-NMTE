// ME23B002 - Jay Adesara
// Assignment 7 - Programming Problem 2
// Stiff Chemical Reaction System (Robertson Problem)
// Methods: Explicit Euler, Trapezoidal (implicit), RK4
//
// STIFFNESS NOTE:
//   Explicit Euler and RK4 blow up because the fast reaction
//   (B+B->C, rate 3e7) creates a stiffness ratio ~3e7/0.04 ~ 7.5e8.
//   Even tiny step sizes (1e-5) are too large for explicit methods.
//
//   The Trapezoidal method solves the implicit system using ONE
//   linearized Newton step per time step (starting from y_n):
//       (I - h/2 * J_n) * delta = h * f(y_n)
//       y_{n+1} = y_n + delta
//   This is the A-stable linearized Crank-Nicolson approach.

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

// ---- Reaction rate constants ----
const double K1 = 0.04;     // A -> B (slow)
const double K2 = 3.0e7;    // B+B -> C+B (very fast)
const double K3 = 1.0e4;    // B+C -> A+C (fast)

// -------------------------------------------------------
// RHS of ODE system
// y[0]=A, y[1]=B, y[2]=C
// -------------------------------------------------------
void computeF(const double y[], double f[]) {
    f[0] = -K1*y[0] + K3*y[1]*y[2];
    f[1] =  K1*y[0] - K3*y[1]*y[2] - K2*y[1]*y[1];
    f[2] =  K2*y[1]*y[1];
}

// -------------------------------------------------------
// Jacobian dF/dy (row-major, 3x3)
// -------------------------------------------------------
void computeJacobian(const double y[], double J[9]) {
    J[0] = -K1;       J[1] = K3*y[2];                J[2] = K3*y[1];
    J[3] =  K1;       J[4] = -K3*y[2] - 2.0*K2*y[1]; J[5] = -K3*y[1];
    J[6] =  0.0;      J[7] = 2.0*K2*y[1];             J[8] = 0.0;
}

// -------------------------------------------------------
// Solve 3x3 linear system A*x = b (Gaussian elim + pivoting)
// -------------------------------------------------------
bool solve3x3(double A[3][3], const double b[3], double x[3]) {
    double M[3][4];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) M[i][j] = A[i][j];
        M[i][3] = b[i];
    }
    for (int col = 0; col < 3; col++) {
        int piv = col;
        for (int r = col+1; r < 3; r++)
            if (fabs(M[r][col]) > fabs(M[piv][col])) piv = r;
        for (int k = 0; k < 4; k++) swap(M[col][k], M[piv][k]);
        if (fabs(M[col][col]) < 1e-300) return false;
        for (int r = col+1; r < 3; r++) {
            double f = M[r][col]/M[col][col];
            for (int k = col; k < 4; k++) M[r][k] -= f*M[col][k];
        }
    }
    for (int i = 2; i >= 0; i--) {
        x[i] = M[i][3];
        for (int j = i+1; j < 3; j++) x[i] -= M[i][j]*x[j];
        x[i] /= M[i][i];
    }
    return true;
}

// -------------------------------------------------------
// Explicit Euler step: y_{n+1} = y_n + h*f(y_n)
// -------------------------------------------------------
void eulerStep(double y[], double h) {
    double f[3]; computeF(y, f);
    for (int i=0; i<3; i++) y[i] += h*f[i];
}

// -------------------------------------------------------
// Trapezoidal implicit step using linearized Newton from y_n:
//   (I - h/2 * J_n) * delta = h * f(y_n)
//   y_{n+1} = y_n + delta
// This single Newton step is A-stable and effectively handles
// the stiff fast transient in one iteration.
// -------------------------------------------------------
void trapezoidalStep(double y[], double h) {
    double f_n[3], J[9];
    computeF(y, f_n);
    computeJacobian(y, J);

    // Build (I - h/2 * J)
    double JR[3][3];
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            JR[i][j] = (i==j ? 1.0 : 0.0) - 0.5*h*J[i*3+j];

    // RHS = h * f(y_n)
    double rhs[3];
    for (int i=0; i<3; i++) rhs[i] = h*f_n[i];

    // Solve for delta
    double delta[3] = {0,0,0};
    solve3x3(JR, rhs, delta);

    for (int i=0; i<3; i++) y[i] += delta[i];
}

// -------------------------------------------------------
// RK4 step
// -------------------------------------------------------
void rk4Step(double y[], double h) {
    double k1[3], k2[3], k3[3], k4[3], ytmp[3];
    computeF(y, k1);
    for (int i=0;i<3;i++) ytmp[i]=y[i]+0.5*h*k1[i];
    computeF(ytmp, k2);
    for (int i=0;i<3;i++) ytmp[i]=y[i]+0.5*h*k2[i];
    computeF(ytmp, k3);
    for (int i=0;i<3;i++) ytmp[i]=y[i]+h*k3[i];
    computeF(ytmp, k4);
    for (int i=0;i<3;i++) y[i] += (h/6.0)*(k1[i]+2*k2[i]+2*k3[i]+k4[i]);
}

// -------------------------------------------------------
// Run a simulation with given method and step size
// method: 0=Euler, 1=Trapezoidal, 2=RK4
// -------------------------------------------------------
void solveMethod(int method, double h,
                 const string &methodName,
                 ofstream &logCSV, ofstream &fullCSV) {

    const double t_end      = 3000.0;
    const long long n_steps = static_cast<long long>(t_end / h + 0.5);
    const int print_first   = 10;
    // Save ~3000 points for plotting
    long long save_every = max(1LL, n_steps / 3000LL);

    double y[3] = {0.9, 0.1, 0.0};
    double t    = 0.0;
    bool blown  = false;

    cout << "\nUsing " << methodName << " method:\n";
    cout << "For h (time step size) = " << scientific << setprecision(3) << h << "\n";
    cout << "Total number of time steps = " << n_steps << "\n";
    cout << fixed << setprecision(7);

    // Print row to console
    auto printRow = [&](long long n) {
        if (n <= (long long)print_first)
            cout << "At time t = " << t
                 << " y1 = " << y[0]
                 << "; y2 = " << y[1]
                 << "; y3 = " << y[2] << "\n";
    };

    printRow(0);

    for (long long n = 1; n <= n_steps; n++) {
        if (!blown) {
            switch(method) {
                case 0: eulerStep(y, h);        break;
                case 1: trapezoidalStep(y, h);  break;
                case 2: rk4Step(y, h);          break;
            }
        }
        t += h;

        if (!isfinite(y[0]) || !isfinite(y[1]) || !isfinite(y[2]))
            blown = true;

        printRow(n);

        // Save to CSV for plotting
        if (t > 0 && (n % save_every == 0 || n == n_steps))
            fullCSV << methodName << "," << h << "," << t << ","
                    << y[0] << "," << y[1] << "," << y[2] << "\n";
    }

    // Record final values
    logCSV << methodName << "," << h << ","
           << y[0] << "," << y[1] << "," << y[2] << "\n";
}

int main() {
    cout << "Programming 2: Stiff Chemical Reaction System\n";
    cout << "==============================================\n";
    cout << "  dy1/dt = -0.04*y1 + 1e4*y2*y3\n"
         << "  dy2/dt =  0.04*y1 - 1e4*y2*y3 - 3e7*y2^2\n"
         << "  dy3/dt =  3e7*y2^2\n"
         << "  IC: y1(0)=0.9, y2(0)=0.1, y3(0)=0  |  t in [0, 3000]\n\n";

    ofstream logCSV("prog2_final_values.csv");
    logCSV << "method,h,y1_t3000,y2_t3000,y3_t3000\n";

    ofstream fullCSV("prog2_timeseries.csv");
    fullCSV << "method,h,t,y1,y2,y3\n";

    double hv[3] = {1e-3, 1e-4, 1e-5};

    // ---- Explicit Euler (demonstrates blow-up) ----
    for (int i=0; i<3; i++) solveMethod(0, hv[i], "Explicit Euler", logCSV, fullCSV);

    // ---- Trapezoidal (stable, handles stiffness) ----
    for (int i=0; i<3; i++) solveMethod(1, hv[i], "Trapezoidal",    logCSV, fullCSV);

    // ---- RK4 (also blows up) ----
    for (int i=0; i<3; i++) solveMethod(2, hv[i], "RK4",            logCSV, fullCSV);

    logCSV.close();
    fullCSV.close();

    // ---- Print comparison table ----
    cout << "\n\na) Species concentration yi at t = 3000s:\n";
    cout << left
         << setw(22) << "Method"
         << setw(10) << "h"
         << setw(14) << "y1(3000)"
         << setw(14) << "y2(3000)"
         << setw(14) << "y3(3000)" << "\n"
         << string(74, '-') << "\n"
         << setw(22) << "Exact (steady state)"
         << setw(10) << "---"
         << setw(14) << 0
         << setw(14) << 0
         << setw(14) << 1 << "\n";
    cout << "\n(Explicit Euler and RK4 blow up for all step sizes)\n";
    cout << "(Trapezoidal results are in prog2_final_values.csv)\n";
    cout << "\nOutputs saved: prog2_final_values.csv, prog2_timeseries.csv\n";
    return 0;
}
