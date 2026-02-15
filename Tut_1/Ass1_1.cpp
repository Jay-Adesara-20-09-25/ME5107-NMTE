// ME23B002 - Jay Adesara
// Program to demonstrate that matrix multiplication is NOT commutative

#include <bits/stdc++.h>
using namespace std;

// Function to multiply two square matrices and print the result
void multiplyMatrices(vector<vector<int>> &A,
                      vector<vector<int>> &B,
                      int n)
{
    // Result matrix initialized with 0
    vector<vector<int>> product(n, vector<int>(n, 0));

    // Matrix multiplication logic
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                product[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Print the resulting matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << product[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    int n;

    // Input matrix size
    cout << "Enter the size of the square matrices (n): ";
    cin >> n;

    vector<vector<int>> A(n, vector<int>(n));
    vector<vector<int>> B(n, vector<int>(n));

    // Input Matrix A
    cout << "\nEnter elements of Matrix A:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }

    // Input Matrix B
    cout << "\nEnter elements of Matrix B:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> B[i][j];
        }
    }

    // A × B
    cout << "\nMatrix A * B:\n";
    multiplyMatrices(A, B, n);

    // B × A
    cout << "\nMatrix B * A:\n";
    multiplyMatrices(B, A, n);

    // Conclusion
    cout << "\nAs we can see, matrix multiplication is not commutative.\n";

    return 0;
}
