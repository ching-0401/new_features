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

#include <doctest/doctest.h>

#include "thread_pool.h"

TEST_CASE("promise_packaged_task_async_futures")
{
	auto res = yuki_new_features__thread_test::promiseFutureTest();
	CHECK_EQ(1, res);

	auto res2 = yuki_new_features__thread_test::packagedTaskTest();
	CHECK_EQ(1, res2);

	auto res3 = yuki_new_features__thread_test::asyncTest(std::launch::async);
	CHECK_EQ(1, res3);
}

TEST_CASE("ThreadPoolFunction")
{
	{
		std::vector<std::future<int>> v;
		yuki_new_features__thread_test::ThreadPoolFunction tp(4);
		for (size_t i = 0; i < 10; ++i) {
			v.emplace_back(tp.enqueue(i));
		}
		static int t = 0;
		for (auto&& i : v) {
			CHECK_EQ(i.get(), t * 2);
			t++;
		}
	}
}

TEST_CASE("ThreadPoolTestPromise")
{
	{
		yuki_new_features__thread_test::ThreadPoolTestPromise tp(4);
		std::vector<std::future<int>> v;
		for (size_t i = 0; i < 10; ++i) {
			std::promise<int> p;
			v.emplace_back(tp.pushTask(std::move(p)));
		}
		static int t = 0;
		for (auto&& i : v) {
			CHECK_EQ(i.get(), 4);
			t++;
		}
	}
}