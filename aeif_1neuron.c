// "one" adaptive exponential integrate-and-fire(AEIF) neuron
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "aeif_1neuron.h"

#define TMAX 1000.0 // time max [ms]
#define DT 0.001 // time difference [ms]

double dvdt(double v_memb, double w_current, struct one_Neuron neuron, FILE *exp_file){
    double exp_term = (v_memb - neuron.vt_theta) / neuron.slopefactor;
    // exp_term = (exp_term > 15) ? 15: exp_term;
    fprintf(exp_file, "%lf ", exp_term);
    return ( -neuron.gl_cond * (v_memb - neuron.el_rest) + neuron.gl_cond * neuron.slopefactor * exp(exp_term) + neuron.i_ext - w_current ) / neuron.c_capa;
}

double dwdt(double v_memb, double w_current, struct one_Neuron neuron){
    return (neuron.a_cond * (v_memb - neuron.el_rest) - w_current) / neuron.tauw_cons;
}


int main(){
// parameter setup
    struct one_Neuron neuron = fig4a; // changable
    double v_memb, w_current;
    v_memb = neuron.el_rest;
    w_current = 0.0;
// file setup
    FILE *v_file, *exp_file;
    char *v_filename = "output_txt/fig4a_voltage.txt";
// debag menu (erase later)
    char *exp_filename = "output_txt/exp_nakami.txt";
    v_file = fopen(v_filename, "w");
    exp_file = fopen(exp_filename, "w");
    if(v_file == NULL || exp_file == NULL){
        printf("cannot open the file\n");
        return 1;
    }
// calculate setup
    int countmax = (int) TMAX / DT;
 
    for(int time = 0; time < countmax; time++){
        if(time % 1 == 0){
            printf("time count : %d\n", time);
            fprintf(v_file, "%f %f\n", time * DT, v_memb);
            fprintf(exp_file, "%f ", time * DT);
        }
        
        if(true){
             // runge-kutta method
            double kv1 = DT * dvdt(v_memb, w_current, neuron, exp_file);
            double kw1 = DT * dwdt(v_memb, w_current, neuron);

            double kv2 = DT * dvdt(v_memb + 0.5 * kv1, w_current + 0.5 * kw1, neuron, exp_file);
            double kw2 = DT * dwdt(v_memb + 0.5 * kv1, w_current + 0.5 * kw1, neuron);

            double kv3 = DT * dvdt(v_memb + 0.5 * kv2, w_current + 0.5 * kw2, neuron, exp_file);
            double kw3 = DT * dwdt(v_memb + 0.5 * kv2, w_current + 0.5 * kw2, neuron);

            double kv4 = DT * dvdt(v_memb + kv3, w_current + kw3, neuron, exp_file);
            double kw4 = DT * dwdt(v_memb + kv3, w_current + kw3, neuron);

            v_memb += (kv1 + 2 * kv2 + 2 * kv3 + kv4) / 6.0;
            w_current += (kw1 + 2 * kw2 + 2 * kw3 + kw4) / 6.0;
        }else{
            // euler method
            v_memb += DT * dvdt(v_memb, w_current, neuron, exp_file);
            w_current += DT * dwdt(v_memb, w_current, neuron);
        } 
        fprintf(exp_file, "\n");
        if(v_memb > 0){
            fprintf(v_file, "%f %f\n", time * DT, 0.0);
            v_memb = neuron.vr_reset;
            w_current = w_current + neuron.b_reset;
        }

    }

    fclose(v_file);
    return 0;
}
