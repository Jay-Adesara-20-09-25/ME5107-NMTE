// ME23B002 - Jay Adesara
// Non-uniform Grid: First Derivative Computation
// Methods:
// (a) Direct central difference in x-space
// (b) Coordinate transformation via ξ-space

#include <bits/stdc++.h>
using namespace std;

// --------------------------------------------------
// Function definition: f(x) = 1 - x^8
// --------------------------------------------------
double compute_function(double x)
{
    return 1.0 - pow(x, 8);
}

// --------------------------------------------------
// Exact derivative: f'(x) = -8x^7
// --------------------------------------------------
double exact_derivative(double x)
{
    return -8.0 * pow(x, 7);
}

// --------------------------------------------------
// Generate grid:
// ξ_j = π j / N  (uniform)
// x_j = cos(ξ_j) (non-uniform)
// --------------------------------------------------
void generate_grid(int N, vector<double> &xi, vector<double> &x)
{
    double pi = acos(-1);

    for (int j = 0; j <= N; j++)
    {
        xi[j] = pi * j / N;
        x[j] = cos(xi[j]);
    }
}

// --------------------------------------------------
// Compute function values at grid points
// --------------------------------------------------
void compute_function_values(const vector<double> &x, vector<double> &f)
{
    for (int j = 0; j < x.size(); j++)
    {
        f[j] = compute_function(x[j]);
    }
}

// --------------------------------------------------
// Method (a): Direct central difference on non-uniform grid
// f'_j ≈ (f_{j+1} - f_{j-1}) / (x_{j+1} - x_{j-1})
// --------------------------------------------------
double direct_method(int j, const vector<double> &x, const vector<double> &f)
{
    return (f[j + 1] - f[j - 1]) / (x[j + 1] - x[j - 1]);
}

// --------------------------------------------------
// Method (b): Coordinate transformation
// df/dx = (df/dξ) * (dξ/dx)
//
// df/dξ ≈ (f_{j+1} - f_{j-1}) / (2Δξ)
// dξ/dx = -1 / sin(ξ)
// --------------------------------------------------
double transform_method(int j, const vector<double> &xi,
                        const vector<double> &f, double dxi)
{
    double df_dxi = (f[j + 1] - f[j - 1]) / (2.0 * dxi);
    double dxi_dx = -1.0 / sin(xi[j]);

    return df_dxi * dxi_dx;
}

int main()
{
    int N = 32;
    double pi = acos(-1);

    // Grid spacing in ξ-space
    double dxi = pi / N;

    // Storage vectors
    vector<double> xi(N + 1), x(N + 1), f(N + 1);

    // Step 1: Generate grid
    generate_grid(N, xi, x);

    // Step 2: Compute function values
    compute_function_values(x, f);

    // Step 3: Open file for output
    ofstream file("nonuniform_results.txt");

    cout << fixed << setprecision(8);
    file << fixed << setprecision(8);

    // Header
    cout << "j\t x\t\t Direct\t\t Transform\t Exact\n";
    file << "x direct transform exact\n";

    // Step 4: Compute derivatives (excluding boundaries)
    for (int j = 1; j < N; j++)
    {
        // Method (a)
        double direct = direct_method(j, x, f);

        // Method (b)
        double transform = transform_method(j, xi, f, dxi);

        // Exact value
        double exact = exact_derivative(x[j]);

        // Print to terminal
        cout << j << "\t"
             << x[j] << "\t"
             << direct << "\t"
             << transform << "\t"
             << exact << endl;

        // Write to file
        file << x[j] << " "
             << direct << " "
             << transform << " "  
             << exact << "\n";
    }

    file.close();

    cout << "\nData saved to nonuniform_results.txt\n";

    return 0;
}