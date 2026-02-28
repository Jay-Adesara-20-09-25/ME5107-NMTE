// ME23B002 - Jay Adesara
// Gaussian Elimination with Partial Pivoting
// 8-node Heat Conduction System

#include <bits/stdc++.h>
using namespace std;

/*
    Function : matrix_generator
    Purpose  : Initializes coefficient matrix A and RHS vector b
               based on finite-difference energy balance equations.
*/
void matrix_generator(vector<vector<double>> &A, vector<vector<double>> &b)
{
    A = {
        {-4, 1, 1, 0, 0, 0, 0, 0},
        {2, -4, 0, 1, 0, 0, 0, 0},
        {1, 0, -4, 1, 1, 0, 0, 0},
        {0, 1, 2, -4, 0, 1, 0, 0},
        {0, 0, 1, 0, -4, 1, 1, 0},
        {0, 0, 0, 1, 2, -4, 0, 1},
        {0, 0, 0, 0, 2, 0, -9, 1},
        {0, 0, 0, 0, 0, 2, 2, -9}
    };

    b = {
        {-1000},
        {-500},
        {-500},
        {0},
        {-500},
        {0},
        {-2000},
        {-1500}
    };
}

/*
    Function : forward_elimination
    Purpose  : Converts matrix A into upper triangular form
               using Gaussian elimination with partial pivoting.
*/
void forward_elimination(vector<vector<double>> &A,
                         vector<vector<double>> &b,
                         int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        // -----------------------------
        // Partial Pivoting
        // -----------------------------
        int pivot_index = i;
        double max_value = fabs(A[i][i]);

        for (int t = i + 1; t < n; t++)
        {
            if (fabs(A[t][i]) > max_value)
            {
                max_value = fabs(A[t][i]);
                pivot_index = t;
            }
        }

        // Swap rows if necessary
        if (pivot_index != i)
        {
            swap(A[i], A[pivot_index]);
            swap(b[i][0], b[pivot_index][0]);
        }

        // Check for zero or near-zero pivot
        if (fabs(A[i][i]) < 1e-12)
        {
            cout << "Matrix is singular or nearly singular.\n";
            return;
        }

        // -----------------------------
        // Elimination
        // -----------------------------
        for (int j = i + 1; j < n; j++)
        {
            double factor = A[j][i] / A[i][i];
            A[j][i] = 0;

            for (int k = i + 1; k < n; k++)
            {
                A[j][k] -= factor * A[i][k];
            }

            b[j][0] -= factor * b[i][0];
        }
    }
}

/*
    Function : backward_substitution
    Purpose  : Solves upper triangular system and prints temperatures.
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

        for (int j = i + 1; j < n; j++)
        {
            sum -= A[i][j] * x[j][0];
        }

        x[i][0] = sum / A[i][i];
    }

    // Print results
    cout << "\nTemperatures (T1 to T8):\n";
    for (int i = 0; i < n; i++)
    {
        cout << "T" << i + 1 << " : " << x[i][0] << " K\n";
    }
}

/*
    Main Function
*/
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 8;

    vector<vector<double>> A(n, vector<double>(n));
    vector<vector<double>> b(n, vector<double>(1));
    vector<vector<double>> x(n, vector<double>(1, 0));

    matrix_generator(A, b);
    forward_elimination(A, b, n);
    backward_substitution(A, b, x, n);

    return 0;
}