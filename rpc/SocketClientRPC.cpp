#include "../common/Log.h"
#include "SocketClientRPC.h"


SocketClientRPC::SocketClientRPC()
	: Dispatched(nullptr)
{
	Log(LOG_WARNING, __FUNCTION__);
	m_CommandBuffer.reserve(1024);
}

SocketClientRPC::~SocketClientRPC()
{
	Log(LOG_WARNING, __FUNCTION__ );
}

bool SocketClientRPC::Handle(const Command& cmd, ICommandHandler* source)
{
	Log(LOG_DEBUG, __FUNCTION__ " started");

	cmd.Serialize(m_CommandBuffer);

	m_SendThread.Send(m_CommandBuffer);

	Log(LOG_DEBUG, __FUNCTION__ " stopped");
	return false;
}

void SocketClientRPC::OnReceivedEvent(Thread* thread)
{
	Log(LOG_DEBUG, __FUNCTION__ " started %s", thread->GetName().c_str());
	Log(LOG_DEBUG, __FUNCTION__ " data size %d", m_RecThread.GetData().size());
	uint32_t* dataSize = (uint32_t*)&m_RecThread.GetData()[0];

	if (*dataSize > Command::MAX_COMMAND_SIZE) 
	{
		Log(LOG_ERR, __FUNCTION__ " wrong data size: %d", *dataSize);
		m_RecThread.GetData().clear();
	}
	if (m_RecThread.GetData().size() >= *dataSize) 
	{
		Command* cmd = Command::Create(m_RecThread.GetData());
		if (cmd != NULL && GetDispatcher() != nullptr) 
		{
			GetDispatcher()->DispatchFrom(*cmd, this);
		}
	}
	Log(LOG_DEBUG, __FUNCTION__ " stopped %s", thread->GetName().c_str());
}

void SocketClientRPC::Process()
{
	Log(LOG_DEBUG, __FUNCTION__ " started");

	if (m_Socket.IsValid()) 
	{
		Log(LOG_INFO, __FUNCTION__ " connected");
		SocketClientThreaded::Process();
	}
	else
	{
		Log(LOG_ERR, __FUNCTION__ " Connect failed with error: %d", WSAGetLastError());
	}
	Log(LOG_DEBUG, __FUNCTION__ " stopped");
}
