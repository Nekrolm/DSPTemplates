#pragma once

#include "ipp_types.h"

#include <memory>
#include <functional>

namespace dsp_utils
{
namespace ipp {

template <class T>
struct IppAllocHelper;

#define MAKE_HELPER(type_name, suffix) template<> \
    struct IppAllocHelper<type_name> {\
        static constexpr auto alloc = ippsMalloc_##suffix;\
    };

MAKE_HELPER(Ipp32f, 32f)
MAKE_HELPER(Ipp32fc, 32fc)
MAKE_HELPER(Ipp64f, 64f)
MAKE_HELPER(Ipp64fc, 32fc)

MAKE_HELPER(Ipp16sc, 16sc)
MAKE_HELPER(Ipp8u, 8u)
MAKE_HELPER(Ipp32u, 32u)




#undef MAKE_HELPER


template <class T>
using managed_sequence_ptr = std::unique_ptr<T[], std::function<void(T*)>>;


template <class T>
T* allocate(std::size_t count){
    return IppAllocHelper<T>::alloc(count);
}


template <class T>
managed_sequence_ptr<T> allocate_managed(std::size_t count)
{
    return managed_sequence_ptr<T>(allocate<T>(count), ippsFree);
}

template <class T>
std::shared_ptr<T> allocate_managed_shared(std::size_t count)
{
    return std::shared_ptr<T>(allocate<T>(count), ippsFree);
}




}

}
