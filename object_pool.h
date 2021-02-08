//
// Created by wutao on 2020/12/31.
//

#ifndef TIME_SERIES_SERVER_OBJECT_POOL_H
#define TIME_SERIES_SERVER_OBJECT_POOL_H

#pragma once
#include <string>
#include <deque>
#include <mutex>
#include <memory>
#include <utility>
#include <iostream>

// 去掉了make_unique生产unique_ptr的方法，改为使用专门的Builder生成指针，然后转为unique_ptr
#if 0
#include "make_unique.h"

namespace wrapper
{

#if (__cplusplus >= 201402L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
    using std::make_unique;
#else
    using data_srv::make_unique;
#endif
};
#endif

class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    noncopyable(noncopyable&&) = delete;
    void operator=(const noncopyable&) = delete;
    void operator=(noncopyable&&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

template<typename T, typename B, size_t MIN, size_t MAX>
class object_pool final: noncopyable {
public:
    using DeleterType = std::function<void(T*)>;

    static object_pool & get()
    {
        static object_pool<T, B, MIN, MAX> pool;
        return pool;
    }

    std::unique_ptr<T, DeleterType> acquire() {
        std::unique_lock<std::mutex> lock(_wait_mutex);
        if (pool_.empty()) {
            if(_total_num < MAX)
            {
                pool_.push_back(std::unique_ptr<T>(b_->build()));
                _total_num++;
            }
            else
            {
                return nullptr;
            }
        }

        std::unique_ptr<T, DeleterType> ptr(pool_.front().release(), [this](T* t) {
            std::unique_lock<std::mutex> lock_(_push_mutex);
            pool_.push_back(std::unique_ptr<T>(t));
        });

        pool_.pop_front();
        return ptr;
    }

    bool empty() const {
        return pool_.empty();
    }

    size_t size() const {
        return pool_.size();
    }

    void Init(std::unique_ptr<B> build)
    {
        std::unique_lock<std::mutex> lock(_wait_mutex);
        b_ = std::move(build);
        if(_total_num != 0)// 只能初始化一次
        {
            std::cout << "Initialization completed !" << std::endl;
            return ;
        }
        for (size_t i = 0; i < MIN; i++) {
            pool_.push_back(std::unique_ptr<T>(b_->build()));
        }
        _total_num = MIN; // 所有已创建的对象个数，最小值为MIN，最大值为MAX，达到最大值之后维持不变
        return;
    }

private:
    object_pool() {
        static_assert(MAX < 500, "MAX size is out of range(500)");
        static_assert(MIN < MAX, "MIN size is bigger than MAX");
    }

    std::deque<std::unique_ptr<T>> pool_;

    size_t _total_num = 0;

    std::mutex _wait_mutex;
    std::mutex _push_mutex;

    std::unique_ptr<B> b_;
};

#endif //TIME_SERIES_SERVER_OBJECT_POOL_H
