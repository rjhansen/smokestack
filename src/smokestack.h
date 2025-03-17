/* Copyright 2025, Robert J. Hansen <rob@hansen.engineering>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
