/**
******************************************************************************
* author:      yuki
* data:        2022.10.08
* email:       tsing0401@outlook.com
******************************************************************************
*/

#ifndef YUKI_NEW_FEATURES_THREAD_POOL_H__
#define YUKI_NEW_FEATURES_THREAD_POOL_H__

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace yuki_new_features__thread_test
{
	// promise & future & packaged_task & async
	int promiseFutureTest();
	int packagedTaskTest();
	int asyncTest(std::launch a_enum);

	// thread pool by promise
	class ThreadPoolTestPromise
	{
	  public:
		ThreadPoolTestPromise(size_t a_thread_num);
		std::future<int> pushTask(std::promise<int>&&);
		~ThreadPoolTestPromise();

	  private:
		std::queue<std::promise<int>> m_task_queue;
		std::atomic<bool> m_stop;
		std::vector<std::thread> m_thread_pool;
		std::condition_variable m_cond_varia;
		std::mutex m_mutex;
	};

	// thread pool by packaged_task  enclosure by std::function
	class ThreadPoolFunction
	{
	  public:
		ThreadPoolFunction(size_t a_threads_num);
		std::future<int> enqueue(int a_n);

		~ThreadPoolFunction();

	  private:
		std::atomic<bool> m_stop;
		std::mutex m_mutex;
		std::condition_variable m_cv;
		std::vector<std::thread> m_threads;
		std::queue<std::function<void()>> m_queue;
	};

	// thread pool by lambda
	class ThreadPool
	{
	  public:
		ThreadPool(size_t);
		template <class F, class... Args>
		auto enqueue(F&& f, Args&&... args)
			-> std::future<typename std::result_of<F(Args...)>::type>;

		//    double enqueueMy(double f(int), int);
		~ThreadPool();

	  private:
		// need to keep track of threads so we can join them
		std::vector<std::thread> workers;
		// the task queue
		std::queue<std::function<void()>> tasks;

		// synchronization
		std::mutex queue_mutex;
		std::condition_variable condition;
		bool stop;
	};

	// the constructor just launches some amount of workers
	inline ThreadPool::ThreadPool(size_t threads) : stop(false)
	{
		for (size_t i = 0; i < threads; ++i)
			workers.emplace_back([this] {
				for (;;) {
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> lock(this->queue_mutex);
						this->condition.wait(lock, [this] {
							return this->stop || !this->tasks.empty();
						});
						if (this->stop && this->tasks.empty())
							return;
						task = std::move(this->tasks.front());
						this->tasks.pop();
					}

					task();
				}
			});
	}

	// add new work item to the pool
	template <class F, class... Args>
	auto ThreadPool::enqueue(F&& f, Args&&... args)
		-> std::future<typename std::result_of<F(Args...)>::type>
	{
		using return_type = typename std::result_of<F(Args...)>::type;

		auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);

			// don't allow enqueueing after stopping the pool
			if (stop)
				throw std::runtime_error("enqueue on stopped ThreadPool");

			tasks.emplace([task]() { (*task)(); });
		}
		condition.notify_one();
		return res;
	}

	// the destructor joins all threads
	inline ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			stop = true;
		}
		condition.notify_all();
		for (std::thread& worker : workers)
			worker.join();
	}

}  // namespace yuki_new_features__thread_test

#endif
