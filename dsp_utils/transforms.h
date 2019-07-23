#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#pragma once

#include "signal_types.h"

#include <algorithm>
#include <iostream>
#include <numeric>


namespace dsp_utils {


inline float normalize_angle(float ang){
    ang = std::fmod(ang, 360.f);
    if (ang < 0) ang += 360;
    return ang;
}

inline double normalize_angle(double ang){
    ang = std::fmod(ang, 360.);
    if (ang < 0) ang += 360;
    return ang;
}

inline int normalize_angle(int ang){
    ang %= 360;
    if (ang < 0) ang += 360;
    return ang;
}

template <class T>
inline auto angle_distance(T from, T to){
    return std::min(normalize_angle(from - to), normalize_angle(to - from));
}


template<class T>
int64_t signum(T&& val){
    static_assert (std::is_integral<T>::value or std::is_floating_point<T>::value,
                   "Signum supported only for integers & floating points!");
    return val < T(0) ? -1 : (val > T(0) ? 1 : 0);
}





template <class T>
std::vector<T> normalize(const std::vector<T>& sig, double norm = 1)
{
    if (sig.size() == 0)
        return {};

    double ABS = std::abs(*max_element(begin(sig), end(sig), [](auto&& x, auto&& y){return abs(x) < abs(y);}));

    std::vector<T> ret(sig.size());
    transform(begin(sig), end(sig), begin(ret),
              [ABS, norm](const T& x)->T{return x / T(ABS) * T(norm);});

    return ret;
}


template <class T>
std::vector<T> sample_down(const std::vector<T>& sig, int dec = 1, int start_phase = 0)
{
    std::vector<T> ret; ret.reserve(sig.size() / dec);
    for (size_t pos = start_phase; pos < sig.size(); pos += dec)
        ret.push_back(sig[pos]);
    return ret;
}



template<class T>
std::vector<T> running_mean(const std::vector<T>& sig, size_t smooth_cnt)
{
    if (sig.size() < smooth_cnt)
        return {};
    std::vector<T> ret(0);

    std::cerr << sig.size() - smooth_cnt + 1 << std::endl;

    ret.reserve(sig.size() - smooth_cnt + 1);

    T sum {0};

    for (size_t i = 0; i < smooth_cnt; ++i)
        sum += sig[i];

    for (size_t i = smooth_cnt; i < sig.size(); ++i)
    {
        ret.push_back(sum / smooth_cnt);
        sum += sig[i];
        sum -= sig[i-smooth_cnt];
    }

    return ret;

}





template<class T, class FuncT>
auto map_sequence(const std::vector<T>& sig, FuncT f){
//    static_assert (std::is_invocable<FuncT, T>::value, "sequenct can't map! types mismatch");
    std::vector<typename std::result_of_t<FuncT(T)>> ret;
    ret.reserve(sig.size());
    for (auto&& v : sig){
        ret.push_back(f(v));
    }
    return ret;
}



template <class T>
std::tuple<std::vector<uint64_t>, T, T> histogram_impl(const std::vector<T>& sig, size_t bins)
{
    static_assert (is_real_v<T>, "only real signals supported!");
    auto min_max_it = std::minmax_element(begin(sig), end(sig));

    auto min_i = min_max_it.first;
    auto max_i = min_max_it.second;

    auto step = static_cast<double>(*max_i - *min_i) / bins;

    std::vector<uint64_t> ret(bins + 1, 0);

    for (T x : sig){
        ret[size_t((x - *min_i) / step)]++;
    }

    return {ret, *min_i, *max_i};
}


//template <class T>
//auto histogram(T&& s, size_t bins){
//    return histogram_impl<typename std::remove_reference_t<decltype(s)>::value_type >(s,bins);
//}


template <class T>
auto median(const std::vector<T>& sig, size_t skip_first = 0, size_t skip_last = 0)
{
    std::vector<T> tmp(sig.size());
    std::copy(begin(sig), end(sig), begin(tmp));
    std::sort(begin(tmp), end(tmp));

    size_t rest_size = sig.size() - skip_first - skip_last;
    size_t I = rest_size / 2 + skip_first;
    return tmp.at(I);
}





//template <class T>
//auto median(T&& s, size_t skip_first = 0, size_t skip_last = 0){
//    return median_impl<typename std::remove_reference_t<decltype(s)>::value_type >(s, skip_first, skip_last);
//}

inline int fft_order_floor(uint64_t size){
    int ret = 0;
    while (1ull << (ret + 1) <= size) ++ret;
    return ret;
}



template <class T>
std::vector<T> fftshift(const std::vector<T>& x){
    std::vector<T> ret(x.size());
    std::rotate_copy(begin(x), begin(x) + x.size() / 2, end(x), begin(ret));
    return ret;
}

template <class Tout, class InIter>
std::vector<Tout> abs(InIter begin, InIter end){
    std::vector<Tout> ret;
    std::transform(begin, end, back_inserter(ret), [](auto&& x)->Tout{return std::abs<Tout>(x);});
    return ret;
}


template <class T>
std::vector<T> clip(const std::vector<T>& s, T lo, T hi){
    static_assert (is_real_v<T>, "only real signal supported");
    std::vector<T> ret(s.size());
    std::transform(begin(s), end(s), begin(ret), [lo, hi](auto&& x){return std::min(std::max(x, lo), hi);});
    return ret;
}





template <class T>
int64_t argmax(const std::vector<T>& s){
    return std::max_element(begin(s), end(s)) - begin(s);
}






template <class T>
struct IppCorr;

template <>
struct IppCorr<Ipp32fc>{
    static constexpr auto correlate = ippsCrossCorrNorm_32fc;
    static constexpr auto data_type = ipp32fc;

};


template <>
struct IppCorr<Ipp64fc>{
    static constexpr auto correlate = ippsCrossCorrNorm_64fc;
    static constexpr auto data_type = ipp64fc;
};


template <>
struct IppCorr<float>{
    static constexpr auto correlate = ippsCrossCorrNorm_32f;
    static constexpr auto data_type = ipp32f;
};


template <>
struct IppCorr<double>{
    static constexpr auto correlate = ippsCrossCorrNorm_64f;
    static constexpr auto data_type = ipp64f;
};



template <class T>
T mean(const std::vector<T>& s){
    if (s.empty())
        return T(0);
    return std::accumulate(begin(s), end(s), T(0)) / s.size();
}

template <class T>
double var(const std::vector<T>& s){
    auto M = mean(s);
    double ret = 0;
    for (auto x : s){
        double dx = std::abs(x - M);
        ret += dx * dx;
    }
    return ret / s.size();
}


template <class T>
std::vector<T> norm(std::vector<T> s){
    auto M = mean(s);
    auto stdV = std::sqrt(var(s));
    for (auto& x : s){
        x -= M;
        x /= stdV;
    }
    return s;
}


inline std::vector<Ipp32fc> norm(std::vector<Ipp32fc> s, IppHintAlgorithm alg = ippAlgHintFast){
    Ipp32fc M;
    ippsMean_32fc(s.data(), s.size(), &M, alg);
    ippsSubC_32fc_I(M, s.data(), s.size());

    double var = 0;
    for (auto x : s){
        var += x.re * x.re + x.im * x.im;
    }
    var /= s.size();

    ippsDivC_32f_I(float(std::sqrt(var)),
                   reinterpret_cast<float*>(s.data()),
                   s.size() * 2);

    return s;
}




template <class T>
std::vector<T> correlate(const std::vector<T>& a, const std::vector<T>& b, size_t corr_size, long low_lag=0,
                    IppEnum flags = ippAlgFFT | ippsNormNone)
{
    int buff_sz = 0;
    ippsCrossCorrNormGetBufferSize(a.size(), b.size(), corr_size, low_lag,
                                   IppCorr<T>::data_type, flags, &buff_sz);

    std::vector<Ipp8u> buffer(buff_sz);

    std::vector<T> ret(corr_size);

    IppCorr<T>::correlate(a.data(), a.size(), b.data(), b.size(), ret.data(), ret.size(), low_lag, flags, buffer.data());

    return ret;

}



template <class T>
inline T mean_index(const T* src, size_t size)
{
    static_assert (std::is_floating_point<T>::value || std::is_integral<T>::value, "not supported type!");

    T sum {0};
    T idx_sum {0};
    for (size_t i = 0; i < size; ++i){
        sum += src[i];
        idx_sum += i * src[i];
    }
    return std::abs(sum) > 0 ? idx_sum / sum : 0;
}



}











#endif // TRANSFORMS_H
