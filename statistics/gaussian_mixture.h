#ifndef GAUSSIAN_MIXTURE_H
#define GAUSSIAN_MIXTURE_H

#pragma once

#include "gaussian.h"
#include <vector>
#include <numeric>


namespace statistics{

template <class T>
class GaussianMixture
{
public:
    GaussianMixture(const std::vector<Gaussian<T>>& gaussians){
        gaussians_ = gaussians;
        normalize();
    }

    double proba(const T& val) const {
        double p = 0;
        for (auto& g : gaussians_)
            p += g.proba(val);
        return p;
    }

    void clear(){
        gaussians_.clear();
    }

    void add(Gaussian<T>&& g){
        gaussians_.emplace_back(std::forward<Gaussian<T>>(g));
    }

    const Gaussian<T>& get(size_t i)const {
        return gaussians_.at(i);
    }


    void normalize(){
        double sum = 0;
        for (auto& g : gaussians_)
            sum += g.alpha();

        for (auto& g : gaussians_)
            g.alpha() /= sum;
    }

    template<class cntType>
    void recalc(const std::vector<T>& val, const std::vector<cntType>& cnt){
        recalc_internal(val, cnt);
    }
    void recalc(const std::vector<T>& val)
    {
        recalc_internal(val, std::vector<uint32_t>(val.size(), 1));
    }



private:

    template<class cntType>
    void recalc_internal(const std::vector<T>& val, const std::vector<cntType>& cnt){
        static_assert (std::is_integral_v<cntType>, "values counts must be integers!");

        if (val.size() != cnt.size()){
            throw std::range_error("cnt.size() != val.size()");
        }

        std::vector<double> vproba(val.size());
        std::transform(begin(val), end(val), begin(vproba),
                       [this](auto x){return proba(x);});


        std::vector<std::vector<double>> W;
        std::vector<double> Ng;

        for (auto&& g : gaussians_){
            W.push_back({});
            double sum = 0;
            auto& Wline = W.back();
            for (size_t i = 0; i < val.size(); ++i)
            {
                Wline.emplace_back( cnt[i] * g.proba(val[i]) / vproba[i]  );
                sum += Wline.back();
            }
            Ng.emplace_back(sum);
        }

        cntType N = std::accumulate(begin(cnt), end(cnt), cntType(0));

        for (size_t i = 0; i < gaussians_.size(); ++i){
            gaussians_[i].alpha() = Ng[i] / N;

            T mean = std::inner_product(begin(W[i]), end(W[i]), begin(val), T(0)) / Ng[i];
            gaussians_[i].mean() = mean;



            gaussians_[i].variance() = std::inner_product(
                        begin(W[i]), end(W[i]), begin(val), 0.,
                        std::plus<double>(),
                        [mean](double w, const T& x)->double{
                               auto AX = std::abs<T>(x - mean);
                               return w * AX * AX;
                        }) / Ng[i];
        }

    }


    std::vector<Gaussian<T>> gaussians_;
};


}




#endif // GAUSSIAN_MIXTURE_H
