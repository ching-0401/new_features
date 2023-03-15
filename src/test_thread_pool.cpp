/**
 ******************************************************************************
 * @file           : test_thread_pool.cpp
 * @author         : tsing
 * @brief          : None
 * @attention      : None
 * @date           : 23-3-9
 * @email          : tsing0401@outlook.com
 ******************************************************************************
 */

#include "thread_pool.h"
#include <doctest/doctest.h>

TEST_CASE("promise_packaged_task_async_futures")
{
	yuki_new_features__thread_test::promiseFutureTest();
	CHECK_EQ(1, 1);

	yuki_new_features__thread_test::packagedtaskTest();
	CHECK_EQ(1, 1);

	yuki_new_features__thread_test::asyncTest(std::launch::async);
	CHECK_EQ(1, 1);

}

TEST_CASE("thread_pool test")
{
	{
		std::vector<std::future<int>> v;
		yuki_new_features__thread_test::ThreadPoolFunction tp(4);
		for(size_t i = 0; i < 10; ++i){
			v.emplace_back(tp.enqueue(i));
		}
		static int t = 0;
		for(auto&& i: v){
			CHECK_EQ(i.get(), t * 2);
			t++;
		}
	}


}