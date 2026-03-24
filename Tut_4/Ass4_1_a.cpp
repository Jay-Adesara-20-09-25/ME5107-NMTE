// ME23B002 - Jay Adesara
// Problem 1(a): Bisection Method for calculating Heat Transfer Coefficient (h)

#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

// Calculates Gauss error function using the Trapezoidal rule (Eq. 7, N=51)
double erf_trapezoidal(double beta) {
    if (beta == 0.0) return 0.0; 
    
    int N = 51;                  
    double a = 0.0;              
    double b = beta;             
    double dx = (b - a) / (N - 1); 
    
    // Eq 7: f(x_1) + f(x_N)
    double sum = exp(-a * a) + exp(-b * b); 
    
    // Sum interior points: 2 * f(x_i)
    for (int i = 2; i <= N - 1; ++i) {
        double x_i = a + (i - 1) * dx;
        sum += 2.0 * exp(-x_i * x_i);
    }
    
    double pi = acos(-1.0);
    return (2.0 / sqrt(pi)) * (dx / 2.0) * sum;
}

// Complementary error function: erfc(x) = 1 - erf(x)
double erfc_custom(double beta) {
    return 1.0 - erf_trapezoidal(beta);
}

// Objective function f(h) evaluated at surface x = 0
double f(double h) {
    double Ti = 25.0, Tinf = 300.0, T_target = 53.5;
    double t = 400.0, k = 400.0, alpha = 1e-4;
    
    double LHS = (T_target - Ti) / (Tinf - Ti);
    double beta = (h * sqrt(alpha * t)) / k; 
    
    return 1.0 - exp(beta * beta) * erfc_custom(beta) - LHS;
}

int main() {
    double h_low = 1.0, h_high = 10000.0;
    double epsilon = 1e-5;       
    
    vector<double> h_history;
    double f_low = f(h_low); 

    // Iteration 1
    double h_mid = h_low + (h_high - h_low) / 2.0;
    h_history.push_back(h_mid);

    // Bisection Loop
    while (true) {
        double f_mid = f(h_mid);
        
        // Update interval brackets
        if (f_low * f_mid < 0) { 
            h_high = h_mid; 
        } else { 
            h_low = h_mid;  
            f_low = f_mid;  
        }
        
        // Next iteration midpoint
        double h_next = h_low + (h_high - h_low) / 2.0;
        h_history.push_back(h_next);
        
        // Stopping condition: |x(n+1) - x(n)| < epsilon
        if (abs(h_next - h_mid) < epsilon) break;
        h_mid = h_next; 
    }

    double h_exact = h_history.back(); 
    int total_iters = h_history.size();
    
    ofstream out("q1a_data.csv");
    out << "Iteration,h_value,Error,Convergence_Rate\n";
    
    for (size_t k = 0; k < h_history.size() - 1; ++k) {
        double e_k = abs(h_history[k] - h_exact);       
        double e_k1 = abs(h_history[k+1] - h_exact);    
        double rate = (e_k > 0) ? (e_k1 / e_k) : 0; // Rate = e_{k+1}/e_k
        out << (k+1) << "," << h_history[k] << "," << e_k << "," << rate << "\n";
    }
    out.close();
    
    cout << "--- Q1(a): Bisection Method ---\n";
    cout << "Converged in: " << total_iters << " iterations\n";
    cout << "Calculated h: " << fixed << setprecision(5) << h_exact << " W/m^2.K\n\n";
    return 0;
}