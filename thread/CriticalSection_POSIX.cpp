#include "CriticalSection.h"
namespace thread
{

	CriticalSection::CriticalSection()
	{
	}

	CriticalSection::~CriticalSection()
	{
	}


	bool CriticalSection::TryLock()
	{
		return false;
	}

	void CriticalSection::Lock()
	{
	}

	void CriticalSection::Unlock()
	{
	}


} // namespace thread
