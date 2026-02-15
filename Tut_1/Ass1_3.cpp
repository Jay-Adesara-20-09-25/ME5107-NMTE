// ME23B002 - Jay Adesara
// Program to measure time taken for matrix-vector multiplication
// for different matrix sizes

#include <bits/stdc++.h>
using namespace std;

// Function to perform matrix-vector multiplication
// A is an n x n matrix
// B is an n x 1 vector
void multiplication(vector<vector<int>> &A,
                    vector<vector<int>> &B,
                    int n)
{
    // Result vector of size n x 1
    vector<vector<int>> result(n, vector<int>(1, 0));

    // Matrix-vector multiplication
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 1; j++) {
            for (int k = 0; k < n; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to generate matrix A and vector B
void matrix_generator(vector<vector<int>> &A,
                      vector<vector<int>> &B,
                      int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            // Filling matrix A
            A[i][j] = (i + j) % 1024;

            // Filling vector B (only one column)
            if (j < 1) {
                B[i][j] = (2 * (i + j)) % 1024;
            }
        }
    }
}

int main()
{
    // Different matrix sizes to test performance
    int nvector[5] = {256, 512, 1024, 2048, 4096};

    cout << "Matrix-Vector Multiplication Timing Results\n";
    cout << "------------------------------------------\n";

    for (int i = 0; i < 5; i++) {

        int n = nvector[i];

        // Creating matrix A (n x n) and vector B (n x 1)
        vector<vector<int>> A(n, vector<int>(n, 0));
        vector<vector<int>> B(n, vector<int>(1, 0));

        // Initialize matrix and vector
        matrix_generator(A, B, n);

        // Start clock
        clock_t start_time = clock();

        // Perform multiplication
        multiplication(A, B, n);

        // Stop clock
        clock_t end_time = clock();

        // Calculate time taken
        double time_taken = double(end_time - start_time) / CLOCKS_PER_SEC;

        // Display results
        cout << "Matrix size: " << n << " x " << n << endl;
        cout << "Time taken : " << time_taken << " seconds\n\n";
    }

    return 0;
}
