#include "Bstring.h"
#include "gtest/gtest.h"
TEST(Bstring,  DefaultConstructor) {
  const Bstring s;
  EXPECT_STREQ("", s.c_str());
  EXPECT_EQ(0u, s.length());
}
TEST(Bstring, Substring) {
  const Bstring s("abcde");
  const Bstring su=s(2,2);
  EXPECT_STREQ("cd",su.c_str());
  EXPECT_EQ(2u, su.length());
}
