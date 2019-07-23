#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <type_traits>
#include <utility>
#include <algorithm>
#include <complex>

namespace statistics
{

template <class T>
class Gaussian {

public:
    Gaussian(const T& mean = T(0), double variance = 1, double alpha = 1):
        mean_(mean), variance_(variance), alpha_(alpha)
    {}

    double proba(const T& val) const {
        auto dx = std::abs(val - mean_);
        dx *= dx;
        return std::exp(-dx / variance_ / 2) / sqrt(2 * M_PI * variance_) * alpha_;
    }

    inline T& mean() { return mean_; }
    inline T mean() const {return mean_;}

    inline double& variance() { return variance_; }
    inline double variance() const {return variance_;}

    inline double& alpha() {return alpha_;}
    inline double alpha() const {return alpha_;}


private:
    std::enable_if_t<std::is_arithmetic_v<T>, T> mean_;
    double variance_;
    double alpha_;
};

}

#endif // GAUSSIAN_H
