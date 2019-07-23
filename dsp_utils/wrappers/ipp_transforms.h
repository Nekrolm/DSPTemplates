#pragma once

#include "ipp_types.h"

#include <ipp.h>

namespace dsp_utils {
namespace ipp {
// REAL TRANSFORMS

template<class T>
struct IppComplexTransformsHelper;


#define MAKE_HELPER(type_name, base_type, suffix) template<>                \
    struct IppComplexTransformsHelper<type_name>                            \
    {                                                                       \
        using real_type = base_type;                                        \
        static constexpr auto power_spectrum  = ippsPowerSpectr_ ## suffix; \
        static constexpr auto complex_to_real = ippsCplxToReal_ ## suffix;  \
        static constexpr auto phase           = ippsPhase_ ## suffix;       \
        static constexpr auto magnitude       = ippsMagnitude_ ## suffix;   \
        static constexpr auto conj            = ippsConj_ ## suffix;        \
        static constexpr auto conj_implace    = ippsConj_ ## suffix ## _I;  \
    };


MAKE_HELPER(Ipp32fc, Ipp32f, 32fc)
MAKE_HELPER(Ipp64fc, Ipp64f, 64fc)

#undef MAKE_HELPER





template<class T>
inline void power_spectrum(const T* src, typename IppComplexTransformsHelper<T>::real_type* dst, std::size_t len)
{
    IppComplexTransformsHelper<T>::power_spectrum(src, dst, len);
}

template<class T>
inline void conj(const T* src, T* dst, std::size_t len)
{
    IppComplexTransformsHelper<T>::conj(src, dst, len);
}

template<class T>
inline void conj(T* srcDst, std::size_t len)
{
    IppComplexTransformsHelper<T>::conj_implace(srcDst, len);
}

// REAL TRANSFORMS

template<class T>
struct IppRealTransformsHelper;

#define MAKE_HELPER(type_name, suffix, prec) template<>                                        \
    struct IppRealTransformsHelper<type_name>                                                  \
    {                                                                                          \
        static constexpr auto real_to_complex             = ippsRealToCplx_ ## suffix;         \
        static constexpr auto maximum                     = ippsMaxEvery_ ## suffix;           \
        static constexpr auto maximum_implace             = ippsMaxEvery_ ## suffix ## _I;     \
        static constexpr auto threshold_less_than         = ippsThreshold_LT_ ## suffix;       \
        static constexpr auto threshold_less_than_implace = ippsThreshold_LT_ ## suffix ## _I; \
        static constexpr auto log10                       = ippsLog10_ ## suffix ## _ ## prec; \
    };

MAKE_HELPER(Ipp32f, 32f, A24)
MAKE_HELPER(Ipp64f, 64f, A26)

#undef MAKE_HELPER


template<class T>
inline void maximum(const T* src1, const T* src2, T* dst, std::size_t len)
{
    IppRealTransformsHelper<T>::maximum(src1, src2, dst, len);
}

template<class T>
inline void maximum(const T* src, T* srcDst, std::size_t len)
{
    IppRealTransformsHelper<T>::maximum_implace(src, srcDst, len);
}

template<class T>
inline void real_to_complex(const T* real, const T* imag, Complex<T>* complexDst, std::size_t len)
{
    IppRealTransformsHelper<T>::real_to_complex(real, imag, complexDst, len);
}

template<class T>
inline void threshold_less_than(T level, const T* src, const T* dst, std::size_t len)
{
    IppRealTransformsHelper<T>::threshold_less_than(src, dst, len, level);
}

template<class T>
inline void threshold_less_than(T level, T* srcDst, std::size_t len)
{
    IppRealTransformsHelper<T>::threshold_less_than_implace(srcDst, len, level);
}

template<class T>
inline void log10(const T* src, T* dst, std::size_t len)
{
    IppRealTransformsHelper<T>::log10(src, dst, len);
}

template<class T>
inline void log10(T* srcDst, std::size_t len)
{
    IppRealTransformsHelper<T>::log10(srcDst, srcDst, len);
}
}
}
