#ifndef THREADBASE_H_
#define THREADBASE_H_
#include <string.h>
#include <string>
#include <pthread.h>

namespace thread
{

class ThreadBase
{
    public:

		ThreadBase(const char* name = nullptr)
			: m_Stop(false)
		{ 
			memset(&m_Thread, 0, sizeof(m_Thread));
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

		virtual int Create()
        {
            pthread_attr_t attr;
            pthread_attr_init(&attr); // TODO attributes to class
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
            int res = pthread_create(&m_Thread, &attr, &ThreadBase::Start, (void *) this);
            pthread_attr_destroy(&attr);
            return res;
        }

        virtual int Join()
        {
            void* status;
            int res = pthread_join(m_Thread, &status);
            return res;
        }

		virtual void Stop()
        {
            m_Stop = true;
        }

protected:
		virtual void OnStart();
		virtual void Process() = 0;
		virtual void OnStop();

        volatile bool m_Stop;

        pthread_t m_Thread;
		std::string m_Name;

		static void* Start(void* ptr);
};

} // namespace thread

#endif // THREADBASE_H_
