#include <string.h>
#include <stddef.h>
#include <errno.h>

#include "Command.h"
#include "../common/Log.h"


void Command::Serialize(std::vector<char>& commandBuffer) const
{
	toBuffer(commandBuffer, m_Command);
}


void Command::Deserialize(const std::vector<char>& commandBuffer)
{
	fromBuffer(commandBuffer, sizeof(uint32_t), m_Command);
}

Command* Command::Create(std::vector<char>& data)
{
	// uint32 size
	// uint32 type
	// uint32 id
	// ... command body
	uint32_t* dataSize = (uint32_t*)&data[0];
	if (*dataSize > Command::MAX_COMMAND_SIZE) {
		Log(LOG_ERR, "Wrong command size: %d\n", *dataSize);
		return nullptr;
	}
	uint32_t* type = dataSize + 1;
	if (*type == cFirst || *type >= cLast) {
		Log(LOG_ERR, "Wrong command type: %d\n", *type);
		return nullptr;
	}

	Log(LOG_INFO, "Command size: %d type: %d\n", *dataSize, *type);
	
	Command* cmd = Create((eCommand)*type);

	if (cmd != nullptr) {
		cmd->Deserialize(data);
	}

	data.erase(data.begin(), data.begin() + *dataSize);

	return cmd;
}

Command* Command::Create(eCommand command)
{
	Command* cmd = nullptr;
	switch (command)
	{
	cError:
	cResponse:
	cConnect:
	cDisconnect:
	cExit:
	cJSONCommand:
	cBinaryCommand:
	default:
		cmd = new Command(command);
		break;
	}
	return cmd;
}


void Command::Destroy(Command* cmd)
{
	delete cmd;
}


void Command::Test(void)
{
  Command cmd;
  cmd.m_Command = cConnect;
}
