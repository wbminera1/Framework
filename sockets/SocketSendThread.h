#ifndef SOCKETSENDTHREAD_H_
#define SOCKETSENDTHREAD_H_
#include "../sockets/Socket.h"
#include "../thread/Thread.h"
#include "../thread/Mutex.h"
#include "../thread/Condition.h"

namespace sockets
{
	class SocketSendThread : public thread::Thread
	{
	public:
		SocketSendThread()
			: Thread(__FUNCTION__)
			, m_Socket(nullptr)
		{
			m_Data.reserve(1024);
		}

		void SetSocket(sockets::Socket* socket) { m_Socket = socket; }

		void Send(const std::vector<char>& data);

		virtual void Stop();

	protected:
		
		virtual void Process();

		sockets::Socket* m_Socket;

		thread::Condition m_CommandWait;
		std::vector<char> m_Data;
	};

} // namespace sockets

#endif // SOCKETSENDTHREAD_H_
