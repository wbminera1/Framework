#include <string.h>
#include <stddef.h>
#include <errno.h>

#include "Command.h"
#include "../common/Log.h"


size_t Command::Serialize(std::vector<char>& commandBuffer) const
{
	toBuffer(commandBuffer, m_Command);
	return toBuffer(commandBuffer, m_Id);
}


size_t Command::Deserialize(const std::vector<char>& commandBuffer)
{
	size_t offset = fromBuffer(commandBuffer, sizeof(uint32_t), m_Command);
	return fromBuffer(commandBuffer, offset, m_Id);
}

Command* Command::Create(std::vector<char>& data)
{
	// uint32 size
	// uint32 type
	// uint32 id
	// ... command body
	uint32_t* dataSize = (uint32_t*)&data[0];
	if (*dataSize > Command::MAX_COMMAND_SIZE) {
		Log(LOG_ERR, "Wrong command size: %d", *dataSize);
		return nullptr;
	}
	uint32_t* type = dataSize + 1;
	if (*type == cFirst || *type >= cLast) {
		Log(LOG_ERR, "Wrong command type: %d", *type);
		return nullptr;
	}

	Log(LOG_INFO, "Command size: %d type: %d", *dataSize, *type);
	
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
	case cError:
	case cResponse:
		cmd = new CommandResponse();
		break;
	case cConnect:
		cmd = new CommandConnect();
		break;
	case cDisconnect:
	case cExit:
	case cJSONCommand:
	case cBinaryCommand:
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

size_t CommandResponse::Serialize(std::vector<char>& commandBuffer) const
{
	size_t offset = Command::Serialize(commandBuffer);
	return toBuffer(commandBuffer, m_Response);
}

size_t CommandResponse::Deserialize(const std::vector<char>& commandBuffer)
{
	size_t offset = Command::Deserialize(commandBuffer);
	return fromBuffer(commandBuffer, offset, m_Response);
}

size_t CommandConnect::Serialize(std::vector<char>& commandBuffer) const
{
	size_t offset = Command::Serialize(commandBuffer);
	return toBuffer(commandBuffer, m_Version);
}

size_t CommandConnect::Deserialize(const std::vector<char>& commandBuffer)
{
	size_t offset = Command::Deserialize(commandBuffer);
	return fromBuffer(commandBuffer, offset, m_Version);
}
