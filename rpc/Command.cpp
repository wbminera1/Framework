#include <string.h>
#include <stddef.h>
#include <errno.h>

#include "Command.h"
#include "../common/Log.h"


void Command::Serialize(std::vector<char>& commandBuffer) const
{
	toBuffer(commandBuffer, command);
}


void Command::Deserialize(const std::vector<char>& commandBuffer)
{
}


CommandSendBlocking::CommandSendBlocking()
  : received(false)
  , command(cError)
  , response(nullptr)
{
  //count_mutex .value = PTHREAD_MUTEX_INITIALIZER;
  //count_threshold_cv.value = PTHREAD_COND_INITIALIZER;
}


bool CommandSendBlocking::SendBlocking(const Command& cmd, ICommandHandler& dest, CommandDispatcher& disp)
{
  bool status = true;

  pthread_mutex_lock(&count_mutex);
//  struct timespec timeToWait;
//  clock_gettime(CLOCK_MONOTONIC, &timeToWait);
//  timeToWait.tv_sec += timeOut;

  received = false;
  command = cmd.command;
  disp.AddHandler(this);
  dest.Send(cmd);
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

bool CommandSendBlocking::Handle(Command& cmd)
{
  if (cmd.command == cResponse)
  {
    pthread_mutex_lock(&count_mutex);
    if (response != NULL)
    {
      response->command = cmd.command;
    }
    received = true;
    pthread_cond_signal(&count_threshold_cv);
    pthread_mutex_unlock(&count_mutex);
    return true;
  }
  return false;
}

void Command::Test(void)
{
  Command cmd;
  cmd.command = cPing;
}
