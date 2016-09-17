#include "CommandEnums.h"
#define GENERATE_ENUM_STRINGS  // Start string generation
#include "CommandEnums.h"
#undef GENERATE_ENUM_STRINGS   // Stop string generation

#include <stdio.h>

void TestEnums()
{
  printf("%s\n", ToString(cError));
  printf("%s\n", ToString(sOk));
}
