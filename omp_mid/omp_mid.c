#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) a < b ? a : b //macros kinda bad, careful

double f(double x);

int main(int argc, char** argv){
    int intervals, threads;
    double dx, area, partial_area, a, b;
    double *points;

    if(argc == 4){
        intervals = atoi(argv[1]);
        a = atof(argv[2]);
        b = atof(argv[3]);
    }
    else{
        printf("Error: too few arguments\nusage: %s <number of intervals> <from (a)> <to (b)>\n", argv[0]);
        return -1;
    }
    threads = MIN(intervals, omp_get_max_threads());

    dx = (b - a)/intervals;
    points = calloc(intervals + 1, sizeof(double));

    for(int i = 0; i < intervals + 1; i++){
        points[i] = i * dx;
    }
    #pragma omp parallel shared(area) private(partial_area)
    {
        area = 0;
        partial_area = 0;

        #pragma omp for
        for(int i = 0; i < intervals; i++){
            partial_area += f(((points[i + 1] - points[i])/2) + points[i]);
        }
        printf("Partial area: %13f\n", partial_area);
        #pragma omp critical
        {
            area += partial_area;
        }
    }
    area *= dx;
    printf("\nThe approximate definite integral from %.0f to %.0f with\n%d intervals, using midpoint rule, is %f\n", a, b, intervals, area);

    free(points);
    return 0;
}

double f(double x) {
    return sqrt(x);
}
