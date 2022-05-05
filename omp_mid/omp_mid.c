#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) a < b ? a : b

const double A = 0;     //start
const double B = 100;   //end

double f(double x);

int main(int argc, char** argv){
    int intervals, threads;
    double dx, area, partial_area;
    double *points;

    if(argc == 2){
        intervals = atoi(argv[1]);
    }
    else{
        printf("Error: too few arguments\nusage: %s <number of intervals>\n", argv[0]);
        return -1;
    }
    // threads = MIN(intervals, omp_get_max_threads());

    dx = (B - A)/intervals;
    points = calloc(intervals + 1, sizeof(double));

    for(int i = 0; i < intervals + 1; i++){
        points[i] = i * dx;
        printf("point %2d: %.2f\n", i, points[i]);
    }
    //#pragma omp parallel shared(area) private(partial_area)
    //{
        area = 0;
        partial_area = 0;

        //#pragma omp for
        for(int i = 0; i < intervals; i++){
            printf("f(%.4f - %.4f / 2) = f(%.4f) = %.4f", points[i + 1], points[i], (points[i + 1] - points[i])/2, f((points[i + 1] - points[i])/2));
            partial_area += f((points[i + 1] - points[i])/2);
        }
        printf("%f\n", partial_area);
        //#pragma omp critical
        //{
            area += partial_area;
        //}
    //}
    area *= dx;
    printf("Approximate area with %d intervals using midpoint rule is %f\n", intervals, area);

    free(points);
    return 0;
}

double f(double x) {
    return x*x;
}
