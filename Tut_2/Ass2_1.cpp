// ME23B002 - Jay Adesara

#include <bits/stdc++.h>
using namespace std;

/*
    Function: matrix_generator
    Purpose :
        Generates matrix A and RHS vector b.
        Currently fills A[i][j] = max(i+1, j+1)
        and b = 1 for all entries.
*/
void matrix_generator(vector<vector<double>> &A, vector<vector<double>> &b, int n)
{
    // Fill matrix A
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = max(i + 1, j + 1); // example matrix definition
        }
        b[i][0] = 1; // RHS vector
    }
}

/*
    Function: forward_elimination
    Purpose :
        Converts matrix A into upper triangular form
        using Gaussian Elimination.
*/
void forward_elimination(vector<vector<double>> &A, vector<vector<double>> &b, int n)
{
    // Loop over pivot rows
    for (int i = 0; i < n - 1; i++)
    {
        // Eliminate entries below pivot
        for (int j = i + 1; j < n; j++)
        {
            double temp = A[j][i] / A[i][i]; // elimination factor
            A[j][i] = 0;                     // explicitly set lower element to zero

            // Update remaining row elements
            for (int k = i + 1; k < n; k++)
            {
                A[j][k] -= temp * A[i][k];
            }

            // Update RHS vector
            b[j][0] -= temp * b[i][0];
        }
    }
}

/*
    Function: backward_substitution
    Purpose :
        Solves upper triangular system after elimination
        and computes 1/(sum of squares of solution vector).
*/
void backward_substitution(vector<vector<double>> &A,
                           vector<vector<double>> &b,
                           vector<vector<double>> &x,
                           int n)
{
    // Solve last variable
    x[n - 1][0] = b[n - 1][0] / A[n - 1][n - 1];

    // Solve remaining variables from bottom to top
    for (int i = n - 2; i >= 0; i--)
    {
        double sum = b[i][0];

        // Subtract known terms
        for (int j = i + 1; j < n; j++)
        {
            sum -= A[i][j] * x[j][0];
        }

        // Divide by diagonal element
        x[i][0] = sum / A[i][i];
    }

    // Compute 1 / (summation of x_i^2)
    double sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        cout << x[i][0] << " "; // print solution vector
        sum += x[i][0] * x[i][0];
    }

    sum = 1.0 / sum;

    // Print result
    cout << endl
         << "n = " << n
         << " 1/(summation of x_i^2) = " << sum << endl;
}

/*
    Main Function:
        Runs Gaussian elimination for different matrix sizes.
*/
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    // Different problem sizes
    int nvector[4] = {32, 128, 512, 1024};

    // Loop over all sizes
    for (int i = 0; i < 4; i++)
    {
        int n = nvector[i];

        // Allocate matrices and vectors
        vector<vector<double>> A(n, vector<double>(n, 0));
        vector<vector<double>> b(n, vector<double>(1, 0));
        vector<vector<double>> x(n, vector<double>(1, 0));

        // Generate system
        matrix_generator(A, b, n);

        // Perform Gaussian elimination
        forward_elimination(A, b, n);

        // Solve using backward substitution
        backward_substitution(A, b, x, n);
    }

    return 0;
}
