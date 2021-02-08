//
// Created by wutao on 2021/1/4.
//

#ifndef TIME_SERIES_SERVER_BUILDER_H
#define TIME_SERIES_SERVER_BUILDER_H

#include <iostream>
#include <string>
#include <memory>
#include <tuple>

template<typename T>
class Builder
{
public:
    virtual ~Builder(){}
    virtual T * build() = 0;
};

template<int ...>
struct seq { };

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...> { };

template<int ...S>
struct gens<0, S...> {
    typedef seq<S...> type;
};

template<typename T, class ...Args>
class RealBuilder: public Builder<T>
{
public:
    RealBuilder(Args&&... args_)
            :params_(std::forward<Args>(args_)...)
    {}

    template<int... S>
    T* create(seq<S...>)
    {
        return new T(std::get<S>(params_) ...);
    }

    T* build()
    {
        return create(typename gens<sizeof...(Args)>::type());
    }

private:
    RealBuilder(const RealBuilder&)=delete;
    void operator=(const RealBuilder&)=delete;
    std::tuple<typename std::decay<Args>::type...> params_;
};

template<class... Args>
unique_ptr<Builder<PowerClient>> make_connection_builder(Args&&... args)
{
    return unique_ptr<Builder<PowerClient>>(new RealBuilder<PowerClient, Args...>(std::forward<Args>(args)...));
}

#endif // TIME_SERIES_SERVER_BUILDER_H