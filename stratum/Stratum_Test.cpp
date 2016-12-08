#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Catch/single_include/catch.hpp"

#include "../common/Log.h"
#include "../stratum/Stratum.h"


TEST_CASE("Stratum", "[Stratum]")
{

	SECTION("StratumParse")
	{
		stratum::Stratum str;
		// Received: {"id":1,"error":null,"result":[null,"ca1eff67000000000000000067ff1ec95a"]}

		str.Process("{\"id\":null,\"method\":\"mining.set_target\",\"params\":[\"01e1e1e1e0000000000000000000000000000000000000000000000000000000\"]}");
		REQUIRE(true);
	}
}
