// ME23B002 - Jay Adesara

#include <bits/stdc++.h>
using namespace std;

void matrix_generator(vector<vector<double>> &A, vector<vector<double>>& b, int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            A[i][j] = max(i+1, j+1);
        }
        b[i][0] = 1;
    }
}

void forward_elimination(vector<vector<double>> &A, vector<vector<double>>& b, int n){
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            double temp = A[j][i]/A[i][i];
            for(int k=i+1;k<n;k++){
                A[j][k] -= temp * A[i][k];
            }
            b[j][0] -= temp * b[i][0];
        }
    }
}

void backward_substitution(vector<vector<double>> &A, vector<vector<double>>& b, vector<vector<double>>& x, int n){
    
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int nvector[4] = {32, 128, 512, 1024};

    for(int i=0;i<4;i++){
        int n = nvector[i];

        vector<vector<double>> A(n, vector<double>(n, 0));
        vector<vector<double>> b(n, vector<double>(1, 0));
        vector<vector<double>> x(n, vector<double>(1, 0));

        matrix_generator(A, b, n);

        forward_elimination(A, b, n);
        backward_substitution(A, b, x, n);

    }

   return 0;
}