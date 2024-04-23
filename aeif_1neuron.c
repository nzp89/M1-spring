// "one" adaptive exponential integrate-and-fire(AEIF) neuron
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "aeif_1neuron.h"

#define TMAX 500.0 // time max [ms]
#define DT 0.0001 // time difference [ms]

double dvdt(double v_memb, double w_current, struct one_Neuron neuron){
    // fprintf( stderr, "%f\n", exp( (v_memb - neuron.vt_theta)));

    double exponent_term = (v_memb - neuron.vt_theta) / neuron.slopefactor;
    if (exponent_term > 15) {
        exponent_term = 15;  
    }

    return ( -neuron.gl_cond * (v_memb - neuron.el_rest) + neuron.gl_cond * neuron.slopefactor * exp( exponent_term ) + neuron.i_ext - w_current ) / neuron.c_capa;
}

double dwdt(double v_memb, double w_current, struct one_Neuron neuron){
    return (neuron.a_cond * (v_memb - neuron.el_rest) - w_current) / neuron.tauw_cons;
}


int main(){

    struct one_Neuron neuron = fig4a; // changable
    double v_memb, w_current;
    v_memb = neuron.el_rest;
    w_current = 0.0;
// file setup
    FILE *v_file;
    char *v_filename = "output_txt/fig4a_voltage.txt";
    v_file = fopen(v_filename, "w");
    if(v_file == NULL){
        printf("cannot open the file : %s\n", v_filename);
    }

    int countmax = (int) TMAX / DT;
 
    for(int time = 0; time < countmax; time++){
        if(time % 100 == 0){
            printf("time count : %d\n", time);
            fprintf(v_file, "%f %f\n", time * DT, v_memb);
        }
        
        if(true){
        // runge-kutta method
            double kv1 = DT * dvdt(v_memb, w_current, neuron);
            double kw1 = DT * dwdt(v_memb, w_current, neuron);

            double kv2 = DT * dvdt(v_memb + 0.5 * kv1, w_current + 0.5 * kw1, neuron);
            double kw2 = DT * dwdt(v_memb + 0.5 * kv1, w_current + 0.5 * kw1, neuron);

            double kv3 = DT * dvdt(v_memb + 0.5 * kv2, w_current + 0.5 * kw2, neuron);
            double kw3 = DT * dwdt(v_memb + 0.5 * kv2, w_current + 0.5 * kw2, neuron);

            double kv4 = DT * dvdt(v_memb + kv3, w_current + kw3, neuron);
            double kw4 = DT * dwdt(v_memb + kv3, w_current + kw3, neuron);

            v_memb += (kv1 + 2 * kv2 + 2 * kv3 + kv4) / 6.0;
            w_current += (kw1 + 2 * kw2 + 2 * kw3 + kw4) / 6.0;
        }else{
        // euler method
            v_memb += DT * dvdt(v_memb, w_current, neuron);
            w_current += DT * dwdt(v_memb, w_current, neuron);  
        }
        
        if(v_memb > 0){
            fprintf(v_file, "%f %f\n", time * DT, 0.0);
            v_memb = neuron.vr_reset;
            w_current = w_current + neuron.b_reset;
        }
        
    }

    fclose(v_file);
    return 0;
}
