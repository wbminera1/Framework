#include "../common/Log.h"
#include "CommandSendBlocking.h"

CommandSendBlocking::CommandSendBlocking()
	: received(false)
	, command(cError)
	, m_Response(nullptr)
{
}

bool CommandSendBlocking::SendBlocking(const Command& cmd, ICommandHandler& dest, CommandDispatcher& disp)
{
	bool status = true;
	count_mutex.Lock();
	//  struct timespec timeToWait;
	//  clock_gettime(CLOCK_MONOTONIC, &timeToWait);
	//  timeToWait.tv_sec += timeOut;

	received = false;
	command = cmd.m_Command;
	disp.AddHandler(this);
	dest.Handle(cmd, this);
	while (!received)
	{
		//    int l_rt = pthread_cond_timedwait_monotonic_np(&count_threshold_cv, &count_mutex, &timeToWait);
		//    if (l_rt == ETIMEDOUT)
		{
			Log(LOG_ERR, "CommandSendBlocking::sendBlocking timeout!");
			break;
		}
	}
	disp.DelHandler(this);
	count_mutex.Unlock();
	return status;
}

bool CommandSendBlocking::Handle(Command& cmd, ICommandHandler* source)
{
	if (cmd.m_Command == cResponse)
	{
		count_mutex.Lock();
		if (m_Response != NULL)
		{
			m_Response->m_Command = cmd.m_Command;
		}
		received = true;
		count_threshold_cv.Signal();
		count_mutex.Unlock();
		return true;
	}
	return false;
}
