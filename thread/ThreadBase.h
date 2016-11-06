#ifndef THREADBASE_H_
#define THREADBASE_H_
#include <string>
#ifdef _WIN64
#include <windows.h>

struct ThreadDataWin64
{
	HANDLE Handle;
	DWORD  Id;
};

typedef DWORD ThreadProcReturnType;
typedef LPVOID ThreadProcParamType;

#define THREADAPI WINAPI
typedef LPTHREAD_START_ROUTINE ThreadProcType;
typedef ThreadDataWin64 ThreadDataType;

#endif
#ifdef __unix__
#include <pthread.h>
#endif

namespace thread
{

struct ThreadPlatformBaseProc
{
	static ThreadProcReturnType Proc(ThreadProcParamType ptr);
};

class ThreadBase
{
    public:

		ThreadBase(const char* name = nullptr, ThreadProcType proc = BaseProc)
			: m_Proc(proc)
			, m_Stop(false)
		{ 
			memset(&m_Data, 0, sizeof(m_Data));
			SetName(name);
		}

        virtual ~ThreadBase() { }

		void SetName(const char* name)
		{
			if (name != nullptr)
			{
				m_Name = name;
			}
		}

		const std::string& GetName() const
		{
			return m_Name;
		}

		virtual int Create();

		virtual int Join();

		virtual void Stop()
        {
            m_Stop = true;
        }

protected:
		virtual void OnStart();
		virtual void Process() = 0;
		virtual void OnStop() { }

        volatile bool m_Stop;

		friend ThreadProcType;
		ThreadProcType m_Proc;
		ThreadDataType m_Data;
		std::string m_Name;

		static ThreadProcReturnType THREADAPI BaseProc(ThreadProcParamType ptr);

};


} // namespace thread

#endif // THREADBASE_H_
