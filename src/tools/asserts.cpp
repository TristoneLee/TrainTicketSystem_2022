#ifndef TRAINTICKETSYSTEM_2022_SRC_TOOLS_ASSERT_CPP
#define TRAINTICKETSYSTEM_2022_SRC_TOOLS_ASSERT_CPP

#include "asserts.h"
namespace sjtu {
void Assert(bool condition) {
  if (!condition) {
    throw 1;
  }
}
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_TOOLS_ASSERT_CPP