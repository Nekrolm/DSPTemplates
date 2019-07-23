#pragma once

#include <ipps.h>
#include <cstdint>

namespace dsp_utils {
namespace ipp {
template<class T>
struct IppComplexHelper;


template<>
struct IppComplexHelper<float>
{
    using type      = Ipp32fc;
    using base_type = Ipp32f;
};

template<>
struct IppComplexHelper<double>
{
    using type      = Ipp64fc;
    using base_type = Ipp64f;
};


template<class T>
struct IppComplexBaseHelper;

template<>
struct IppComplexBaseHelper<Ipp32fc>
{
    using base_type = Ipp32f;
};

template<>
struct IppComplexBaseHelper<Ipp64fc>
{
    using base_type = Ipp64f;
};

template<>
struct IppComplexBaseHelper<Ipp32f>
{
    using base_type = Ipp32f;
};

template<>
struct IppComplexBaseHelper<Ipp64f>
{
    using base_type = Ipp64f;
};



template<class T>
using Complex = typename IppComplexHelper<T>::type;

template<class T>
using BaseType = typename IppComplexBaseHelper<T>::base_type;
}
}
