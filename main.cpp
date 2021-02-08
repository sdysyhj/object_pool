//
// Created by wutao on 2020/12/31.
//

#include <iostream>
#include <gtest/gtest.h>
#include <tuple>
#include <thread>

#include <cstdlib>

#include "object_pool.h"
#include "builder.h"

using namespace std;

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

struct connection {
    std::string id;
    connection(std::string id):id(id)
    {
        cout << BOLDBLUE << "construct" << RESET << endl;
    }
    ~connection()
    {
        cout << BOLDYELLOW << "deconstruct" << RESET << endl;
    }
};

template<class... Args>
unique_ptr<Builder<connection>> make_connection_builder(Args&&... args)
{
    return unique_ptr<Builder<connection>>(new RealBuilder<connection, Args...>(std::forward<Args>(args)...));
}

auto& pool = object_pool<connection, Builder<connection>, 10, 50>::get();

TEST(OBJECT_POOL_TEST, acquire)
{
    string id = "21";
    pool.Init(make_connection_builder(id));

    {
        auto conn = pool.acquire();
        EXPECT_EQ(pool.size(), 9);
        cout << BOLDGREEN << "conn's id:" << conn->id << RESET << endl;
    }

    EXPECT_EQ(pool.size(), 10);

    auto conn = pool.acquire();
    conn.reset();
    EXPECT_EQ(pool.size(), 10);
}

void th_function()
{
    auto conn = pool.acquire();
    if(conn != nullptr)
    {
        cout << BOLDGREEN << "conn's id:" << conn->id << RESET << endl;
    }
    else
    {
        cout << BOLDRED << "conn is empty" << RESET << endl;
    }
    usleep(2000);
}

TEST(OBJECT_POOL_TEST, multithread)
{
    std::thread threads[200];
    for(auto &t1: threads)
    {
        t1 = std::thread(th_function);
    }

    for(auto &t1: threads)
    {
        t1.join();
    }
}

int main(int argc, char **argv) {
    try {
        testing::InitGoogleTest(&argc, argv);//将命令行参数传递给gtest
        return RUN_ALL_TESTS();   //RUN_ALL_TESTS()运行所有测试案例
    }
    catch (std::exception& e)
    {
        cout << BOLDRED << e.what() << RESET << endl;
    }

    return 0;
}
