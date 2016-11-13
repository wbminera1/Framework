#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "../common/Log.h"

#include "gason/src/gason.h"
#include "Catch/single_include/catch.hpp"

bool LoadFile(const char* fileName, char*& data)
{
	FILE* fh = fopen(fileName, "rb");
	if (fh != nullptr)
	{
		fseek(fh, 0, SEEK_END);
		size_t length = (size_t)ftell(fh);
		if (length > 0) {
			data = (char*)malloc((size_t)length);
			fseek(fh, 0, SEEK_SET);
			size_t read = fread(data, 1, length, fh);
			if (read == length) {
				fclose(fh);
				return true;
			}
			free(data);
			data = nullptr;
		}
		fclose(fh);
	}
	return false;
}

TEST_CASE("JSONTest", "[JSON]")
{

	SECTION("GasonParse")
	{
		char* data;
		bool passed = false;
		if (LoadFile("tests/Data/big.json", data))
		{
			char *endptr;
			JsonValue value;
			JsonAllocator allocator;
			int result = jsonParse(data, &endptr, &value, allocator);
			passed = (result == JSON_OK);
			free(data);
			data = nullptr;
		}
		REQUIRE(passed);
	}
}
