#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

const int N = 10;
const double A = 0;
const double B = 100;

double f(double x);
double trap(double l_endpt, double r_endpt, int trap_count, double base_len);

int main(int argc, char** argv){
    double dx, xi, area, partial_area;

    dx = (B - A)/N;

    #pragma omp parallel shared(area) private(partial_area, xi)
    {
        area = 0;
        partial_area = 0;

        #pragma omp for
        for(int i = 1; i < N; i++){
            xi = A + i*dx;
            partial_area += f(xi);
        }
        printf("%f\n",partial_area);
        #pragma omp critical
        {
            area += partial_area;
            area += (f(A) + f(B))/2.0;
            area *= dx;
        }

    }
    printf("Approximate area with %d trapezoids is %f\n", N, area);
    return 0;
}

double trap(double l_endpt, double r_endpt, int trap_count, double base_len) {
    double approx, xi;
    // (dx/2) * (f(x_0) + 2f(x_1) + 2f(x_2) + ... + 2f(x_{n-1}) + f(x_n))
    approx = (f(l_endpt) + f(r_endpt))/2.0;   //f(x_0) and f(x_n) term
    for(int i = 1; i < trap_count; i++) {   //used to be i <= trap_count - 1 in case breaks
        xi = l_endpt + i*base_len;          //x_i = a + i*dx
        approx += f(xi);
    }
    approx = approx*base_len;   //(dx/2) bit

    return approx;
}

double f(double x) {
    return x*x;
}
