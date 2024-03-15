// "one" adaptive exponential integrate-and-fire(AEIF) neuron
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "aeif_parameter.h"

#define TMAX 1000.0 // time max [ms]
#define DT 0.001 // time difference [ms]

static inline double dvdt(double v_memb, double w_current, struct one_Neuron neurontype){
    return ( -neurontype.gl_cond * (v_memb - neurontype.el_rest) + neurontype.gl_cond * neurontype.slopefactor * exp( (v_memb - neurontype.vt_theta) / neurontype.slopefactor ) + neurontype.i_ext - w_current ) / neurontype.c_capa;
}

static inline double dwdt(double v_memb, double w_current, struct one_Neuron neurontype){
    return (neurontype.a_cond * (v_memb - neurontype.el_rest) - w_current) / neurontype.tauw_cons;
}


int main(){

    struct one_Neuron neurontype = fig4g; // changable
    double v_memb, w_current;
    v_memb = neurontype.el_rest;
    w_current = 0.0;

    FILE *v_file;
    char *v_filename = "fig4g_voltage.txt";
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
        v_memb += DT * dvdt(v_memb, w_current, neurontype);
        w_current += DT * dwdt(v_memb, w_current, neurontype);

        if(v_memb > 0){
            fprintf(v_file, "%f %f\n", time * DT, 0.0);
            v_memb = neurontype.vr_reset;
            w_current = w_current + neurontype.b_reset;
        }
    }

    fclose(v_file);
    return 0;
}