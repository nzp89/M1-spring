#ifndef PARAMETERS_H
#define PARAMETERS_H


struct Neurons_cons{
    int n_number; // number of neurons
    double p_connect; // probability to connect
    double cm_capa; // membrane capacitance [pF]
    double gl_cond; // leak conductance [nS]
    double el_rest; // resting potential [mV]
    double vt_theta; // spike threshold [mV]
    double slopefactor; // slope factor [mV]
    double vr_reset; // reset potential [mV]
    double tauw_cons; // adaptation time constant [ms]
    double taus_cons; // synaptic time constant [ms]
    double vrev_exc; // excitatory synaptic reversal potential [mV]
    double vrev_inh; // inhibitory synaptic reversal potential [mV]
    double exc_rate; // percentage of excitatory neuron
};

const struct Neurons_cons aeif_bistable = {
    .n_number = 1000,
    .p_connect = 0.1,
    .cm_capa = 200,
    .gl_cond = 12,
    .el_rest = -70, 
    .vt_theta = -50,
    .slopefactor = 2,
    .vr_reset = -58,
    .tauw_cons = 300,
    .taus_cons = 2.728,
    .vrev_exc = 0,
    .vrev_inh = -80,
    .exc_rate = 0.8
};


#endif