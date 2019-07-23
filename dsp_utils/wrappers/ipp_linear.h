#pragma once

#include "ipp_types.h"



namespace dsp_utils {
namespace ipp {
template<class T>
struct IppLinearHelper;

#define MAKE_HELPER(type_name, suffix) template<>                            \
    struct IppLinearHelper<type_name>                                        \
    {                                                                        \
        static constexpr auto set_value         = ippsSet_ ## suffix;        \
        static constexpr auto copy              = ippsCopy_ ## suffix;       \
        static constexpr auto zero              = ippsZero_ ## suffix;       \
        static constexpr auto add               = ippsAdd_ ## suffix;        \
        static constexpr auto add_implace       = ippsAdd_ ## suffix ## _I;  \
        static constexpr auto add_const         = ippsAddC_ ## suffix;       \
        static constexpr auto add_const_implace = ippsAddC_ ## suffix;       \
        static constexpr auto mul               = ippsMul_ ## suffix;        \
        static constexpr auto mul_implace       = ippsMul_ ## suffix ## _I;  \
        static constexpr auto mul_const         = ippsMulC_ ## suffix;       \
        static constexpr auto mul_const_implace = ippsMulC_ ## suffix ## _I; \
        static constexpr auto sub_const         = ippsSubC_ ## suffix;       \
        static constexpr auto sub_const_implace = ippsSubC_ ## suffix ## _I; \
    };


#define MAKE_HELPER_INTS(type_name, suffix) template<>         \
    struct IppLinearHelper<type_name>                          \
    {                                                          \
        static constexpr auto set_value = ippsSet_ ## suffix;  \
        static constexpr auto copy      = ippsCopy_ ## suffix; \
        static constexpr auto zero      = ippsZero_ ## suffix; \
    };



MAKE_HELPER(Ipp32f, 32f)
MAKE_HELPER(Ipp32fc, 32fc)
MAKE_HELPER(Ipp64f, 64f)
MAKE_HELPER(Ipp64fc, 64fc)


MAKE_HELPER_INTS(Ipp32s,  32s)
MAKE_HELPER_INTS(Ipp8u,   8u)
MAKE_HELPER_INTS(Ipp16s,  16s)
MAKE_HELPER_INTS(Ipp16sc, 16sc)




#undef MAKE_HELPER_INTS
#undef MAKE_HELPER

template<class T>
inline void copy(const T* from, T* to, std::size_t len)
{
    IppLinearHelper<T>::copy(from, to, len);
}

template<class T>
inline void zero(T* dst, std::size_t len)
{
    IppLinearHelper<T>::zero(dst, len);
}

template<class T>
inline void set_value(T value, T* dst, std::size_t len)
{
    IppLinearHelper<T>::set_value(value, dst, len);
}

template<class T>
inline void mul(const T* src1, const T* src2, T* dst, std::size_t len)
{
    IppLinearHelper<T>::mul(src1, src2, dst, len);
}

template<class T>
inline void mul(const T* src, T* srcDst,  std::size_t len)
{
    IppLinearHelper<T>::mul_implace(src, srcDst, len);
}

template<class T>
inline void mul_const(T value, const T* src, T* dst, std::size_t len)
{
    IppLinearHelper<T>::mul_const(src, value, dst, len);
}

template<class T>
inline void mul_const(T value, T* srcDst, std::size_t len)
{
    IppLinearHelper<T>::mul_const_implace(value, srcDst, len);
}

template<class T>
inline void add(const T* src1, const T* src2, T* dst, std::size_t len)
{
    IppLinearHelper<T>::add(src1, src2, dst, len);
}

template<class T>
inline void add(const T* src, T* srcDst, std::size_t len)
{
    IppLinearHelper<T>::add_implace(src, srcDst, len);
}

template<class T>
inline void add_const(T value, const T* src, T* dst, std::size_t len)
{
    IppLinearHelper<T>::add_const(src, value, dst, len);
}

template<class T>
inline void add_const(T value, T* srcDst, std::size_t len)
{
    IppLinearHelper<T>::add_const_implace(value, srcDst, len);
}


template<class T>
inline void sub_const(T value, const T* src, T* dst, std::size_t len)
{
    IppLinearHelper<T>::sub_const(src, value, dst, len);
}

template<class T>
inline void sub_const(T value, T* srcDst, std::size_t len)
{
    IppLinearHelper<T>::sub_const_implace(value, srcDst, len);
}



}
}
