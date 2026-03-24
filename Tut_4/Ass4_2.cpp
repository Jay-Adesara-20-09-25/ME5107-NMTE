// ME23B002 - Jay Adesara
// Problem 2: System of Non-Linear Equations (Multivariate Newton's Method)

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Evaluates the function vector F(X)
void calculate_f(vector<double> &f, const vector<double> &x) {
    f[0] = x[0] + x[1] + x[2] - 3.0;
    f[1] = x[0] * x[0] + x[1] * x[1] + x[2] * x[2] - 5.0;
    f[2] = exp(x[0]) + x[0] * x[1] - x[0] * x[2] - 1.0; 
}

// Evaluates the Jacobian Matrix J(X)
void calculate_jacobian(vector<vector<double>> &J, const vector<double> &x) {
    J[0][0] = 1.0;                     J[0][1] = 1.0;        J[0][2] = 1.0;
    J[1][0] = 2.0 * x[0];              J[1][1] = 2.0 * x[1]; J[1][2] = 2.0 * x[2];
    J[2][0] = exp(x[0]) + x[1] - x[2]; J[2][1] = x[0];       J[2][2] = -x[0];
}

// Solves the linear system J * dx = -F using Gaussian Elimination with partial pivoting
bool solve_linear_system(vector<vector<double>> A, vector<double> b, vector<double>& dx) {
    int n = A.size();
    
    // Forward Elimination
    for(int i = 0; i < n; i++) {
        // Find pivot
        int pivot = i;
        for(int k = i + 1; k < n; k++) {
            if(abs(A[k][i]) > abs(A[pivot][i])) {
                pivot = k;
            }
        }
        
        // Check for singular matrix (using 1e-9 to catch the zero-row safely)
        if(abs(A[pivot][i]) < 1e-9) return false; 
        
        // Swap rows
        swap(A[i], A[pivot]);
        swap(b[i], b[pivot]);

        // Eliminate
        for(int k = i + 1; k < n; k++) {
            double factor = A[k][i] / A[i][i];
            for(int j = i; j < n; j++) {
                A[k][j] -= factor * A[i][j];
            }
            b[k] -= factor * b[i];
        }
    }
    
    // Back Substitution
    for(int i = n - 1; i >= 0; i--) {
        dx[i] = b[i];
        for(int j = i + 1; j < n; j++) {
            dx[i] -= A[i][j] * dx[j];
        }
        dx[i] /= A[i][i];
    }
    return true;
}

// Runs the Multivariate Newton-Raphson Method
void solve_system(vector<double> initial_guess, int guess_number) {
    vector<double> x = initial_guess;
    vector<vector<double>> J(3, vector<double>(3, 0.0));
    vector<double> f(3, 0.0);
    vector<double> dx(3, 0.0);
    
    double tol = 1e-6;      // Relaxed tolerance due to singular Jacobian at the root
    int max_iters = 30;     // Safeguard to prevent infinite loops
    
    cout << "--- Solving for Initial Guess " << guess_number << " : [" 
         << x[0] << ", " << x[1] << ", " << x[2] << "]^T ---\n";
    
    int iter = 0;
    while(iter < max_iters) {
        iter++;
        calculate_f(f, x);
        calculate_jacobian(J, x);
        
        // Setup the right hand side: -F
        vector<double> negF = {-f[0], -f[1], -f[2]};
        
        // Solve J * dx = -F
        if(!solve_linear_system(J, negF, dx)) {
            cout << "Iter " << iter << ": Jacobian became singular. Stopping exactly at the root.\n";
            break;
        }
        
        // Update variables: X_{n+1} = X_n + dx and calculate the error
        double error = 0.0;
        for(int i = 0; i < 3; i++) {
            x[i] += dx[i];
            error += dx[i] * dx[i];
        }
        error = sqrt(error);
        
        cout << "Iter " << iter << ": x = [" << fixed << setprecision(5) 
             << x[0] << ", " << x[1] << ", " << x[2] << "] (Error: " << error << ")\n";
             
        // Check for convergence
        if(error < tol) {
            break; 
        }
    }
    cout << "=> Converged to Root " << guess_number << " : [" 
         << fixed << setprecision(5) << x[0] << ", " << x[1] << ", " << x[2] << "]^T\n\n";
}

int main() {
    // Test both initial guesses specified in the assignment
    solve_system({0.1, 1.2, 2.5}, 1);
    solve_system({1.0, 0.0, 1.0}, 2);
    
    return 0;
}