//
// Created by wutao on 2021/1/4.
//

#ifndef TIME_SERIES_SERVER_STRING_FORMAT_H
#define TIME_SERIES_SERVER_STRING_FORMAT_H

#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include <libgen.h>

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

#define debug_print(fmt, ...) \
        do { fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#endif //TIME_SERIES_SERVER_STRING_FORMAT_H
