#ifndef TRAINTICKETSYSTEM_2022_SRC_TOOLS_ASSERT_CPP
#define TRAINTICKETSYSTEM_2022_SRC_TOOLS_ASSERT_CPP

#include "asserts.h"
namespace sjtu {
void Assert(bool condition, const char* msg) {
  if (!condition) {
    throw msg;
  }
}
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_TOOLS_ASSERT_CPP