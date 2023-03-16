#include "thread_pool.h"
#include <atomic>
#include <chrono>
#include <future>

namespace yuki_new_features__thread_test
{
	// test promist & future & packaged_task & async
	void promiseFutureTest()
	{
		auto f = [](std::promise<std::string>& a_p, std::mutex& a_m) {
			std::lock_guard l(a_m);
			std::cout << "promiseTest" << std::endl;
			a_p.set_value("child thread: promiseTest");
		};
		std::mutex m;
		std::promise<std::string> p;
		std::future<std::string> futu = p.get_future();

		std::thread t(f, std::ref(p), std::ref(m));
		t.join();

		std::cout << futu.get() << std::endl;
	}

	void packagedtaskTest()
	{
		auto f = [](int a_m, int a_n) { return a_m + a_n; };
		std::packaged_task<int(int, int)> task(f);

		std::future<int> res = task.get_future();
		std::thread t(std::move(task), 1, 2);
		std::cout << res.get() << std::endl;
	}

	void asyncTest(std::launch a_enum)
	{
		auto f = [](int a_m, int a_n) {
			std::cout << "child thread value: " << a_m + a_n << std::endl;
			return a_m + a_n;
		};
		std::future<int> ff = std::async(a_enum, f, 2, 3);

		std::chrono::duration<int, std::ratio<1>> m3(10);
		std::this_thread::sleep_for(m3);

		std::cout << "asyncTest" << std::endl;
		std::cout << ff.get() << std::endl;
	}

}

namespace yuki_new_features__thread_test
{

	// test thread pool by promise
	ThreadPoolTestPromise::ThreadPoolTestPromise(size_t a_thread_num)
		: m_stop(false)
	{
		std::thread([this]()
		            {
						std::unique_lock<std::mutex> ul(this->m_mutex);
						this->m_cond_varia.wait(ul, [this](){
							return !this->m_task_queue.empty() || m_stop;
							if(m_stop){
								break;
							}
							auto t = std::move(m_task_queue.front());
							m_task_queue.pop();
							t.set_value(i * 2);
						});
					});
	}

	std::future<int> ThreadPoolTestPromise::pushTask(std::promise<int> && a_p, int)
	{

	}
}

namespace yuki_new_features__thread_test
{
	// test thread pool by enclosure std::function
	ThreadPoolFunction::ThreadPoolFunction(size_t a_threads_num):m_stop(false)
	{
		for(size_t i = 0; i < a_threads_num; ++i){
			m_threads.emplace_back(
				[this](){
					while(1){
						std::function<void()> f;
						{
							std::unique_lock<std::mutex> ul(this->m_mutex);
							this->m_cv.wait(ul, [this](){return !this->m_queue.empty() || this->m_stop;});
							if(this->m_stop){
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
		auto task = std::make_shared<std::packaged_task<int()>> (std::packaged_task<int()>([a_n](){
			return a_n * 2;
		}));
		std::future<int> res = task->get_future();
		{
			std::unique_lock<std::mutex> lg(m_mutex);
			m_queue.emplace([task](){ (*task)();});

		}
		m_cv.notify_one();
		return res;
	}

	ThreadPoolFunction::~ThreadPoolFunction()
	{
		{
			std::unique_lock<std::mutex> ul(m_mutex);
			m_stop=true;

		}
		m_cv.notify_all();
		for(auto& i: m_threads){
			i.join();
		}

	}

}

namespace yuki_new_features__thread_test
{

    TaskTest::TaskTest(std::packaged_task<int(int)>&& a_t, int a_i): m_int(a_i)
    {
        m_p = std::move(a_t);
    }

    TaskTest::TaskTest()
    {

    }


//    bool TaskQueueTest::pushTask(std::packaged_task<int(int)>&&a_task, int a_i)
//    {
//        if(m_queue.empty()){
//            return false;
//        }

//        std::unique_lock<std::mutex> l(thread_pool_mutex);
//        TaskTest t{std::move(a_task), a_i};
//        m_queue.push(t);
//        condition_varia.notify_one();

//        return true;
//    }

    bool TaskQueueTest::popTask(TaskTest& a_t)
    {
//        std::unique_lock<std::mutex> l(thread_pool_mutex);
//        if(m_queue.size()){
////            a_t = std::move(m_queue.front());
//            m_queue.pop();
//        }
    }

    bool TaskQueueTest::isEmpty()
    {
        return m_queue.empty();
    }

//    void ThreadPoolTest::initThread()
//    {
//        while(m_flag){

//            std::unique_lock<std::mutex> l(thread_pool_mutex);
//            m_cond_varia.wait(l, [this](){return !this->m_task_queue.isEmpty();});

//            TaskTest task;
//            if(m_task_queue.popTask(task)){
//                std::packaged_task<int(int)> package_t = std::move(task.m_p);
//                int temp = task.m_int;
//                package_t(temp);
//            }
//            else{
//                std::this_thread::yield();
//            }
//        }

//    }

//    void ThreadPoolTest::pushTask(std::packaged_task<int(int)>&& a_p, int a_int)
//    {
//        m_task_queue.pushTask(std::move(a_p), a_int);
//    }

}


