#include "smokestack.h"
#include <exception>
#include <gtest/gtest.h>
#include <print>
#include <ranges>
#include <string>

using namespace engineering::hansen::smokestack;
using std::println;
using std::string;
using std::ranges::transform_view;

TEST(smokestack, constructor) {
  try {
    Smokestack s;
    EXPECT_TRUE(true);
  } catch (std::exception &e) {
    EXPECT_TRUE(false);
  }
}

TEST(smokestack, push) {
  Smokestack s;
  s.push("A");
  s.push("B");
  s.push("C");

  EXPECT_TRUE(0 == s.reservoir_size() && 3 == s.stack_size() &&
              "CBA" == s.pop() + s.pop() + s.pop());
}

TEST(smokestack, add) {
  Smokestack s;
  s.add("A");
  s.add("B");
  s.add("C");
  EXPECT_TRUE(3 == s.reservoir_size() && 0 == s.stack_size() &&
              "ERROR" != s.pop());
}

TEST(smokestack, replenish) {
  string abet("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  string teba;
  Smokestack s;
  for (auto i = abet.cbegin(); i < abet.cend(); i += 1)
    s.push(string(i, i + 1));
  s.replenish();
  while (!s.stack_empty())
    teba += s.pop();
  EXPECT_TRUE(abet != teba && abet.size() == teba.size());
}
