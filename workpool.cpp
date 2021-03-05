#include "workpool.h"

WorkPool::WorkPool(size_t threadsCount) noexcept(true)
	: m_threadsCount(threadsCount)
{
	m_works.reserve(threadsCount ? threadsCount : 1);
}

WorkPool::~WorkPool()
{
	try
	{
		wait();
	}
	catch(...) {}
}

void WorkPool::wait() noexcept(false)
{
	std::exception_ptr ex = nullptr;
	for(auto it = m_works.begin(); it != m_works.end();)
	{
		it->wait();
		if(!ex)
			ex = it->get();
		it = m_works.erase(it);
	}
	if(ex)
		std::rethrow_exception(ex);
}
