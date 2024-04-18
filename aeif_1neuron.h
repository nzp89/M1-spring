#ifndef PARAMETERS_H
#define PARAMETERS_H

struct one_Neuron{
    double c_capa; // capacitance [pF]
    double gl_cond; // leak conductance [nS]
    double el_rest; // resting potential [mV]
    double vt_theta; // spike threshold [mV]
    double slopefactor; // slope factor [mV] 
    double a_cond; // w's conductance [nS]
    double tauw_cons; // time constant [ms]
    double b_reset; // reset current [pA]
    double vr_reset; // reset potential [mV]
    double i_ext; // ext current [pA]
};

const struct one_Neuron fig4a = {
    .c_capa = 200,
    .gl_cond = 10,
    .el_rest = -70,
    .vt_theta = -50,
    .slopefactor = 2,
    .a_cond = 2,
    .tauw_cons = 30,
    .b_reset = 0,
    .vr_reset = -58,
    .i_ext = 500
};

const struct one_Neuron fig4b = {
    .c_capa = 200,
    .gl_cond = 12,
    .el_rest = -70,
    .vt_theta = -50,
    .slopefactor = 2,
    .a_cond = 2,
    .tauw_cons = 300,
    .b_reset = 60,
    .vr_reset = -58,
    .i_ext = 500
};

const struct one_Neuron fig4c = {
    .c_capa = 130,
    .gl_cond = 18,
    .el_rest = -58,
    .vt_theta = -50,
    .slopefactor = 2,
    .a_cond = 4,
    .tauw_cons = 150,
    .b_reset = 120,
    .vr_reset = -50,
    .i_ext = 400
};

const struct one_Neuron fig4d = {
    .c_capa = 200,
    .gl_cond = 10,
    .el_rest = -58,
    .vt_theta = -50,
    .slopefactor = 2,
    .a_cond = 2,
    .tauw_cons = 120,
    .b_reset = 100,
    .vr_reset = -46,
    .i_ext = 210
};

const struct one_Neuron fig4e = {
    .c_capa = 200,
    .gl_cond = 12,
    .el_rest = -70,
    .vt_theta = -50,
    .slopefactor = 2,
    .a_cond = -10,
    .tauw_cons = 300,
    .b_reset = 0,
    .vr_reset = -58,
    .i_ext = 300
};

const struct one_Neuron fig4f = {
    .c_capa = 200,
    .gl_cond = 12,
    .el_rest = -70,
    .vt_theta = -50,
    .slopefactor = 2,
    .a_cond = -6,
    .tauw_cons = 300,
    .b_reset = 0,
    .vr_reset = -58,
    .i_ext = 110
};

const struct one_Neuron fig4g = {
    .c_capa = 100,
    .gl_cond = 10,
    .el_rest = -65,
    .vt_theta = -50,
    .slopefactor = 2,
    .a_cond = -10,
    .tauw_cons = 90,
    .b_reset = 30,
    .vr_reset = -47,
    .i_ext = 350
};

const struct one_Neuron fig4h = {
    .c_capa = 100,
    .gl_cond = 12,
    .el_rest = -60,
    .vt_theta = -50,
    .slopefactor = 2,
    .a_cond = -11,
    .tauw_cons = 130,
    .b_reset = 30,
    .vr_reset = -48,
    .i_ext = 160
};


#endif