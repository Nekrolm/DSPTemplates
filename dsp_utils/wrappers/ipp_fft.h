#pragma once

#include "ipp_types.h"
#include "ipp_alloc.h"

#include <algorithm>

namespace dsp_utils {

namespace ipp {

enum FFTNormMode
{
    NORM_NONE      = IPP_FFT_NODIV_BY_ANY,
    NORM_FORWARD   = IPP_FFT_DIV_FWD_BY_N,
    NORM_BACKWARD  = IPP_FFT_DIV_INV_BY_N,
    NORM_BOTH_SQRT = IPP_FFT_DIV_BY_SQRTN
};

template<class T>
struct FFTHelper;


template<>
struct FFTHelper<Ipp32fc>
{
    using spec_type = IppsFFTSpec_C_32fc;

    static constexpr auto get_buff_size   = ippsFFTGetSize_C_32fc;
    static constexpr auto init            = ippsFFTInit_C_32fc;
    static constexpr auto forward         = ippsFFTFwd_CToC_32fc;
    static constexpr auto forward_implace = ippsFFTFwd_CToC_32fc_I;

    static constexpr auto backward         = ippsFFTInv_CToC_32fc;
    static constexpr auto backward_implace = ippsFFTInv_CToC_32fc_I;
};


template<>
struct FFTHelper<Ipp64fc>
{
    using spec_type = IppsFFTSpec_C_64fc;

    static constexpr auto get_buff_size   = ippsFFTGetSize_C_64fc;
    static constexpr auto init            = ippsFFTInit_C_64fc;
    static constexpr auto forward         = ippsFFTFwd_CToC_64fc;
    static constexpr auto forward_implace = ippsFFTFwd_CToC_64fc_I;

    static constexpr auto backward         = ippsFFTInv_CToC_64fc;
    static constexpr auto backward_implace = ippsFFTInv_CToC_64fc_I;
};

inline size_t fft_order_ceil(size_t size)
{
    size_t order = 0;
    size_t x     = 1;

    while (x < size)
    {
        order++;
        x <<= 1;
    }
    return order;
}

inline size_t fft_order_floor(size_t size)
{
    size_t order = 0;
    size_t x     = 1;

    while (x * 2 <= size)
    {
        order++;
        x <<= 1;
    }
    return order;
}

template<class T>
inline void fft_shift(T* srcDst, size_t size)
{
    std::rotate(srcDst, srcDst + size / 2, srcDst + size);
}

template<class T>
class FFT
{
public:
    using SamplesT                          = ipp::Complex<T>;
    FFT(size_t order = 10, FFTNormMode norm = NORM_NONE) :
        order_(order)
    {
        int specdata_sz, specbuff_sz, workbuff_sz;

        ipp::FFTHelper<SamplesT>::get_buff_size(order_,
                                                norm,
                                                ippAlgHintFast,
                                                &specdata_sz,
                                                &specbuff_sz,
                                                &workbuff_sz);


        workBuff_size_ = workbuff_sz;
        specData_      = allocate_managed_shared<Ipp8u>(specdata_sz);
        specBuff_      = allocate_managed_shared<Ipp8u>(specbuff_sz);
        workBuff_      = allocate_managed<Ipp8u>(workBuff_size_);



        ipp::FFTHelper<SamplesT>::init(&pFFTSpec_, order_,
                                       norm,
                                       ippAlgHintFast, specData_.get(), specBuff_.get());
    }

    ~FFT()     = default;
    FFT(FFT&&) = default;
    FFT(const FFT& other)
    {
        order_         = other.order_;
        workBuff_size_ = other.workBuff_size_;
        specBuff_      = other.specBuff_;
        workBuff_      = allocate_managed<Ipp8u>(workBuff_size_);

        // shared buffers -- same pointer!!!
        pFFTSpec_ = other.pFFTSpec_;
    }

    FFT& operator=(FFT&&) = default;
    FFT& operator=(const FFT& other)
    {
        FFT tmp(other);

        *this = std::move(tmp);
        return *this;
    }

    void forward(const ipp::Complex<T>* src, ipp::Complex<T>* dst)
    {
        FFTHelper<SamplesT>::forward(src, dst, pFFTSpec_, workBuff_.get());
    }

    void forward(ipp::Complex<T>* srcDst)
    {
        FFTHelper<SamplesT>::forward_implace(srcDst, pFFTSpec_, workBuff_.get());
    }

    void backward(const ipp::Complex<T>* src, ipp::Complex<T>* dst)
    {
        FFTHelper<SamplesT>::backward(src, dst, pFFTSpec_, workBuff_.get());
    }

    void backward(ipp::Complex<T>* srcDst)
    {
        FFTHelper<SamplesT>::backward_implace(srcDst, pFFTSpec_, workBuff_.get());
    }

    inline size_t size() const { return 1ll << order_; }

private:

    size_t order_;
    size_t workBuff_size_;
    managed_sequence_ptr<Ipp8u> workBuff_              = nullptr;
    std::shared_ptr<Ipp8u> specBuff_                   = nullptr;
    std::shared_ptr<Ipp8u> specData_                   = nullptr;
    typename FFTHelper<SamplesT>::spec_type* pFFTSpec_ = nullptr;
};

}

}
