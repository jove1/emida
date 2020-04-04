#include "subpixel_max.hpp"

using namespace emida;

const std::array<double, 6 * 3 * 3> lstsq_matrix<double,3>::mat{
 58.0/72, 16.0/72, -2.0/72, 16.0/72, -8.0/72, -8.0/72, -2.0/72, -8.0/72, 10.0/72,
-3.0/4, 2.0/3, 6.0/72, -1.0/2, 2.0/3, -12.0/72, -1.0/4, 2.0/3, -30.0/72,
-3.0/4, -1.0/2, -1.0/4, 2.0/3, 2.0/3, 2.0/3, 6.0/72, -12.0/72, -30.0/72,
12.0/72, -1.0/3, 1.0/6, 1.0/6, -1.0/3, 1.0/6, 1.0/6, -1.0/3, 1.0/6,
1.0/4, 0, -1.0/4, 0, 0, 0, -1.0/4, 0, 1.0/4,
1.0/6, 1.0/6, 1.0/6, -1.0/3, -1.0/3, -1.0/3, 1.0/6, 1.0/6, 1.0/6 };

/*
const std::array<double, 6 * 3 * 3> lstsq_matrix<double, 3>::mat = { 0.8055555555555551, 0.22222222222222301, -0.027777777777776236, 0.22222222222222135, -0.1111111111111108, -0.11111111111111002, -0.027777777777779566, -0.1111111111111115, 0.13888888888888895,
-0.7499999999999991, 0.666666666666667, 0.08333333333333304, -0.4999999999999997, 0.6666666666666664, -0.16666666666666727, -0.24999999999999914, 0.6666666666666671, -0.41666666666666724,
-0.7500000000000001, -0.5000000000000023, -0.25000000000000083, 0.6666666666666703, 0.6666666666666679, 0.6666666666666698, 0.08333333333333481, -0.16666666666666785, -0.4166666666666661,
0.1666666666666664, -0.3333333333333336, 0.16666666666666652, 0.1666666666666664, -0.3333333333333336, 0.16666666666666652, 0.1666666666666664, -0.3333333333333336, 0.16666666666666652,
0.24999999999999972, -6.01370805005286e-17, -0.2500000000000003, 1.6653345369377348e-16, 3.885780586188048e-16, 1.6653345369377348e-16, -0.2500000000000003, -5.551115123125783e-17, 0.24999999999999978,
0.16666666666666688, 0.16666666666666785, 0.16666666666666724, -0.33333333333333504, -0.33333333333333404, -0.3333333333333347, 0.16666666666666652, 0.1666666666666674, 0.16666666666666696 };
*/