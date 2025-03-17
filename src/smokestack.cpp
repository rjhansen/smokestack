#include "smokestack.h"
#include <algorithm>
#include <emscripten/bind.h>
#include <mutex>

using std::lock_guard;
using std::mutex;
using std::string;
using std::ranges::shuffle;

namespace engineering {
namespace hansen {
namespace smokestack {

Smokestack::Smokestack() {}

void Smokestack::add(string token) {
  lock_guard<mutex> lock(reservoir_mutex);
  reservoir.emplace_back(token);
}

void Smokestack::push(std::string token) {
  lock_guard<mutex> lock(reservoir_mutex);
  stack.push(token);
}

void Smokestack::replenish() {
  lock_guard<mutex> stack_lock(stack_mutex);
  lock_guard<mutex> reservoir_lock(reservoir_mutex);
  while (!stack.empty()) {
    reservoir.push_back(stack.top());
    stack.pop();
  }
  shuffle(reservoir, prng);
  for (string token : reservoir)
    stack.push(token);
  reservoir.clear();
  reservoir.resize(0);
}

string Smokestack::pop() {
  {
    lock_guard<mutex> lock(stack_mutex);
    if (!stack.empty()) {
      string rv = stack.top();
      stack.pop();
      return rv;
    }
  }
  replenish();
  {
    lock_guard<mutex> lock(stack_mutex);
    if (!stack.empty()) {
      auto rv = stack.top();
      stack.pop();
      return rv;
    } else {
      return "ERROR";
    }
  }
}

size_t Smokestack::stack_size() {
  lock_guard<mutex> lock(stack_mutex);
  return static_cast<size_t>(stack.size());
};

size_t Smokestack::reservoir_size() {
  lock_guard<mutex> lock(reservoir_mutex);
  return static_cast<size_t>(reservoir.size());
}

bool Smokestack::stack_empty() {
  lock_guard<mutex> lock(stack_mutex);
  return stack.empty();
}

bool Smokestack::reservoir_empty() {
  lock_guard<mutex> lock(reservoir_mutex);
  return reservoir.empty();
}

void Smokestack::clear_stack() {
  lock_guard<mutex> lock(stack_mutex);
  while (!stack.empty())
    stack.pop();
}

void Smokestack::clear_reservoir() {
  lock_guard<mutex> lock(reservoir_mutex);
  reservoir.clear();
}

void Smokestack::clear() {
  lock_guard<mutex> stack_lock(stack_mutex);
  lock_guard<mutex> reservoir_lock(reservoir_mutex);
  while (!stack.empty())
    stack.pop();
  reservoir.clear();
}
} // namespace smokestack
} // namespace hansen
} // namespace engineering

EMSCRIPTEN_BINDINGS(smokestack) {
  emscripten::class_<engineering::hansen::smokestack::Smokestack>("Smokestack")
      .constructor<>()
      .function("add", &engineering::hansen::smokestack::Smokestack::add)
      .function("push", &engineering::hansen::smokestack::Smokestack::push)
      .function("replenish",
                &engineering::hansen::smokestack::Smokestack::replenish)
      .function("pop", &engineering::hansen::smokestack::Smokestack::pop)
      .function("stack_size",
                &engineering::hansen::smokestack::Smokestack::stack_size)
      .function("reservoir_size",
                &engineering::hansen::smokestack::Smokestack::reservoir_size)
      .function("stack_empty",
                &engineering::hansen::smokestack::Smokestack::stack_empty)
      .function("reservoir_empty",
                &engineering::hansen::smokestack::Smokestack::reservoir_empty)
      .function("clear", &engineering::hansen::smokestack::Smokestack::clear)
      .function("clear_stack",
                &engineering::hansen::smokestack::Smokestack::clear_stack)
      .function("clear_reservoir",
                &engineering::hansen::smokestack::Smokestack::clear_reservoir);
}
