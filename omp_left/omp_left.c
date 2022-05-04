#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

const int N = 5000;     //intervals
const double A = 0;     //start
const double B = 100;   //end

double f(double x);

int main(int argc, char** argv){
    double dx, xi, area, partial_area;
    double *points = (double *)calloc(sizeof(double)*N);
    dx = (B - A)/N;

    for(int i = 0; i < (int)(B - A); i += dx){
        points[i] = i * dx;
    }
    #pragma omp parallel shared(area) private(partial_area)
    {
        area = 0;
        partial_area = 0;

        #pragma omp for
        for(int i = 0; i < N - 1; i++){
            partial_area += f((points[i + 1] - points[i])/2);
        }
        printf("%f\n", partial_area);
        #pragma omp critical
        {
            area += partial_area;
            area *= dx;
        }

    }
    printf("Approximate area with %d intervals using midpoint rule is %f\n", N, area);
    return 0;
}

double f(double x) {
    return x*x;
}
