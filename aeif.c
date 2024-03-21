// adaptive exponential integrate-and-fire(AEIF) neurons
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "aeif_bistable.h" // neuron's constants 

#define G_EXC 1.0 // excitatory synapse conductance [nS]
#define G_INH 4.0 // inhibitory synapse conductance [nS]

#define I_INJ 220.0 // first injection current [pA]

#define DT 0.001 // [ms]
#define MAX_TIME 1000.0 // [ms]

double random_number(double min, double max){
    // make a random number [min, max]
    return min + (double)(rand() * (max - min) / (1.0 + RAND_MAX));
}

double dvdt(double v_memb, double i_inj, double w_adap, double sum_syn, double i_ext, struct Neurons_cons np){
    // equation (1)-1 
    return ( -np.gl_cond * (v_memb - np.el_rest) + np.gl_cond * np.slopefactor * exp((v_memb - np.vt_theta) / np.slopefactor) + i_inj - w_adap + sum_syn + i_ext ) / np.cm_capa;
}

double dwidt(double a_subt, double v_memb, double w_adap, struct Neurons_cons np){
    // equation (1)-2
    return ( a_subt * (v_memb - np.el_rest) - w_adap ) / np.tauw_cons;
}

double dgidt(double g_syn, struct Neurons_cons np){
    // equation (1)-3
    return -g_syn / np.taus_cons;
}

void sum_synaptic_connection(int neuron_index, double *v_memb, int **Mij, double *g_syn, double *sum, struct Neurons_cons np){
    // equation (1)-1-sum
    double v_rev;
    double sum_syn = 0.0;
    for(int j = 0; j < np.n_number; j++){
        v_rev = (j <= (int)(np.exc_rate * np.n_number)) ? np.vrev_exc : np.vrev_inh;
        sum_syn += (v_rev - v_memb[neuron_index]) * Mij[neuron_index][j] * g_syn[neuron_index];
    }
    sum[neuron_index] = sum_syn;
    return;
}

void generate_connection_mij(int **Mij, FILE *mij_out, struct Neurons_cons np){
    // make connection according to connection probability
    int pre_neuron, post_neuron;
    double rand_tmp; // random number temporary
    for (pre_neuron = 0; pre_neuron < np.n_number; pre_neuron++){
        for (post_neuron = 0; post_neuron < np.n_number; post_neuron++){
            if (pre_neuron != post_neuron){
                rand_tmp = random_number(0,1);
                Mij[pre_neuron][post_neuron] = (rand_tmp < np.p_connect) ? 1 : 0;
                fprintf(mij_out, "%d ", Mij[pre_neuron][post_neuron]);
            }else{
                // pre = post -> no connection
                Mij[pre_neuron][post_neuron] = 0;
                fprintf(mij_out, "%d ", Mij[pre_neuron][post_neuron]);
            }
        }
        fprintf(mij_out, "\n");
    }
    return;
}

void initialize(double *v_memb, double *w_adap, double *g_syn, double *a_subt, double *gamma, double *i_inj, struct Neurons_cons np){
    double v_max, v_min, w_max, w_min, a_max, a_min;

    v_max = -50;
    v_min = -70;
    w_max = 70;
    w_min = 0;
    a_max = 0.21;
    a_min = 0.19;

    for(int i = 0; i < np.n_number; i++){
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

    struct Neurons_cons np = aeif_bistable;

    // 各配列のメモリ確保
    double *v_memb_potential = (double *)malloc(np.n_number * sizeof(double));
    double *w_adap_current = (double *)malloc(np.n_number * sizeof(double));
    double *gamma_ext_current = (double *)malloc(np.n_number * sizeof(double));
    double *I_inj_current = (double *)malloc(np.n_number * sizeof(double));
    double *g_syn_conductance = (double *)malloc(np.n_number * sizeof(double));
    double *a_subthreshold = (double *)malloc(np.n_number * sizeof(double));

    double *sum_synconnect = (double *)malloc(np.n_number * sizeof(double));

    // 2次元配列のメモリ確保
    int **Mij_connect = (int **)malloc(np.n_number * sizeof(int *));
    for (int i = 0; i < np.n_number; i++) {
        Mij_connect[i] = (int *)malloc(np.n_number * sizeof(int));
    }

    int count_limit = (int) MAX_TIME / DT;
    
    FILE *v_out_pointer, *t_out_pointer, *mij_out_pointer, *w_out_pointer;
    char *v_filename = "v_memb_out.txt";
    char *t_filename = "t_spike_out.txt";
    char *mij_filename = "connection.txt";
    char *w_filename = "current.txt";

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

    mij_out_pointer = fopen(mij_filename, "w");
    if(mij_out_pointer == NULL){
        printf("cannot open output file : %s \n", mij_filename);
    }

    w_out_pointer = fopen(w_filename, "w");
    if(w_out_pointer == NULL){
        printf("cannot open output file : %s \n", w_filename);
    }


    srand((unsigned int)time(NULL));
    initialize(v_memb_potential, w_adap_current, g_syn_conductance, a_subthreshold, gamma_ext_current, I_inj_current, np);
    generate_connection_mij(Mij_connect, mij_out_pointer, np);
    // calculate defferential equation
    for (int t = 0; t < count_limit; t++){
        if(t % 100 == 0){
            printf("time : %f ms\n", t * DT);
            fprintf(v_out_pointer, "%f ", t * DT);
            fprintf(w_out_pointer, "%f ", t * DT);
        }
        for (int i = 0; i < np.n_number; i++){
            sum_synaptic_connection(i, v_memb_potential, Mij_connect, g_syn_conductance, sum_synconnect, np);

            v_memb_potential[i] += DT * dvdt(v_memb_potential[i], I_inj_current[i], w_adap_current[i], sum_synconnect[i], gamma_ext_current[i], np);
            w_adap_current[i] += DT * dwidt(a_subthreshold[i], v_memb_potential[i], w_adap_current[i], np);
            g_syn_conductance[i] += DT * dgidt(g_syn_conductance[i], np);
            
            if(v_memb_potential[i] > np.vt_theta){
                //printf("%d spiked\n", i);
                fprintf(t_out_pointer, "%f %d\n", t * DT, i);
                v_memb_potential[i] = np.vr_reset;
                w_adap_current[i] += 70;
                g_syn_conductance[i] += (i <= (int) np.exc_rate * np.n_number) ? G_EXC : G_INH;
            }
            if(t % 100 == 0){
                fprintf(v_out_pointer, "%lf ", v_memb_potential[i]);
                fprintf(w_out_pointer, "%lf ", w_adap_current[i]);
            }
        }
        if(t % 100 == 0){
            fprintf(v_out_pointer, "\n");
            fprintf(w_out_pointer, "\n");
        }
    }

    fclose(v_out_pointer);
    fclose(t_out_pointer);
    fclose(mij_out_pointer);
    fclose(w_out_pointer);
    return 0;
}