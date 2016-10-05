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

	pthread_mutex_lock(&count_mutex);
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
	pthread_mutex_unlock(&count_mutex);
	return status;
}

bool CommandSendBlocking::Handle(Command& cmd, ICommandHandler* source)
{
	if (cmd.m_Command == cResponse)
	{
		pthread_mutex_lock(&count_mutex);
		if (m_Response != NULL)
		{
			m_Response->m_Command = cmd.m_Command;
		}
		received = true;
		pthread_cond_signal(&count_threshold_cv);
		pthread_mutex_unlock(&count_mutex);
		return true;
	}
	return false;
}
