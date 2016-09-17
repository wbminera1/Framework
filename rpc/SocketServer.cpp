//#define _WINSOCK_DEPRECATED_NO_WARNINGS 
//#include <errno.h>
#include <string.h>
//#include <ws2tcpip.h>

#include "../sockets/Socket.h"

#include "../common/Log.h"
#include "SocketServer.h"
#include "Command.h"
//#include <filesystem>

const int MAX_CONNECTIONS = 1;
//const char REQUEST_DELIMITER = '\n';

SocketServer::SocketServer(int _port, CommandDispatcher* dispatcher)
  : Dispatched(dispatcher)
  , m_Port(_port)
{
//  commandToSend.reserve(MAX_COMMAND_SIZE);
}

SocketServer::~SocketServer()
{
}

bool SocketServer::Start()
{
	sockets::SocketAddress serverA(AF_INET);
	serverA.pton("127.0.0.1");
	serverA.port(m_Port);
	sockets::Socket serverS;

	if (serverS.Open() && serverS.Bind(serverA)) {
		if (serverS.Listen(MAX_CONNECTIONS)) {
			requestMutex.Lock();
			sockets::Socket connectedS;
			sockets::SocketAddress connectedA(AF_INET);
			if (connectedS.Accept(serverS, connectedA))
			{
				Log(LOG_INFO, "SocketServer - accepted :\n");
			}
			requestMutex.Unlock();
		}
	}
	return false;
}

/*
void SocketServer::Loop()
{
  std::string stream, request;
  stream.reserve(MAX_COMMAND_SIZE);
  request.reserve(MAX_COMMAND_SIZE);
  uint32_t commandSize = 0;
  while (true)
  {
    const int flags = 0;
    auto bytes_received = recv(m_AcceptSocket, &stream[0], (int)stream.capacity(), flags);
    if (bytes_received == 0)
    {
      Log(LOG_INFO, "SocketServer - host shutdown.\n");
      break;
    }
    if (bytes_received < 0)
    {
		Log(LOG_ERR, "SocketServer - receive error: %d\n", bytes_received);
      break;
    }

	Log(LOG_INFO, "SocketServer - received %d\n", bytes_received);
    // Parsing incoming data
    for (int i = 0; i < bytes_received; ++i)
    {
      if (request.size() >= request.capacity())
      {
		  Log(LOG_INFO, "SocketServer - request is too long (at least %d) .\n ", request.size());
	      request.clear();
        break;
      }
      request += stream[i];
	  if ((commandSize == 0) && (request.size() >= sizeof(commandSize)))
	  {
		  commandSize = *reinterpret_cast<const uint32_t*>(request.c_str());
	  }
	  if ((commandSize > 0) && (request.size() == commandSize))
	  {
		  //!! On command
		  Log(LOG_INFO, "SocketServer - command size %d receive.\n ", commandSize);
		  commandSize = 0;
		  request.clear();
	  }
	}
    stream.clear();
  } // Request handling loop
}
*/

/*
bool SocketServer::Send(const Command& cmd)
{
	pthread_mutex_lock(&requestMutex);

  //std::string str;
  //cmd.Serialize(str);

  //commandToSend = str;

  if (commandToSend.length() > 0)
  {
    commandToSend.push_back(REQUEST_DELIMITER);
    ::send(m_AcceptSocket, commandToSend.c_str(), (int)commandToSend.size(), 0);
    //!!Log(LOG_NOTICE,"SocketServer - sent data '%s'\n", commandToSend.c_str());
    commandToSend.clear();
  }
  pthread_mutex_unlock(&requestMutex);
  return true;
}
*/

/*
void SocketServer::Handle(std::string& str, std::vector<char>& binaryData)
{
  str += '\0';
  Command cmd;
  //if (cmd.Parse(str))
  {
    //cmd.binary.swap(binaryData); // move
	//Log(LOG_INFO, "SocketServer::handle, binary size %d ", cmd.binary.size());
    if (dispatcher != NULL)
    {
      dispatcher->dispatch(cmd);
    }
    //CommandHandler handler;
    //handler.onCommand(cmd, this);
  }
}
*/

