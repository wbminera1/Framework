#include "../common/Statics.h"

#include "Catch/single_include/catch.hpp"

TEST_CASE("CommonTest", "[statics]")
{
	SECTION("MaxMin")
	{
		char tmin[statics::Min(8, 16)];
		REQUIRE(sizeof(tmin) == 8);
		char tmax[statics::Max(8, 16)];
		REQUIRE(sizeof(tmax) == 16);
	}
}