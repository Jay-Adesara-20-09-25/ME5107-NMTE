// ME23B002 - Jay Adesara
// IVP Solver

#include <bits/stdc++.h>
using namespace std;

//-----------------------------------------------------------
// RHS of ODE
//-----------------------------------------------------------
double compute_rhs(double time, double solution)
{
    return -10.0 * (solution - exp(-time)) - exp(-time);
}

//-----------------------------------------------------------
// Exact solution
//-----------------------------------------------------------
double compute_exact_solution(double time)
{
    return exp(-time) + 9.0 * exp(-10.0 * time);
}

//-----------------------------------------------------------
// Newton-Raphson for implicit methods
//-----------------------------------------------------------
double solve_implicit(double t, double y_old, double h)
{
    double y = y_old;

    while (true)
    {
        double g = y - y_old - h * compute_rhs(t + h, y);
        double dg = 1.0 + 10.0 * h;

        double y_new = y - g / dg;

        if (fabs(y_new - y) < 1e-10)
            break;

        y = y_new;
    }

    return y;
}

//-----------------------------------------------------------
// Compute slope depending on method
//-----------------------------------------------------------
double compute_slope(double t, double y, double h, int method)
{
    if (method == 1) // Explicit Euler
        return compute_rhs(t, y);

    else if (method == 2) // Implicit Euler
    {
        double y_next = solve_implicit(t, y, h);
        return compute_rhs(t + h, y_next);
    }

    else if (method == 3) // Trapezoidal
    {
        double y_next = solve_implicit(t, y, h);
        return 0.5 * (compute_rhs(t, y) + compute_rhs(t + h, y_next));
    }

    else if (method == 4) // RK2
    {
        double k1 = compute_rhs(t, y);
        double k2 = compute_rhs(t + h / 2, y + h * k1 / 2);
        return k2;
    }

    else if (method == 5) // RK4
    {
        double k1 = compute_rhs(t, y);
        double k2 = compute_rhs(t + h / 2, y + h * k1 / 2);
        double k3 = compute_rhs(t + h / 2, y + h * k2 / 2);
        double k4 = compute_rhs(t + h, y + h * k3);

        return (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    }

    else if (method == 6) // RKF45
    {
        double k1 = h * compute_rhs(t, y);
        double k2 = h * compute_rhs(t + h / 4, y + k1 / 4);
        double k3 = h * compute_rhs(t + 3 * h / 8, y + (3 * k1 + 9 * k2) / 32);
        double k4 = h * compute_rhs(t + 12 * h / 13, y + (1932 * k1 - 7200 * k2 + 7296 * k3) / 2197);
        double k5 = h * compute_rhs(t + h, y + (439 * k1) / 216 - 8 * k2 + (3680 * k3) / 513 - (845 * k4) / 4104);
        double k6 = h * compute_rhs(t + h / 2, y - (8 * k1) / 27 + 2 * k2 - (3544 * k3) / 2565 + (1859 * k4) / 4104 - (11 * k5) / 40);

        double increment =
            (16 * k1) / 135 + (6656 * k3) / 12825 +
            (28561 * k4) / 56430 - (9 * k5) / 50 + (2 * k6) / 55;

        return increment / h;
    }

    return 0.0;
}

//-----------------------------------------------------------
// MAIN
//-----------------------------------------------------------
int main()
{
    vector<double> h_values = {0.1, 0.05, 0.025, 0.0125, 0.00625};

    vector<string> methods = {
        "ExplicitEuler", "ImplicitEuler", "Trapezoidal",
        "RK2", "RK4", "RKF45"};

    cout << fixed << setprecision(6);

    // Loop over methods
    for (int m = 1; m <= 6; m++)
    {
        vector<double> errors;

        // Loop over step sizes
        for (double h : h_values)
        {
            int N = int(0.8 / h);

            vector<double> t(N + 1), y(N + 1), y_exact(N + 1);

            // Initial condition
            t[0] = 0.0;
            y[0] = 10.0;
            y_exact[0] = compute_exact_solution(0.0);

            // Time integration
            for (int i = 1; i <= N; i++)
            {
                t[i] = t[i - 1] + h;

                y[i] = y[i - 1] +
                       h * compute_slope(t[i - 1], y[i - 1], h, m);

                y_exact[i] = compute_exact_solution(t[i]);
            }

            // Save solution file
            string filename = methods[m - 1] + "_h_" + to_string(h) + ".txt";
            ofstream file(filename);

            file << "t numerical exact\n";
            for (int i = 0; i <= N; i++)
                file << t[i] << " " << y[i] << " " << y_exact[i] << "\n";

            file.close();

            // Compute error at final time
            errors.push_back(fabs(y[N] - y_exact[N]));
        }

        // -----------------------------
        // PRINT p VALUES
        // -----------------------------
        cout << "\n==== " << methods[m - 1] << " ====\n";

        for (int i = 0; i < errors.size() - 1; i++)
        {
            double p = log2(errors[i] / errors[i + 1]);

            cout << "p between h = " << h_values[i]
                 << " and " << h_values[i + 1]
                 << " = " << p << endl;
        }

        // -----------------------------
        // SAVE ERROR FILE
        // -----------------------------
        ofstream ef(methods[m - 1] + "_error.txt");

        ef << "h error\n";
        for (int i = 0; i < h_values.size(); i++)
            ef << h_values[i] << " " << errors[i] << "\n";

        ef << "\nOrder:\n";
        for (int i = 0; i < errors.size() - 1; i++)
            ef << log2(errors[i] / errors[i + 1]) << "\n";

        ef.close();
    }

    return 0;
}