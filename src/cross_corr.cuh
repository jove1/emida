#pragma once

#include <cstdint>

namespace emida
{

template<typename T, typename RES>
void run_cross_corr(const T* pic_a, const T* pic_b, RES* res, int cols, int rows);


}