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
#define VR_RESET -58.0 // reset potential [mV]
#define TAUW_CONS 300.0 // adaptation time constant [ms]
#define TAUS_CONS 2.728 // synaptic time constant [ms]
#define VREV_EXC 0.0 // excitatory synaptic reversal potential [mV]
#define VREV_INH -80.0 // inhibitory synaptic reversal potential [mV]
#define EXC_RATE 0.8 // excitatory is 80%

#define G_EXC 1.0 // excitatory synapse conductance [nS]
#define G_INH 4.0 // inhibitory synapse conductance [nS]

#define I_INJ 255.0 // first injection current [pA]

#define DT 0.001 // [s]
#define MAX_TIME 1.0 // [s]

double random_number(double min, double max){
    // make a random number [min, max]
    return min + (double)(rand() * (max - min) / (1.0 + RAND_MAX));
}

double dvdt(double v_memb, double i_inj, double w_adap, double sum_syn, double i_ext){
    // equation (1)-1 
    return ( -GL_COND * (v_memb - EL_REST) + GL_COND * SLOPEFACT * exp((v_memb - VT_THETA) / SLOPEFACT) + i_inj - w_adap + sum_syn + i_ext ) / CM_CAPA;
}

double dwidt(double a_subt, double v_memb, double w_adap){
    // equation (1)-2
    return ( a_subt * (v_memb - EL_REST) - w_adap ) / TAUW_CONS;
}

double dgidt(double g_syn){
    // equation (1)-3
    return -g_syn / TAUS_CONS;
}

void sum_synaptic_connection(int neuron_index, double *v_memb, double Mij[][N_NEURONS], double *g_syn, double *sum){
    // equation (1)-1-sum
    double v_rev;
    double sum_syn = 0.0;
    for(int j = 0; j < N_NEURONS; j++){
        v_rev = (j <= (int)(EXC_RATE * N_NEURONS)) ? VREV_EXC : VREV_INH;
        sum_syn += (v_rev - v_memb[neuron_index]) * Mij[neuron_index][j] * g_syn[neuron_index];
    }
    sum[neuron_index] = sum_syn;
    return;
}

void generate_connection_mij(double Mij[][N_NEURONS]){
    // make connection according to connection probability
    int row, column;
    double rand_tmp; // random number temporary
    for (row = 0; row < N_NEURONS; row++){
        for (column = 0; column < N_NEURONS; column++){
            rand_tmp = random_number(0,1);
            Mij[row][column] = (rand_tmp < P_CONNECT) ? 1 : 0;
        }
    }
}

void initialize(double *v_memb, double *w_adap, double *g_syn, double *a_subt, double *gamma, double *i_inj){
    double v_max, v_min, w_max, w_min, a_max, a_min;
    v_max = -50;
    v_min = -70;
    w_max = 70;
    w_min = 0;
    a_max = 0.21;
    a_min = 0.19;

    for(int i = 0; i < N_NEURONS; i++){
        v_memb[i] = random_number(v_min, v_max); // [-70, -50] [mV]
        w_adap[i] = random_number(w_min, w_max); // [0, 70] [pA]
        a_subt[i] = random_number(a_min, a_max); // [0.19, 0.21] [nS]
        g_syn[i] = 0;
        gamma[i] = 0;
        i_inj[i] = I_INJ;
    }
    return;
}


int main(){
    double v_memb_potential[N_NEURONS]; // 
    double w_adap_current[N_NEURONS]; // manual
    double gamma_ext_current[N_NEURONS]; //
    double I_inj_current[N_NEURONS]; // manual
    double g_syn_conductance[N_NEURONS]; // 
    double a_subthreshold[N_NEURONS]; //

    double sum_synconnect[N_NEURONS]; //
    double Mij_connect[N_NEURONS][N_NEURONS]; //

    int count_limit = (int) MAX_TIME / DT;
    
    FILE *v_out_pointer, *t_out_pointer;
    char *v_filename = "v_memb_out.txt";
    char *t_filename = "t_spike_out.txt";

    v_out_pointer = fopen(v_filename, "w");
    if(v_out_pointer == NULL){
        printf("cannot open output file : %s \n", v_filename);
        return 1;
    }

    t_out_pointer = fopen(t_filename, "w");
    if(t_out_pointer == NULL){
        printf("cannot open output file : %s \n", t_filename);
        return 1;
    }

    srand((unsigned int)time(NULL));
    initialize(v_memb_potential, w_adap_current, g_syn_conductance, a_subthreshold, gamma_ext_current, I_inj_current);
    generate_connection_mij(Mij_connect);
    // calculate defferential equation
    for (int t = 0; t < count_limit; t++){
        printf("time count : %d\n", t);
        fprintf(v_out_pointer, "%f ", t * DT);
        for (int i = 0; i < N_NEURONS; i++){
            sum_synaptic_connection(i, v_memb_potential, Mij_connect, g_syn_conductance, sum_synconnect);

            v_memb_potential[i] += DT * dvdt(v_memb_potential[i], I_inj_current[i], w_adap_current[i], sum_synconnect[i], gamma_ext_current[i]);
            w_adap_current[i] += DT * dwidt(a_subthreshold[i], v_memb_potential[i], w_adap_current[i]);
            g_syn_conductance[i] += DT * dgidt(g_syn_conductance[i]);
            
            if(v_memb_potential[i] > VT_THETA){
                printf("%d spiked\n", i);
                fprintf(t_out_pointer, "%f %d\n", t * DT, i);
                v_memb_potential[i] = -58.0;
                w_adap_current[i] += 70;
                g_syn_conductance[i] += (i <= (int) EXC_RATE * N_NEURONS) ? G_EXC : G_INH;
            }

            fprintf(v_out_pointer, "%lf ", v_memb_potential[i]);
        }
        fprintf(v_out_pointer, "\n");
    }

    fclose(v_out_pointer);
    fclose(t_out_pointer);
    return 0;
}