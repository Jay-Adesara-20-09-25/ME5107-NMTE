// ME23B002 - Jay Adesara

#include<bits/stdc++.h>
#include<chrono>

using namespace std;

#define double long double


/*
    Function : inverse
    Purpose  : Computes inverse of matrix A using
               Gauss-Jordan elimination with pivoting
    Input    : Square matrix A
    Output   : A becomes A^{-1}
    Complexity : O(n^3)
*/
void inverse(vector<vector<double>> &A)
{
    int n = A.size();

    // Augment matrix with identity matrix
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            A[i].push_back(i==j ? 1.0 : 0.0);

    // Perform Gauss-Jordan elimination
    for(int i=0;i<n;i++)
    {
        int pivot=i;

        // Partial pivoting
        for(int j=i+1;j<n;j++)
            if(fabs(A[j][i])>fabs(A[pivot][i]))
                pivot=j;

        swap(A[i],A[pivot]);

        // Normalize pivot row
        double div=A[i][i];

        for(int j=i;j<2*n;j++)
            A[i][j]/=div;

        // Eliminate other rows
        for(int j=0;j<n;j++)
        {
            if(j!=i)
            {
                double factor=A[j][i];

                for(int k=i;k<2*n;k++)
                    A[j][k]-=factor*A[i][k];
            }
        }
    }

    // Remove original matrix part and keep inverse
    for(int i=0;i<n;i++)
        A[i].erase(A[i].begin(),A[i].begin()+n);
}


/*
    Function : matrix_vector_multiply
    Purpose  : Computes multiplication of matrix A
               with vector x
               result = A*x
*/
vector<double> matrix_vector_multiply(vector<vector<double>> &A,
                                       vector<double> &x)
{
    int n=A.size();

    vector<double> res(n,0);

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            res[i]+=A[i][j]*x[j];

    return res;
}


/*
    Function : matrix_subtract
    Purpose  : Computes A = A - B
*/
void matrix_subtract(vector<vector<double>> &A,
                     vector<vector<double>> &B)
{
    int n=A.size();

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            A[i][j]-=B[i][j];
}


/*
    Function : vector_subtract
    Purpose  : Computes vector subtraction
               result = a - b
*/
vector<double> vector_subtract(vector<double> &a,
                               vector<double> &b)
{
    int n=a.size();

    vector<double> res(n);

    for(int i=0;i<n;i++)
        res[i]=a[i]-b[i];

    return res;
}


/*
    Main Program
    Solves the block tridiagonal system described
    in Programming Q2 of the assignment.

    Matrix structure :

        | D1  I   0  ... |
        | I   D2  I  ... |
        | 0   I   D3 ... |
        | ...          DN|

    where each Di is a tridiagonal block (5×5).
*/
int main()
{
    // Values of N required in the assignment
    vector<int> Nvalues={10,20,30};

    int Nblk=5;

    // Files for storing data used in plotting
    ofstream solFile("solution.dat");
    ofstream timeFile("time.dat");

    for(int N:Nvalues)
    {

        // Start timing
        auto start=chrono::high_resolution_clock::now();

        /*
            Step 1 : Construct block matrices Di
            Each Di is a tridiagonal matrix
        */
        vector<vector<vector<double>>> D(
        N,vector<vector<double>>(Nblk,vector<double>(Nblk,0)));

        for(int i=0;i<N;i++)
        {
            for(int j=0;j<Nblk;j++)
            {
                if(j>0) D[i][j][j-1]=1;

                D[i][j][j]=-4;

                if(j<Nblk-1) D[i][j][j+1]=1;
            }
        }


        /*
            Step 2 : Construct RHS vectors fi
            fi =
            [1 , 1/Nblk , ... , 1/Nblk , 2]
        */
        vector<vector<double>> f(N,vector<double>(Nblk,1.0/Nblk));

        for(int i=0;i<N;i++)
        {
            f[i][0]=1;
            f[i][Nblk-1]=2;
        }


        /*
            Step 3 : Forward Block Elimination

            Convert system into upper block triangular form

            D_i^{-1} f_i
            D_{i+1} = D_{i+1} − D_i^{-1}
            f_{i+1} = f_{i+1} − D_i^{-1} f_i
        */
        for(int i=0;i<N-1;i++)
        {
            inverse(D[i]);

            f[i]=matrix_vector_multiply(D[i],f[i]);

            matrix_subtract(D[i+1],D[i]);

            f[i+1]=vector_subtract(f[i+1],f[i]);
        }


        /*
            Step 4 : Back Substitution
            Solve for solution blocks xi
        */
        vector<vector<double>> x(N,vector<double>(Nblk,0));

        inverse(D[N-1]);

        x[N-1]=matrix_vector_multiply(D[N-1],f[N-1]);

        for(int i=N-2;i>=0;i--)
        {
            vector<double> temp=
            matrix_vector_multiply(D[i],x[i+1]);

            x[i]=vector_subtract(f[i],temp);
        }


        // Stop timing
        auto end=chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed=end-start;


        /*
            Step 5 : Store data for plotting

            Assignment requires plotting
            x_{i,j} vs j where i = N/2
        */
        int mid=N/2;

        for(int j=0;j<Nblk;j++)
            solFile<<N<<" "<<j+1<<" "<<x[mid][j]<<endl;


        /*
            Step 6 : Store runtime for
            Time vs N plot
        */
        timeFile<<N<<" "<<elapsed.count()<<endl;


        // Print runtime in terminal
        cout<<"N = "<<N<<"  Time = "<<elapsed.count()<<endl;

    }

    solFile.close();
    timeFile.close();

    return 0;
}