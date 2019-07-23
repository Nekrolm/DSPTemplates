#pragma once


#include "signal_types.h"
#include <cmath>

namespace dsp_utils {
namespace signal_gen
{
template <class Ttap>
std::vector<std::complex<Ttap>> lfm(size_t sample_rate_Hz, double duration_s, double spec_width_Hz,
                                    double cfreq_ofs_Hz = 0, double start_phase = 0)
{
    double dt = 1. / sample_rate_Hz;
    double T = duration_s;
    double Tbegin = -T / 2;
    size_t N = sample_rate_Hz * T;

    std::vector<std::complex<Ttap>> ret;
    for (size_t i = 0; i < N; ++i){
        auto t = Tbegin + i * dt;
        Ttap ang = M_PI * t * t / T * spec_width_Hz + start_phase + 2*M_PI * cfreq_ofs_Hz * t;
        ang = std::fmod(ang, 2*M_PI);
        ret.push_back(std::polar<Ttap>(Ttap(1), ang));
    }
    return ret;
}





}
}
