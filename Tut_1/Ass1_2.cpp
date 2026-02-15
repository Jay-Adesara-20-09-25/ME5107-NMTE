// ME23B002 - Jay Adesara
// Program to show that (A + A^T) is a symmetric matrix

#include <bits/stdc++.h>
using namespace std;

// Function to compute (A + A^T) and verify symmetry
void check(vector<vector<int>> &A, int n)
{
    // Matrix to store transpose of A
    vector<vector<int>> AT(n, vector<int>(n, 0));

    // Finding transpose of A
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            AT[i][j] = A[j][i];
        }
    }

    // Matrix to store A + A^T
    vector<vector<int>> M(n, vector<int>(n, 0));

    // Adding A and its transpose
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M[i][j] = A[i][j] + AT[i][j];
        }
    }

    // Printing (A + A^T)
    cout << "\nMatrix (A + A^T):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }

    // Printing transpose of (A + A^T)
    cout << "\nTranspose of (A + A^T):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << M[j][i] << " ";
        }
        cout << endl;
    }
}

int main()
{
    int n;

    // Input matrix size
    cout << "Enter the size of the square matrix (n): ";
    cin >> n;

    vector<vector<int>> A(n, vector<int>(n));

    // Input matrix A
    cout << "\nEnter elements of Matrix A:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }

    // Check symmetry of (A + A^T)
    check(A, n);

    // Conclusion
    cout << "\nAs we can see, (A + A^T) is symmetric.\n";

    return 0;
}
