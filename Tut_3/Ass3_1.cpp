// ME23B002 - Jay Adesara

#include <bits/stdc++.h>
using namespace std;

/*
    Function : f
    Purpose  : Defines the function f(x) = sin(5x)
*/
double f(double x)
{
    return sin(5 * x);
}

/*
    Function : matrix_generator
    Purpose  : Generates grid points, RHS vector and
               tridiagonal matrix coefficients
*/
void matrix_generator(vector<double> &a,
                      vector<double> &b,
                      vector<double> &c,
                      vector<double> &rhs,
                      vector<double> &x,
                      int N)
{
    // Generate grid points
    for (int j = 0; j <= N; j++)
        x[j] = 3.0 * j / N;

    // First RHS entry
    rhs[0] = (N / 3.0) * (-2.5 * f(x[0]) + 2 * f(x[1]) + 0.5 * f(x[2]));

    // Interior RHS entries
    for (int j = 1; j < N; j++)
        rhs[j] = N * (f(x[j + 1]) - f(x[j - 1]));

    // Last RHS entry
    rhs[N] = (N / 3.0) * (2.5 * f(x[N]) - 2 * f(x[N - 1]) - 0.5 * f(x[N - 2]));

    // Tridiagonal coefficients
    b[0] = 1;
    c[0] = 2;

    for (int i = 1; i < N; i++)
    {
        a[i] = 1;
        b[i] = 4;
        c[i] = 1;
    }

    a[N] = 2;
    b[N] = 1;
}

/*
    Function : thomas_algorithm
    Purpose  : Solves tridiagonal system
               using Thomas Algorithm
               a - lower diagonal
               b - main diagonal
               c - upper diagonal
*/
void thomas_algorithm(vector<double> &a,
                      vector<double> &b,
                      vector<double> &c,
                      vector<double> &rhs,
                      vector<double> &y,
                      int N)
{
    // Forward elimination
    for (int i = 1; i <= N; i++)
    {
        double m = a[i] / b[i - 1];

        b[i] -= m * c[i - 1];
        rhs[i] -= m * rhs[i - 1];
    }

    // Back substitution
    y[N] = rhs[N] / b[N];

    for (int i = N - 1; i >= 0; i--)
        y[i] = (rhs[i] - c[i] * y[i + 1]) / b[i];
}

/*
    Function : save_solution
    Purpose  : Saves (x_j , y_j) to a file
*/
void save_solution(vector<double> &x,
                   vector<double> &y,
                   int N)
{
    string filename = "solution_" + to_string(N) + ".dat";

    ofstream file(filename);

    for (int j = 0; j <= N; j++)
        file << x[j] << " " << y[j] << endl;

    file.close();

    cout << "Saved file: " << filename << endl;
}

/*
    Main Function
*/
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Nvalues[3] = {15, 25, 50};

    for (int k = 0; k < 3; k++)
    {
        int N = Nvalues[k];

        vector<double> a(N + 1, 0), b(N + 1, 0), c(N + 1, 0);
        vector<double> rhs(N + 1, 0), x(N + 1, 0), y(N + 1, 0);

        matrix_generator(a, b, c, rhs, x, N);

        thomas_algorithm(a, b, c, rhs, y, N);

        save_solution(x, y, N);
    }

    return 0;
}