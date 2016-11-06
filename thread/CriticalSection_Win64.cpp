#include "CriticalSection.h"
namespace thread
{

	CriticalSection::CriticalSection()
	{
		InitializeCriticalSectionAndSpinCount(&m_CriticalSection,0x100);
	}

	CriticalSection::~CriticalSection()
	{
		DeleteCriticalSection(&m_CriticalSection);
	}


	bool CriticalSection::TryLock()
	{
		BOOL res = TryEnterCriticalSection(&m_CriticalSection);
		return res != FALSE;
	}

	void CriticalSection::Lock()
	{
		EnterCriticalSection(&m_CriticalSection);
	}

	void CriticalSection::Unlock()
	{
		LeaveCriticalSection(&m_CriticalSection);
	}


} // namespace thread
