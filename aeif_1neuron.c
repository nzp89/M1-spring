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
    // double output = neuron.i_ext - w_current;
    // fprintf(exp_file, "(%lf)-(%lf) %lf ",neuron.i_ext, w_current, output);
    return (( -neuron.gl_cond * (v_memb - neuron.el_rest) + neuron.gl_cond * neuron.slopefactor * exp(exp_term) + neuron.i_ext - w_current ) / neuron.c_capa);
}

double dwdt(double v_memb, double w_current, struct one_Neuron neuron, FILE *exp_file){
    // double output = w_current;
    // fprintf(exp_file, "%lf ", output);
    return ((neuron.a_cond * (v_memb - neuron.el_rest) - w_current) / neuron.tauw_cons);
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
            //printf("time count : %d\n", time);
            fprintf(v_file, "%f %f\n", time * DT, v_memb);
            fprintf(exp_file, "%f\n", time * DT);
        }
        
        if(true){
             // runge-kutta method
            double kv1 = DT * dvdt(v_memb, w_current, neuron, exp_file);
            double kw1 = DT * dwdt(v_memb, w_current, neuron, exp_file);
            fprintf(exp_file, "%lf = %f * {-%f * (%lf - %f) + %f * %f * exp(%lf) + %f - %lf} / %f\n", kv1, DT, neuron.gl_cond, v_memb, neuron.el_rest, neuron.gl_cond, neuron.slopefactor, (v_memb - neuron.vt_theta) / neuron.slopefactor, neuron.i_ext, w_current, neuron.c_capa);

            double kv2 = DT * dvdt(v_memb + 0.5 * kv1, w_current + 0.5 * kw1, neuron, exp_file);
            double kw2 = DT * dwdt(v_memb + 0.5 * kv1, w_current + 0.5 * kw1, neuron, exp_file);
            fprintf(exp_file, "%lf = %f * {-%f * (%lf - %f) + %f * %f * exp(%lf) + %f - %lf} / %f\n", kv2, DT, neuron.gl_cond, v_memb + 0.5 * kv1, neuron.el_rest, neuron.gl_cond, neuron.slopefactor, (v_memb + 0.5 * kv1 - neuron.vt_theta) / neuron.slopefactor, neuron.i_ext, w_current + 0.5 * kw1, neuron.c_capa);
            
            double kv3 = DT * dvdt(v_memb + 0.5 * kv2, w_current + 0.5 * kw2, neuron, exp_file);
            double kw3 = DT * dwdt(v_memb + 0.5 * kv2, w_current + 0.5 * kw2, neuron, exp_file);
            fprintf(exp_file, "%lf = %f * {-%f * (%lf - %f) + %f * %f * exp(%lf) + %f - %lf} / %f\n", kv3, DT, neuron.gl_cond, v_memb + 0.5 * kv2, neuron.el_rest, neuron.gl_cond, neuron.slopefactor, (v_memb + 0.5 * kv2 - neuron.vt_theta) / neuron.slopefactor, neuron.i_ext, w_current + 0.5 * kw2, neuron.c_capa);
            
            double kv4 = DT * dvdt(v_memb + kv3, w_current + kw3, neuron, exp_file);
            double kw4 = DT * dwdt(v_memb + kv3, w_current + kw3, neuron, exp_file);
            fprintf(exp_file, "%lf = %f * {-%f * (%lf - %f) + %f * %f * exp(%lf) + %f - %lf} / %f\n", kv4, DT, neuron.gl_cond, v_memb + kv3, neuron.el_rest, neuron.gl_cond, neuron.slopefactor, (v_memb + kv3 - neuron.vt_theta) / neuron.slopefactor, neuron.i_ext, w_current + kw3, neuron.c_capa);

            v_memb += (kv1 + 2 * kv2 + 2 * kv3 + kv4) / 6.0;
            w_current += (kw1 + 2 * kw2 + 2 * kw3 + kw4) / 6.0;
            fprintf(exp_file, "%lf = v_old + (%lf + 2 * %lf + 2 * %lf + %lf) / 6\n", v_memb, kv1, kv2, kv3, kv4);
        }else{
            // euler method
            v_memb += DT * dvdt(v_memb, w_current, neuron, exp_file);
            w_current += DT * dwdt(v_memb, w_current, neuron, exp_file);
            fprintf(exp_file, "%lf = old_v + %f * {-%f * (%lf - %f) + %f * %f * exp(%lf) + %f - %lf} / %f\n",v_memb, DT, neuron.gl_cond, v_memb, neuron.el_rest, neuron.gl_cond, neuron.slopefactor, (v_memb - neuron.vt_theta) / neuron.slopefactor, neuron.i_ext, w_current, neuron.c_capa);
        } 
        fprintf(exp_file, "\n");
        if(v_memb > 0.0){
            fprintf(v_file, "%f %f\n", time * DT, 0.0);
            v_memb = neuron.vr_reset;
            w_current = w_current + neuron.b_reset;
        }

    }

    fclose(v_file);
    return 0;
}
