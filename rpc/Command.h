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

	Command() : m_Command(cError) {}
	explicit Command(eCommand command) : m_Command(command) {}

	void Serialize(std::vector<char>& commandBuffer) const;
	void Deserialize(const std::vector<char>& commandBuffer);

	template <typename T> void toBuffer(std::vector<char>& commandBuffer, const T& field) const
	{
		std::vector<char> tmpBuffer;
		tmpBuffer.assign((char*)&field, (char*)&field + sizeof(field));
		commandBuffer.insert(commandBuffer.end(), tmpBuffer.begin(), tmpBuffer.end());
	}

	template <typename T> void fromBuffer(const std::vector<char>& commandBuffer, size_t offset, T& field) const
	{
		field = *(T*)&commandBuffer[offset];
	}

	static const size_t MAX_COMMAND_SIZE = 1024;
	
	static Command* Create(std::vector<char>& data);
	static Command* Create(eCommand command);

	static void Destroy(Command*);

	static void Test(void);

};

#endif /* COMMAND_H_ */
