#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "blackhole_lab.h"

void ray_trace(const int number_of_points, double *value_storage, bool *completed, bool *changed)
{
    #define C_RK_ORDER 4
    const double RK_factor1[] = {1. / 2.                           };
    const double RK_factor2[] = {     0., 1. / 2.                  };
    const double RK_factor3[] = {     0.,      0.,      1.         };
    const double RK_factor4[] = {1. / 6., 1. / 3., 1. / 3., 1. / 6.};
    const double * const RK_factor[C_RK_ORDER] = {RK_factor1, RK_factor2, RK_factor3, RK_factor4};
    
    // time integration
    for (int i = 0; i < number_of_points; i++) {
        double *value = value_storage + i * C_NUMBER_OF_FUNCTIONS;
        if (!completed[i]) {
            completed[i] = BOOL_STOP_CONDITION(value);
        }
        if (!completed[i]) {
            double value_temp[C_NUMBER_OF_FUNCTIONS];
            double derivative[C_RK_ORDER][C_NUMBER_OF_FUNCTIONS];
            for (int k = 0; k <= C_RK_ORDER; k++) {
                for (int j = 0; j < C_NUMBER_OF_FUNCTIONS; j++) {
                    value_temp[j] = value[j];
                }
                for (int l = 0; l < k; l++) {
                    for (int j = 0; j < C_NUMBER_OF_FUNCTIONS; j++) {
                        value_temp[j] += RK_factor[k - 1][l] * derivative[l][j] * C_DELTA_TIME;
                    }
                }
                if (C_RK_ORDER == k) {
                    for (int j = 0 ;j < C_NUMBER_OF_FUNCTIONS; j++) {
                        value[j] = value_temp[j];
                    }
                } else {
                    derivative[k][X_r    ] = DERIVATIVE_X_r    (value_temp);
                    derivative[k][X_theta] = DERIVATIVE_X_theta(value_temp);
                    derivative[k][X_phi  ] = DERIVATIVE_X_phi  (value_temp);
                    derivative[k][U_t    ] = DERIVATIVE_U_t    (value_temp);
                    derivative[k][U_r    ] = DERIVATIVE_U_r    (value_temp);
                    derivative[k][U_theta] = DERIVATIVE_U_theta(value_temp);
                    derivative[k][U_phi  ] = DERIVATIVE_U_phi  (value_temp);
                }
            }
            *changed = true;
        }
    }
}

void run(int w_start, int w_end, int h_start, int h_end, int *status, double *y, double *z)
{
    int w_interval = w_end - w_start + 1;
    int h_interval = h_end - h_start + 1;
    int number_of_points = w_interval * h_interval;
    
    double *value_storage = malloc(sizeof(double) * number_of_points * C_NUMBER_OF_FUNCTIONS);
    bool   *completed     = malloc(sizeof(bool  ) * number_of_points);
    
    for (int i = 0; i < number_of_points; i++) {
        double *value = value_storage + i * C_NUMBER_OF_FUNCTIONS;
        int h = h_start + i / w_interval;
        int w = w_start + i % w_interval;
        // set initial value
        value[X_r    ] = INITIAL_X_r    (w, h, value);
        value[X_theta] = INITIAL_X_theta(w, h, value);
        value[X_phi  ] = INITIAL_X_phi  (w, h, value);
        value[U_r    ] = INITIAL_U_r    (w, h, value);
        value[U_theta] = INITIAL_U_theta(w, h, value);
        value[U_phi  ] = INITIAL_U_phi  (w, h, value);
        value[U_t    ] = INITIAL_U_t    (w, h, value);
        
        completed[i] = false;
    }
    
    for (int k = 0; k < C_TOTAL_STEP; k++) {
        bool changed = false;
        ray_trace(number_of_points, value_storage, completed, &changed);
        if (!changed) break;
    }
    
    for (int i = 0; i < number_of_points; i++) {
        double *value = value_storage + i * C_NUMBER_OF_FUNCTIONS;
        int index = (h_start + i / w_interval) * C_RESOLUTION_WIDTH + (w_start + i % w_interval);
        // information return
        if (BOOL_CROSS_PICTURE(value)) {
            status[index] = HIT;
            y[index] = Y(value) / C_l;
            z[index] = Z(value) / C_l;
        } else if (BOOL_NEAR_HORIZON(value)) {
            status[index] = FALL;
        } else if (BOOL_OUTSIDE_BOUNDARY(value)) {
            status[index] = OUTSIDE;
        } else {
            status[index] = YET;
        }
    }
    
    free(value_storage);
}

void export(int *status, double *y, double *z)
{
    FILE *data_fp = fopen(C_OUTPUT_FILENAME, "wb");
    
    int resolution[2] = {C_RESOLUTION_WIDTH, C_RESOLUTION_HEIGHT};
    fwrite(resolution, sizeof(int), 2, data_fp);
    
    fwrite(status, sizeof(int   ), C_RESOLUTION_TOTAL_PIXELS, data_fp);
    fwrite(     y, sizeof(double), C_RESOLUTION_TOTAL_PIXELS, data_fp);
    fwrite(     z, sizeof(double), C_RESOLUTION_TOTAL_PIXELS, data_fp);
    
    fclose(data_fp);
}

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1.e-6;
}

int main()
{
    int *status = malloc(sizeof(int   ) * C_RESOLUTION_TOTAL_PIXELS);
    double   *y = malloc(sizeof(double) * C_RESOLUTION_TOTAL_PIXELS);
    double   *z = malloc(sizeof(double) * C_RESOLUTION_TOTAL_PIXELS);

    int w_start = 0;
    int w_end   = C_RESOLUTION_WIDTH  - 1;
    int h_start = 0;
    int h_end   = C_RESOLUTION_HEIGHT - 1;
    
    double start = get_time();
    run(w_start, w_end, h_start, h_end, status, y, z);
    printf("Elapsed time: %fs\n", get_time() - start);

    export(status, y, z);
    
    free(status);
    free(y);
    free(z);
    
    return 0;
}
