// ME23B002 - Jay Adesara
// Performance comparison of:
// Case A: x (y^T z)
// Case B: (x y^T) z

#include <bits/stdc++.h>
using namespace std;

/*
Dimensions:
x  : m x n
y^T: n x m
z  : m x 1
*/

// Case A: x (y^T z)
void multiplication(vector<vector<int>> &x,
                    vector<vector<int>> &yt,
                    vector<vector<int>> &z,
                    int m, int n)
{
    // Step 1: Compute (y^T z) -> n x 1
    vector<vector<int>> temp(n, vector<int>(1, 0));

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < m; k++) {
            temp[i][0] += yt[i][k] * z[k][0];
        }
    }

    // Step 2: Compute x * temp -> m x 1
    vector<vector<int>> result(m, vector<int>(1, 0));

    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            result[i][0] += x[i][k] * temp[k][0];
        }
    }
}

// Case B: (x y^T) z
void multiply(vector<vector<int>> &x,
              vector<vector<int>> &yt,
              vector<vector<int>> &z,
              int m, int n)
{
    // Step 1: Compute (x y^T) -> m x m
    vector<vector<int>> temp(m, vector<int>(m, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < n; k++) {
                temp[i][j] += x[i][k] * yt[k][j];
            }
        }
    }

    // Step 2: Multiply with z -> m x 1
    vector<vector<int>> result(m, vector<int>(1, 0));

    for (int i = 0; i < m; i++) {
        for (int k = 0; k < m; k++) {
            result[i][0] += temp[i][k] * z[k][0];
        }
    }
}

// Matrix generator
void matrix_generator(vector<vector<int>> &x,
                      vector<vector<int>> &y,
                      vector<vector<int>> &yt,
                      vector<vector<int>> &z,
                      int m, int n)
{
    // Generate x and y (m x n)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            x[i][j] = (i + j) % 100;
            y[i][j] = (2 * (i + j)) % 100;
        }
        z[i][0] = (3 * i) % 100;   // vector z (m x 1)
    }

    // Compute transpose y^T (n x m)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            yt[i][j] = y[j][i];
        }
    }
}

int main()
{
    // (m, n) test cases
    pair<int, int> sizes[5] = {
        {256, 8}, {512, 8}, {1024, 8}, {2048, 8}, {4096, 8}
    };

    cout << "Performance Comparison\n";
    cout << "Case A: x (y^T z)\n";
    cout << "Case B: (x y^T) z\n";
    cout << "---------------------\n";

    for (int i = 0; i < 5; i++) {

        int m = sizes[i].first;
        int n = sizes[i].second;

        vector<vector<int>> x(m, vector<int>(n, 0));
        vector<vector<int>> y(m, vector<int>(n, 0));
        vector<vector<int>> yt(n, vector<int>(m, 0));
        vector<vector<int>> z(m, vector<int>(1, 0));

        matrix_generator(x, y, yt, z, m, n);

        // Case A timing
        clock_t start = clock();
        multiplication(x, yt, z, m, n);
        clock_t end = clock();

        cout << "m = " << m << ", n = " << n << endl;
        cout << "Case A time: "
             << double(end - start) / CLOCKS_PER_SEC
             << " seconds\n";

        // Case B timing
        start = clock();
        multiply(x, yt, z, m, n);
        end = clock();

        cout << "Case B time: "
             << double(end - start) / CLOCKS_PER_SEC
             << " seconds\n\n";
    }

    return 0;
}
