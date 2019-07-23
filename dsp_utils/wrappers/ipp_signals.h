#pragma once

#include "ipp_types.h"

#include "ipp_transforms.h"

#include <cmath>

namespace dsp_utils {
namespace ipp {
template<class T>
struct IppSignalHelper;

#define MAKE_HELPER(type_name, suffix) template<>         \
    struct IppSignalHelper<type_name>                     \
    {                                                     \
        static constexpr auto tone = ippsTone_ ## suffix; \
    };


MAKE_HELPER(Ipp32fc, 32fc)
MAKE_HELPER(Ipp64fc, 64fc)

MAKE_HELPER(Ipp64f, 64f)
MAKE_HELPER(Ipp32f, 32f)


template<class T>
inline void _simple_tone(T* dst,
                         std::size_t N,
                         BaseType<T> ampl,
                         BaseType<T> rFreq,
                         BaseType<T>* phase,
                         IppHintAlgorithm hint)
{
    BaseType<T> phase_zero = 0;

    if (phase != nullptr)
    {
        phase_zero = *phase;
    }
    IppSignalHelper<T>::tone(dst, N, ampl, rFreq, &phase_zero, hint);

    if (phase != nullptr)
    {
        *phase = phase_zero;
    }
}

template<class T>
inline void tone(T* dst, std::size_t N, BaseType<T> ampl,
                 BaseType<T> rFreq,
                 BaseType<T>* phase = nullptr,
                 IppHintAlgorithm hint = IppHintAlgorithm::ippAlgHintFast)
{
    _simple_tone(dst, N, ampl, rFreq, phase, hint);
}

// dst[n] = ampl * e^((2PI*rFreq*n + phase) * i)



template<class T>
inline void _complex_tone(T* dst,
                          std::size_t N,
                          BaseType<T> ampl,
                          BaseType<T> rFreq,
                          BaseType<T>* phase,
                          IppHintAlgorithm hint)
{
    BaseType<T> zero_phase = 0;

    if (phase)
        zero_phase = *phase;

    if ((rFreq < 0) && (zero_phase > 0))
    {
        zero_phase = 2 * M_PI - zero_phase;
    }

    IppSignalHelper<T>::tone(dst, N, ampl, std::abs(rFreq), &zero_phase, hint);

    if (rFreq < 0)
    {
        conj(dst, N);

        if (zero_phase > 0)
            zero_phase = M_PI * 2 - zero_phase;
    }

    if (phase)
    {
        *phase = zero_phase;
    }
}

// for complex signal allow frequency (-1, 1)
template<>
inline void tone(Ipp32fc* dst, std::size_t N,
                 float ampl,   float rFreq,
                 float* phase, IppHintAlgorithm hint)
{
    _complex_tone(dst, N, ampl, rFreq, phase, hint);
}

template<>
inline void tone(Ipp64fc* dst, std::size_t N,
                 double ampl,   double rFreq,
                 double* phase, IppHintAlgorithm hint)
{
    _complex_tone(dst, N, ampl, rFreq, phase, hint);
}
}
}
