#include "thread_pool.h"

#include <atomic>
#include <chrono>
#include <future>

namespace yuki_new_features__thread_test
{
	// test promise & future & packaged_task & async
	int promiseFutureTest()
	{
		auto f = [](std::promise<std::int32_t>& a_p, std::mutex& a_m) {
			std::lock_guard l(a_m);
			a_p.set_value(1);
		};
		std::mutex m;
		std::promise<std::int32_t> p;
		std::future<std::int32_t> futu = p.get_future();

		std::thread t(f, std::ref(p), std::ref(m));
		t.join();

		return futu.get();
	}

	int packagedTaskTest()
	{
		auto f = [](int a_m, int a_n) { return a_m + a_n; };
		std::packaged_task<int(int, int)> task(f);
		std::future<int> res = task.get_future();
		std::thread t(std::move(task), 1, 0);
		t.join();
		return res.get();
	}

	int asyncTest(std::launch a_enum)
	{
		auto f = [](int a_m, int a_n) { return a_m > a_n ? a_m : a_n; };
		std::future<int> ff = std::async(a_enum, f, 1, 0);

		std::chrono::duration<int, std::ratio<1>> m3(1);
		std::this_thread::sleep_for(m3);

		return ff.get();
	}

}  // namespace yuki_new_features__thread_test

namespace yuki_new_features__thread_test
{
	// test thread pool by promise
	ThreadPoolTestPromise::ThreadPoolTestPromise(size_t a_thread_num)
		: m_stop(false)
	{
		for (size_t i = 0; i < a_thread_num; ++i) {
			m_thread_pool.emplace_back([this]() {
				while (1) {
					{
						std::unique_lock<std::mutex> ul(this->m_mutex);
						this->m_cond_varia.wait(ul, [this]() {
							return !this->m_task_queue.empty() || m_stop;
						});
						if (m_stop) {
							break;
						}
						auto t = std::move(m_task_queue.front());
						t.set_value(2 * 2);
						m_task_queue.pop();
					}
				}
			});
		}
	}

	std::future<int> ThreadPoolTestPromise::pushTask(std::promise<int>&& a_p)
	{
		std::future<int> ff = a_p.get_future();
		{
			std::unique_lock<std::mutex> ul(m_mutex);
			m_task_queue.push(std::move(a_p));
		}
		m_cond_varia.notify_one();
		return ff;
	}

	ThreadPoolTestPromise::~ThreadPoolTestPromise()
	{
		{
			std::unique_lock<std::mutex> ul(m_mutex);
			m_stop = true;
		}
		m_cond_varia.notify_all();
		for (auto&& i : m_thread_pool) {
			i.join();
		}
	}

}  // namespace yuki_new_features__thread_test

namespace yuki_new_features__thread_test
{
	// test thread pool by enclosure std::function
	ThreadPoolFunction::ThreadPoolFunction(size_t a_threads_num) : m_stop(false)
	{
		for (size_t i = 0; i < a_threads_num; ++i) {
			m_threads.emplace_back([this]() {
				while (1) {
					std::function<void()> f;
					{
						std::unique_lock<std::mutex> ul(this->m_mutex);
						this->m_cv.wait(ul, [this]() {
							return !this->m_queue.empty() || this->m_stop;
						});
						if (this->m_stop) {
							break;
						}
						f = std::move(this->m_queue.front());
						this->m_queue.pop();
					}
					f();
				}
			});
		}
	}

	std::future<int> ThreadPoolFunction::enqueue(int a_n)
	{
		auto task = std::make_shared<std::packaged_task<int()>>(
			std::packaged_task<int()>([a_n]() { return a_n * 2; }));
		std::future<int> res = task->get_future();
		{
			std::unique_lock<std::mutex> lg(m_mutex);
			m_queue.emplace([task]() { (*task)(); });
		}
		m_cv.notify_one();
		return res;
	}

	ThreadPoolFunction::~ThreadPoolFunction()
	{
		{
			std::unique_lock<std::mutex> ul(m_mutex);
			m_stop = true;
		}
		m_cv.notify_all();
		for (auto& i : m_threads) {
			i.join();
		}
	}

}  // namespace yuki_new_features__thread_test