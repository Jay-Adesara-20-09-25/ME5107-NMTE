// ME23B002 - Jay Adesara
// Finite Difference Error Analysis with Theoretical Comparison

#include <bits/stdc++.h>
using namespace std;

// -----------------------------
// Function u(x)
// -----------------------------
double f(double x)
{
    return 7.0 - x * tan(x);
}

// -----------------------------
// Exact derivative
// -----------------------------
double exact_derivative(double x)
{
    return -tan(x) - x / (cos(x) * cos(x));
}

// -----------------------------
// First-order scheme
// -----------------------------
double first_order(double x, double dx, int dir)
{
    if (dir == 1)
        return (f(x + dx) - f(x)) / dx;
    else
        return (f(x) - f(x - dx)) / dx;
}

// -----------------------------
// Second-order scheme
// -----------------------------
double second_order(double x, double dx, int dir)
{
    if (dir == 1)
        return (-f(x + 2 * dx) + 4 * f(x + dx) - 3 * f(x)) / (2 * dx);
    else
        return (3 * f(x) - 4 * f(x - dx) + f(x - 2 * dx)) / (2 * dx);
}

// -----------------------------
// Fourth-order scheme
// -----------------------------
double fourth_order(double x, double dx, int dir)
{
    if (dir == 1)
        return (-3 * f(x + 4 * dx) + 16 * f(x + 3 * dx) - 36 * f(x + 2 * dx) + 48 * f(x + dx) - 25 * f(x)) / (12 * dx);
    else
        return (25 * f(x) - 48 * f(x - dx) + 36 * f(x - 2 * dx) - 16 * f(x - 3 * dx) + 3 * f(x - 4 * dx)) / (12 * dx);
}

// -----------------------------
// Compute ∞-norm error
// -----------------------------
void compute_error(double dx, double &e1, double &e2, double &e4)
{
    double start = -1.0, end = 1.0;
    e1 = e2 = e4 = 0.0;

    for (double x = start; x <= end; x += dx)
    {
        double exact = exact_derivative(x);

        double a1 = (x + dx <= end) ? first_order(x, dx, 1) : first_order(x, dx, 2);
        double a2 = (x + 2 * dx <= end) ? second_order(x, dx, 1) : second_order(x, dx, 2);
        double a4 = (x + 4 * dx <= end) ? fourth_order(x, dx, 1) : fourth_order(x, dx, 2);

        e1 = max(e1, abs(a1 - exact));
        e2 = max(e2, abs(a2 - exact));
        e4 = max(e4, abs(a4 - exact));
    }
}

// -----------------------------
// Compute slope
// -----------------------------
double slope(double e1, double e2, double h1, double h2)
{
    return log(e2 / e1) / log(h2 / h1);
}

// -----------------------------
// MAIN
// -----------------------------
int main()
{
    vector<double> dx = {0.1, 0.01, 0.001};

    vector<double> err1(3), err2(3), err4(3);

    cout << fixed << setprecision(10);

    // Compute errors
    for (int i = 0; i < dx.size(); i++)
        compute_error(dx[i], err1[i], err2[i], err4[i]);

    // -----------------------------
    // ERROR TABLE
    // -----------------------------
    cout << "\n========== ERROR TABLE (∞-norm) ==========\n";
    cout << setw(10) << "dx"
         << setw(20) << "First"
         << setw(20) << "Second"
         << setw(20) << "Fourth" << endl;

    for (int i = 0; i < dx.size(); i++)
    {
        cout << setw(10) << dx[i]
             << setw(20) << err1[i]
             << setw(20) << err2[i]
             << setw(20) << err4[i] << endl;
    }

    // -----------------------------
    // SLOPES (AVERAGE)
    // -----------------------------
    double s1 = (slope(err1[0], err1[1], dx[0], dx[1]) +
                 slope(err1[1], err1[2], dx[1], dx[2])) /
                2.0;

    double s2 = (slope(err2[0], err2[1], dx[0], dx[1]) +
                 slope(err2[1], err2[2], dx[1], dx[2])) /
                2.0;

    double s4 = (slope(err4[0], err4[1], dx[0], dx[1]) +
                 slope(err4[1], err4[2], dx[1], dx[2])) /
                2.0;

    // -----------------------------
    // THEORETICAL VALUES
    // -----------------------------
    double t1 = 1.0, t2 = 2.0, t4 = 4.0;

    // -----------------------------
    // PRINT COMPARISON
    // -----------------------------
    cout << "\n========== SLOPE COMPARISON ==========\n";
    cout << setw(15) << "Scheme"
         << setw(20) << "Computed"
         << setw(20) << "Theoretical"
         << setw(20) << "Error" << endl;

    cout << setw(15) << "First"
         << setw(20) << s1
         << setw(20) << t1
         << setw(20) << abs(s1 - t1) << endl;

    cout << setw(15) << "Second"
         << setw(20) << s2
         << setw(20) << t2
         << setw(20) << abs(s2 - t2) << endl;

    cout << setw(15) << "Fourth"
         << setw(20) << s4
         << setw(20) << t4
         << setw(20) << abs(s4 - t4) << endl;

    // -----------------------------
    // FILE OUTPUT
    // -----------------------------
    ofstream f1("first_order.txt"), f2("second_order.txt"), f4("fourth_order.txt");

    for (int i = 0; i < dx.size(); i++)
    {
        f1 << dx[i] << " " << err1[i] << "\n";
        f2 << dx[i] << " " << err2[i] << "\n";
        f4 << dx[i] << " " << err4[i] << "\n";
    }

    f1.close();
    f2.close();
    f4.close();

    cout << "\nData written to files successfully.\n";

    return 0;
}