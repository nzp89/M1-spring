// adaptive exponential integrate-and-fire(AEIF) neurons
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N_NEURONS 1000 // number of neuron
#define P_CONNECT 0.1
#define CM_CAPA 200.0 // capacitance membrane [pF]
#define GL_COND 12.0 // leak conductance [nS]
#define EL_REST -70.0 // resting potential [mV]
#define SLOPEFACT 2.0 // slope factor [mV]
#define VT_THETA -50.0 // spike threshold [mV]
#define TAUW_CONS 300.0 // adaptation time constant [ms]
#define TAUS_CONS 2.728 // synaptic time constant [ms]
#define VREV_EXC 0.0 // excitatory synaptic reversal potential [mV]
#define VREV_INH -80.0 // inhibitory synaptic reversal potential [mV]

double dvdt(double v_memb, double v_rev, double i_inj, double w_adap, double sum_syn, double i_ext){
    return ( -GL_COND * (v_memb - EL_REST) + GL_COND * SLOPEFACT * exp((v_memb - VT_THETA) / SLOPEFACT) + i_inj - w_adap + sum_syn + i_ext ) / CM_CAPA;
}

double dwidt(double a_i, double v_memb, double w_adap){
    return ( a_i * (v_memb - EL_REST) - w_adap ) / TAUW_CONS;
}

double dgidt(double g_i){
    return -g_i / TAUS_CONS;
}

double sum_synaptis_connection(int neuron_index, double v_i, double Mij[][N_NEURONS], double *g_syn){
    double v_rev;
    double sum_syn = 0.0;
    for(int j = 0; j < N_NEURONS; j++){
        if( j <= (int)(0.8 * N_NEURONS) ){ 
            v_rev = VREV_EXC;
        }else{ 
            v_rev = VREV_INH;
        }
        sum_syn += (v_rev - v_i) * Mij[neuron_index][j] * g_syn[neuron_index];
    }
    return sum_syn;
}

double generate_subthreshold_ai(){
    // subthreshold adaptation [nS] (randomly distributed in the interval [0.19,0.21])
    double min, max;
    min = 0.19;
    max = 0.21;
    return min + (double)(rand() * (max - min) / (1.0 + RAND_MAX));
}

int main(){
    double Vi_memb_potential[N_NEURONS];
    double wi_adap_current[N_NEURONS];
    double gammai_ext_current[N_NEURONS];
    double Ii_inj_current[N_NEURONS];
    double gi_syn_conductance[N_NEURONS];

    double sum_synconnect[N_NEURONS];
    double Mij_connect[N_NEURONS][N_NEURONS];

    srand((unsigned int)time(NULL));
    

    return 0;
}