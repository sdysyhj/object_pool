//
// Created by wutao on 2020/12/31.
//

#ifndef TIME_SERIES_SERVER_MAKE_UNIQUE_H
#define TIME_SERIES_SERVER_MAKE_UNIQUE_H

namespace data_srv
{
    template<typename T, typename... Ts>
    std::unique_ptr<T> make_unique(Ts&&... params)
    {
        return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
    }
}

#endif //TIME_SERIES_SERVER_MAKE_UNIQUE_H
