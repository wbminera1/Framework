#ifndef SOCKETRECTHREAD_H_
#define SOCKETRECTHREAD_H_

#include "../sockets/Socket.h"
#include "../thread/Thread.h"

namespace sockets
{
	class SocketRecThread : public thread::Thread
	{
	public:
		SocketRecThread(size_t bufferSize = 1024)
			: Thread(__FUNCTION__)
			, m_Socket(nullptr)
			, m_BufferSize(bufferSize)
		{
			m_Data.reserve(bufferSize);
		}

		void SetSocket(sockets::Socket* socket) { m_Socket = socket; }
		std::vector<char>& GetData() { return m_Data; }

		void SetOnReceived(ThreadEventFn onStart, void* dataPtr = nullptr)
		{
			m_OnReceived.m_Fn = onStart;
			m_OnReceived.m_Args.m_Thread = this;
			m_OnReceived.m_Args.m_DataPtr = dataPtr;
		}

	protected:

		void Process();

		ThreadEvent m_OnReceived;
		sockets::Socket* m_Socket;
		size_t m_BufferSize;
		std::vector<char> m_Data;
	};
} // namespace sockets

#endif // SOCKETRECTHREAD_H_
