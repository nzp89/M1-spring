// "one" adaptive exponential integrate-and-fire(AEIF) neuron
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "aeif_1neuron.h"

#define TMAX 1000.0 // time max [ms]
#define DT 0.01 // time difference [ms]

double dvdt(double v_memb, struct one_Neuron neuron){
    double exp_term = (v_memb - neuron.vt_theta) / neuron.slopefactor;
    return (( -neuron.gl_cond * (v_memb - neuron.el_rest) + neuron.gl_cond * neuron.slopefactor * exp(exp_term) + neuron.i_ext ) / neuron.c_capa);
}
int main(){
// parameter setup
    struct one_Neuron neuron = fig4a; // changable
    double v_memb, dv;
    int fHz = 0;
    v_memb = neuron.vr_reset;
// file setup
    FILE *v_file;
    char *v_filename = "output_txt/fig4a_voltage.txt";
    v_file = fopen(v_filename, "w");
// calculate
    int countmax = (int) TMAX / DT;

    for(int time = 0; time < countmax; time++){
        fprintf(v_file, "%f %f\n", time * DT, v_memb);
        double kv1 = DT * dvdt(v_memb, neuron);
        double kv2 = DT * dvdt(v_memb + 0.5 * kv1, neuron);
        double kv3 = DT * dvdt(v_memb + 0.5 * kv2, neuron);
        double kv4 = DT * dvdt(v_memb + kv3, neuron);
        dv = (kv1 + 2 * kv2 + 2 * kv3 + kv4) / 6.0;
        if(v_memb > 0.0){
            v_memb = neuron.vr_reset;
            fHz++;
        }else{
            v_memb = v_memb + dv;
        }
    }
    printf("%d\n", fHz);
    fclose(v_file);
    return 0;

}

