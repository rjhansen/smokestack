#ifndef HAS_SMOKESTACK_HEADER
#define HAS_SMOKESTACK_HEADER

#include <cstddef>
#include <mutex>
#include <random>
#include <stack>
#include <vector>

namespace engineering {
namespace hansen {
namespace smokestack {

class Smokestack {
public:
  Smokestack();
  void add(std::string token);
  void push(std::string token);
  void replenish();
  std::string pop();
  size_t stack_size();
  size_t reservoir_size();
  bool stack_empty();
  bool reservoir_empty();
  void clear();
  void clear_stack();
  void clear_reservoir();

protected:
  inline static std::mt19937 prng{std::random_device{}()};
  std::stack<std::string> stack;
  std::vector<std::string> reservoir;
  std::mutex prng_mutex, stack_mutex, reservoir_mutex;
};
} // namespace smokestack
} // namespace hansen
} // namespace engineering

#endif
