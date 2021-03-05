#ifndef WORKSCHEDULER_H
#define WORKSCHEDULER_H

#include <future>
#include <atomic>
#include <vector>


class WorkPool
{
	template<typename Work>
	class ExceptionSafeWork
	{
	public:
		ExceptionSafeWork(Work&& work, WorkPool& scheduler)
			: m_work(std::move(work))
			, m_scheduler(scheduler)
		{}

		std::exception_ptr operator()() noexcept(true)
		{
			if(m_scheduler.m_doAbort)
				return nullptr;

			try
			{
				m_work();
				std::unique_lock lck(m_scheduler.m_workDoneMtx);
				m_scheduler.m_workDoneCV.notify_one();
				return nullptr;
			}
			catch(...)
			{
				if(!m_scheduler.m_doAbort)
					m_scheduler.m_doAbort.store(true);
				return std::current_exception();
			}
		}

	private:
		Work m_work;
		WorkPool& m_scheduler;
	};

public:
	using FreezeNotifier = std::function<void(const std::chrono::milliseconds&)>;

	WorkPool(size_t threadsCount) noexcept(true);
	virtual ~WorkPool();

	void wait() noexcept(false);

	template<typename Work>
	void put(Work&& work) noexcept(false)
	{
		if(m_doAbort)
			wait();

		do
		{
			for(auto it = m_works.begin(); it != m_works.end();)
			{
				if(it->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
				{
					auto ex = it->get();
					if(ex)
						std::rethrow_exception(ex);
					it = m_works.erase(it);
					break;
				}
				else
				{
					++it;
				}
			}

			if(m_works.size() == m_threadsCount)
			{
				std::unique_lock lck{ m_workDoneMtx };
				m_workDoneCV.wait(lck);
			}
		}
		while(m_works.size() == m_threadsCount);

		m_works.emplace_back(std::async(std::launch::async, ExceptionSafeWork<Work>{ std::move(work), *this }));
	}

private:
	size_t m_threadsCount;
	std::vector<std::future<std::exception_ptr>> m_works;
	std::atomic_bool m_doAbort{ false };
	std::mutex m_workDoneMtx;
	std::condition_variable m_workDoneCV;
};

#endif // WORKSCHEDULER_H
