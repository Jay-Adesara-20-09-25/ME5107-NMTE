// ME23B002 - Jay Adesara
// Problem 1(b): Newton's Method for calculating Heat Transfer Coefficient (h)

#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

double erf_trapezoidal(double beta) {
    if (beta == 0.0) return 0.0;
    int N = 51;
    double a = 0.0, b = beta;
    double dx = (b - a) / (N - 1);
    
    double sum = exp(-a * a) + exp(-b * b);
    for (int i = 2; i <= N - 1; ++i) {
        double x_i = a + (i - 1) * dx;
        sum += 2.0 * exp(-x_i * x_i);
    }
    double pi = acos(-1.0);
    return (2.0 / sqrt(pi)) * (dx / 2.0) * sum;
}

double erfc_custom(double beta) {
    return 1.0 - erf_trapezoidal(beta);
}

// Function to calculate both f(h) and its analytical derivative f'(h)
void calculate_f_and_df(double h, double &f_val, double &df_val) {
    double Ti = 25.0, Tinf = 300.0, T_target = 53.5;
    double t = 400.0, k = 400.0, alpha = 1e-4;
    
    double LHS = (T_target - Ti) / (Tinf - Ti);
    double c = sqrt(alpha * t) / k; 
    double beta = c * h;
    
    double shared_term = exp(beta * beta) * erfc_custom(beta);
    
    f_val = 1.0 - shared_term - LHS;
    
    double pi = acos(-1.0);
    df_val = c * ((2.0 / sqrt(pi)) - 2.0 * beta * shared_term);
}

int main() {
    double h_current = -15000.0; // Given initial guess
    double epsilon = 1e-5;
    
    vector<double> h_history;
    h_history.push_back(h_current); // Iteration 0 (Initial Guess)

    while (true) {
        double f_val, df_val;
        calculate_f_and_df(h_current, f_val, df_val);
        
        // Newton-Raphson Step
        double h_next = h_current - (f_val / df_val);
        h_history.push_back(h_next);
        
        // Stopping condition
        if (abs(h_next - h_current) < epsilon) break;
        h_current = h_next; 
    }

    double h_exact = h_history.back(); 
    int total_iters = h_history.size() - 1; // Subtract 1 because index 0 is the initial guess
    
    ofstream out("q1b_data.csv");
    out << "Iteration,h_value,Error,Convergence_Rate\n";
    
    for (size_t k = 0; k < h_history.size() - 1; ++k) {
        double e_k = abs(h_history[k] - h_exact);
        double e_k1 = abs(h_history[k+1] - h_exact);
        double rate = abs(e_k1 / (e_k * e_k)); // Rate = e_{k+1}/e_k^2
        out << (k) << "," << h_history[k] << "," << e_k << "," << rate << "\n";
    }
    out.close();
    
    cout << "--- Q1(b): Newton's Method ---\n";
    cout << "Converged in: " << total_iters << " iterations\n";
    cout << "Calculated h: " << fixed << setprecision(5) << h_exact << " W/m^2.K\n\n";
    return 0;
}