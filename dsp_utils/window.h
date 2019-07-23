#pragma once

#include "signal_types.h"

#include <cmath>

namespace dsp_utils{
namespace window
{
template <class T>
std::vector<T> taylor(size_t N, double SSL = -55, size_t N_lobes = 9)
{
    static_assert (is_real_v<T>, "only real signals supported!");
    auto sqr = [](auto&& x){return x * x;};


    double D =  std::acosh(std::pow(10., -SSL / 20.)) / M_PI;
    double SS = sqr(N_lobes) / (sqr(D) + sqr(N_lobes - 0.5));

    std::vector<double> FF;

    for (int i = 1; i < N_lobes; ++i){
        double val = (i % 2) ? 1 : -1;
        for (int j = 1; j < N_lobes; ++j)
        {
            val *= (1. - sqr(i) / (SS * (sqr(D) + sqr(j-0.5))));
            if (i != j){
                val /= (1. - sqr(double(i)/j) );
            }
        }
        FF.push_back(val / 2);
    }


    std::vector<T> taps;

    for (size_t i = 0; i < N; ++i){
        int m = 1;
        double t = -0.5 + double(i) / N;
        double tap = 0;
        for (auto k : FF){
            tap += k * std::cos( m * t * 2 * M_PI  );
            ++m;
        }
        taps.push_back(static_cast<T>(tap * 2 + 1));

    }
    return taps;
}


}
}
