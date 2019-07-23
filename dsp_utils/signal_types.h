#ifndef SIGNAL_TYPES_H
#define SIGNAL_TYPES_H

#pragma once

#include <complex>
#include <vector>
#include <type_traits>

#include <ipp.h>

namespace dsp_utils {

using  complex_32f = std::complex<Ipp32f>;
using  complex_64f = std::complex<Ipp64f>;



template <class T>
struct is_complex : std::false_type {};

template  <class T>
struct is_complex<std::complex<T>> : std::true_type {};

template <>
struct is_complex<Ipp32fc> : std::true_type {};

template <>
struct is_complex<Ipp64fc> : std::true_type {};




template <class T>
constexpr bool is_complex_v = is_complex<T>::value;

template <class T>
constexpr bool is_real_v = std::is_integral<T>::value || std::is_floating_point<T>::value;




}



#endif // SIGNAL_TYPES_H
