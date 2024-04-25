// "one" adaptive exponential integrate-and-fire(AEIF) neuron
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "aeif_1neuron.h"

#define TMAX 1000.0 // time max [ms]
#define DT 0.001 // time difference [ms]

static inline double dvdt(double v_memb, double w_current, struct one_Neuron neuron){
    return ( -neuron.gl_cond * (v_memb - neuron.el_rest) + neuron.gl_cond * neuron.slopefactor * exp( (v_memb - neuron.vt_theta) / neuron.slopefactor ) + neuron.i_ext - w_current ) / neuron.c_capa;
}

static inline double dwdt(double v_memb, double w_current, struct one_Neuron neuron){
    return (neuron.a_cond * (v_memb - neuron.el_rest) - w_current) / neuron.tauw_cons;
}


int main(){

    struct one_Neuron neuron = fig4a; // changable
    double v_memb, w_current;
    v_memb = neuron.el_rest;
    w_current = 0.0;

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
        
        if(false){
             // runge-kutta method
            double dvdt1 = dvdt(v_memb, w_current, neuron);
            double dwdt1 = dwdt(v_memb, w_current, neuron);
            double dvdt2 = dvdt(v_memb + 0.5 * DT * dvdt1, w_current + 0.5 * DT * dwdt1, neuron);
            double dwdt2 = dwdt(v_memb + 0.5 * DT * dvdt1, w_current + 0.5 * DT * dwdt1, neuron);
            double dvdt3 = dvdt(v_memb + 0.5 * DT * dvdt2, w_current + 0.5 * DT * dwdt2, neuron);
            double dwdt3 = dwdt(v_memb + 0.5 * DT * dvdt2, w_current + 0.5 * DT * dwdt2, neuron);
            double dvdt4 = dvdt(v_memb + DT * dvdt3, w_current + DT * dwdt3, neuron);
            double dwdt4 = dwdt(v_memb + DT * dvdt3, w_current + DT * dwdt3, neuron);
            v_memb += DT * (dvdt1 + 2 * dvdt2 + 2 * dvdt3 + dvdt4) / 6.0;
            w_current += DT * (dwdt1 + 2 * dwdt2 + 2 * dwdt3 + dwdt4) / 6.0;
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
