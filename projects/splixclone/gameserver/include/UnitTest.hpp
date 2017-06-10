#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

#include "gtest/gtest.h"

namespace testing
{
 namespace internal
 {
  enum GTestColor {
      COLOR_DEFAULT,
      COLOR_RED,
      COLOR_GREEN,
      COLOR_YELLOW
  };

  extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
 }
}

#define TMSG(...)  do { testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, "[ TestCase ] "); testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, __VA_ARGS__); } while(0)

#endif
