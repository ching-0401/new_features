/**
******************************************************************************
* @file           : main.cpp
* @author         : tsing
* @brief          : None
* @attention      : None
* @date           : 22-1-31
* @email          : tsing0401@outlook.com
******************************************************************************
*/

#include <bits/stdc++.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

#include "associative_container.h"  // to do
#include "template_test.h"          // to do
#include "thread_pool.h"            // done

using namespace std;

int main()
{
	srand(time(0));

	/***
	// template specialization test & type traits test
	yuki_new_features__template_test::Test<int, int> template_t_1;
	template_t_1.show(1, 2);

	yuki_new_features__template_test::Test<int, std::string> template_t_2;
	template_t_2.show(1, "2");

	yuki_new_features__template_test::Test<std::string, std::string>
template_t_3; template_t_3.show("1", "2");

	yuki_new_features__template_test::TraitsTest<yuki_new_features__template_test::PODStruct>
template_t_4;
//    template_t_4.show()

	std::cout << "\n\n\n\n\n";
	std::cout.flush();


	// thread test
	{
//       yuki_new_features__thread_test::promiseFutureTest();

//       yuki_new_features__thread_test::packagedtaskTest();


	   yuki_new_features__thread_test::asyncTest(std::launch::async);

//       yuki_new_features__thread_test::asyncTest(std::launch::deferred);
	}
	*****/

	//    yuki_new_features::ThreadPool pool(4);
	//    std::vector< std::future<int> > results;
	////    std::vector< std::future<std::pair<int, int>> > results;

	//    for(int i = 0; i < 8; ++i) {
	//        results.emplace_back(
	//            pool.enqueue([i] {
	////                std::cout << "hello " << i << std::endl;
	//                std::this_thread::sleep_for(std::chrono::seconds(10 - i));
	////                std::cout << "world " << i << std::endl;
	////                return std::make_pair(i, i*i);
	//                return i * i;
	//            })
	//        );
	//    }

	//    for(auto&& res: results){
	////        std::cout << res.get().first << "\t\t" << res.get().second <<
	///std::endl;
	//        std::cout << res.get() << "\n";
	//    }

	//    std::chrono::system_clock::now();

	//    std::unordered_multimap<int, int> m;
	//    m.insert({1, 0});
	//    m.insert({1, 1});

	//    m.insert({2, 4});
	//    for(size_t i = 0; i < m.bucket_count(); ++i){
	//        for(auto local_iter = m.begin(i); local_iter != m.end(i);
	//        ++local_iter){
	//            std::cout << local_iter->first << '\t' << local_iter->second
	//            << std::endl;
	//        }
	//    }

	//    std::mutex mtx;
	//    mtx.lock();
	//    mtx.unlock();

	//    std::cout << std::endl << std::endl << std::endl ;
	//    std::unordered_map<int, int> um;

	//    um.insert({1, 3});
	//    um.insert({2, 4});
	//    auto it = um.find(1);
	//    std::cout << it->second << std::endl;

	//    auto f = []()->int {int a = 0; int b = 0; return 0;};

	//    std::cout << f() << std::endl;

	//    cout << "thread num: " <<  std::thread::hardware_concurrency() <<
	//    std::endl;

	//    return 0;
}
