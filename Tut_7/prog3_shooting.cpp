// ME23B002 - Jay Adesara
// Assignment 7 - Programming Problem 3
// Boundary Value Problem using Shooting Method
// ODE: y'' = y^2
// BCs: y(0) = 0,  dy/dx|_{x=1} = -0.8
// Grid: dx = 0.01, Euler Explicit, Secant method for shooting

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

// -------------------------------------------------------
// Shoot from x=0 with given slope s = dy/dx|_{x=0}
// Returns: dy/dx at x=1 (we want this to equal -0.8)
// Also fills vectors y_vals and dy_vals if non-null
// ODE system:
//   z1 = y           z1' = z2
//   z2 = y'          z2' = z1^2  (since y'' = y^2)
// -------------------------------------------------------
double shoot(double s, double dx,
             vector<double> *y_vals  = nullptr,
             vector<double> *dy_vals = nullptr,
             vector<double> *x_vals  = nullptr) {

    double x  = 0.0;
    double z1 = 0.0;   // y(0) = 0  (BC1)
    double z2 = s;     // dy/dx at x=0 = s (shooting parameter)

    int n = static_cast<int>(1.0 / dx + 0.5);

    if (y_vals)  y_vals->clear();
    if (dy_vals) dy_vals->clear();
    if (x_vals)  x_vals->clear();

    if (y_vals)  y_vals->push_back(z1);
    if (dy_vals) dy_vals->push_back(z2);
    if (x_vals)  x_vals->push_back(x);

    // Euler explicit marching from x=0 to x=1
    for (int i = 0; i < n; i++) {
        double f1 = z2;           // dz1/dx = z2
        double f2 = z1 * z1;     // dz2/dx = y^2 = z1^2

        z1 += dx * f1;
        z2 += dx * f2;
        x  += dx;

        if (y_vals)  y_vals->push_back(z1);
        if (dy_vals) dy_vals->push_back(z2);
        if (x_vals)  x_vals->push_back(x);
    }

    return z2;  // dy/dx at x=1
}

int main() {
    // ---- Parameters ----
    double dx       = 0.01;
    double bc_left  = 0.0;    // y(0) = 0
    double bc_right = -0.8;   // dy/dx|_{x=1} = -0.8
    double tol      = 1e-5;   // convergence criterion

    cout << "\nProgramming 3: Shooting Method for BVP\n";
    cout << "ODE: y'' = y^2,   y(0)=0,   y'(1) = -0.8\n";
    cout << "Grid size dx = " << dx << "\n\n";

    // ---- Open output file ----
    ofstream guessFile("prog3_guesses.csv");
    guessFile << "guess_no,s,y_at_1,dy_at_1\n";

    // ---- First two guesses (as specified in problem) ----
    double s1 =  1.0;   // First guess: dy/dx|_{x=0} = 1
    double s2 = -1.0;   // Second guess: dy/dx|_{x=0} = -1

    double g1 = shoot(s1, dx);  // dy/dx at x=1 for guess 1
    double g2 = shoot(s2, dx);  // dy/dx at x=1 for guess 2

    // Residuals: phi(s) = dy/dx|_{x=1} - bc_right  (want phi=0)
    double phi1 = g1 - bc_right;
    double phi2 = g2 - bc_right;

    cout << "guess 1: " << s1 << "\n";
    cout << "  dy/dx|_{x=0} = " << s1
         << "  |  dy/dx|_{x=1} = " << g1 << "\n\n";
    cout << "guess 2: " << s2 << "\n";
    cout << "  dy/dx|_{x=0} = " << s2
         << "  |  dy/dx|_{x=1} = " << g2 << "\n\n";

    guessFile << 1 << "," << s1 << "," << "," << g1 << "\n";
    guessFile << 2 << "," << s2 << "," << "," << g2 << "\n";

    // ---- Secant method iterations ----
    // s_{n+1} = s_n - phi_n * (s_n - s_{n-1}) / (phi_n - phi_{n-1})
    int guess_no = 2;
    double s_new, g_new, phi_new;

    while (true) {
        // Secant update
        s_new = s2 - phi2 * (s2 - s1) / (phi2 - phi1);
        g_new = shoot(s_new, dx);
        phi_new = g_new - bc_right;

        guess_no++;
        cout << "guess " << guess_no << ": " << s_new << "\n";
        cout << "  dy/dx|_{x=0} = " << s_new
             << "  |  dy/dx|_{x=1} = " << g_new << "\n\n";

        guessFile << guess_no << "," << s_new << "," << "," << g_new << "\n";

        // Check convergence criterion: |dy/dx|_{x=1} + 0.8| < tol
        if (fabs(g_new + 0.8) < tol) {
            cout << "Solution converged for guess " << guess_no << "\n";
            break;
        }

        // Update for next iteration
        s1   = s2;    phi1 = phi2;
        s2   = s_new; phi2 = phi_new;

        if (guess_no > 100) {
            cout << "Warning: did not converge in 100 iterations\n";
            break;
        }
    }

    guessFile.close();

    // ---- Final converged solution ----
    vector<double> y_vals, dy_vals, x_vals;
    shoot(s_new, dx, &y_vals, &dy_vals, &x_vals);

    // ---- Print table for x = 0, 0.1, ..., 1.0 ----
    cout << "\nFor the converged solution:\n";
    cout << left << setw(10) << "x"
         << setw(15) << "y(x)"
         << setw(15) << "dy/dx" << "\n";
    cout << "---------------------------------------------------\n";

    // Open CSV for full solution (all x)
    ofstream solFile("prog3_solution.csv");
    solFile << "x,y,dydx\n";
    for (int i = 0; i < (int)x_vals.size(); i++) {
        solFile << x_vals[i] << "," << y_vals[i] << "," << dy_vals[i] << "\n";
    }
    solFile.close();

    // Print at x = 0, 0.1, 0.2, ..., 1.0  (every 10th point since dx=0.01)
    int step10 = static_cast<int>(0.1 / dx + 0.5);
    for (int i = 0; i < (int)x_vals.size(); i += step10) {
        cout << left << setw(10) << x_vals[i]
             << setw(15) << y_vals[i]
             << setw(15) << dy_vals[i] << "\n";
    }
    // Print last point x=1.0 if not already printed
    int last = (int)x_vals.size() - 1;
    if (last % step10 != 0) {
        cout << left << setw(10) << x_vals[last]
             << setw(15) << y_vals[last]
             << setw(15) << dy_vals[last] << "\n";
    }

    cout << "\nOutputs saved to prog3_guesses.csv and prog3_solution.csv\n";
    return 0;
}
