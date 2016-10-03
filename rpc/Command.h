#ifndef COMMAND_H_
#define COMMAND_H_
#include <string>
#include <vector>
#include <cstdint>
#include <pthread.h>

#include "CommandEnums.h"

/**
 *
 */
struct Command
{
	eCommand m_Command;
	uint32_t m_Id;

	Command() : m_Command(cError), m_Id(0) {}
	explicit Command(eCommand command) : m_Command(command), m_Id(0) {}
	virtual ~Command() {}

	virtual size_t Serialize(std::vector<char>& commandBuffer) const;
	virtual size_t Deserialize(const std::vector<char>& commandBuffer);

	template <typename T> size_t toBuffer(std::vector<char>& commandBuffer, const T& field) const
	{
		std::vector<char> tmpBuffer;
		tmpBuffer.assign((char*)&field, (char*)&field + sizeof(field));
		commandBuffer.insert(commandBuffer.end(), tmpBuffer.begin(), tmpBuffer.end());
		return commandBuffer.size();
	}

	template <typename T> size_t fromBuffer(const std::vector<char>& commandBuffer, size_t offset, T& field) const
	{
		field = *(T*)&commandBuffer[offset];
		return offset + sizeof(field);
	}

	static const uint32_t VERSION = 0x0001;
	static const size_t MAX_COMMAND_SIZE = 1024;
	
	static Command* Create(std::vector<char>& data);
	static Command* Create(eCommand command);

	static void Destroy(Command*);

	static void Test(void);

};

struct CommandResponse : public Command
{
	eResponse m_Response;
	CommandResponse() : Command(cResponse), m_Response(sWrongCommand) {}
	CommandResponse(const Command& cmd, eResponse response = sWrongCommand) : Command(cResponse), m_Response(response) { m_Id = cmd.m_Id; }

	virtual size_t Serialize(std::vector<char>& commandBuffer) const;
	virtual size_t Deserialize(const std::vector<char>& commandBuffer);
};

struct CommandConnect : public Command
{
	uint32_t m_Version;

	CommandConnect() : Command(cConnect), m_Version(VERSION) {}

	virtual size_t Serialize(std::vector<char>& commandBuffer) const;
	virtual size_t Deserialize(const std::vector<char>& commandBuffer);
};

#endif /* COMMAND_H_ */
