# Smokestack
Ken Burnside of [Ad Astra Games](https://www.adastragames.com) needs a
small amount of WebAssembly written so that his online platform for
playing Ad Astra’s space combat boardgames can support new products. I
volunteered to do the job, since I was looking to get my hands dirty
with WebAssembly.

# Requirements
Specifically, the model to use is a random stack of tokens bearing
short alphanumeric messages, along with a hat for discards. You get
to draw `N` tokens from the stack, keep a quantity of them, and toss
your discards into the hat. If the stack runs out, the hat tokens are
collected, randomized, and become the new stack without interruption.
In the unlikely event so many tokens are in play there are no hat tokens
with which to replenish, it ought signal an error by dispensing an all
capitals token reading `ERROR`.

# Implementation
Smokestack is written in bog-standard ISO C++23 with a healthy eye
towards code reliability and maintainability, using the excellent
[Emscripten](https://emscripten.org) tool to transpile it to
WebAssembly.

Random numbers are provided by the Mersenne Twister after seeding with
the C++ system’s preferred source of high quality entropy.

Smokestack is safe for multithreaded use. (I don’t know why you’d want
to, but Smokestack works just fine regardless.)

The final deliverables are `smokestack.js` and `smokestack.wasm`.

## The `Smokestack` API
| **Method name** | **Parameters** | **Returns** | **Description** |
|:----------------|:--------------:|:-----------:|----------------:|
| Constructor     | None           | None        | Use with JS `new`|
|`add`            | string         | None        | Adds to the hat |
|`push`           | string         | None        | Pushes to the stack |
|`replenish`      | None           | None        | Empties stack into hat, shuffles tokens, repopulates stack by emptying hat |
|`pop`            | None           | string      | Pops a token off the stack (potentially replenishing) |
|`stack_size`     | None           | integer     | Returns the number of tokens in the stack |
|`reservoir_size` | None           | integer     | Returns the number of tokens in the hat |
|`stack_empty`    | None           | boolean     | `true` ⟺ (0 === `.stack_size()`) |
|`reservoir_empty`| None           | boolean     | `true` ⟺ (0 === `.reservoir_size()`) |
|`clear`          | None           | None        | Empties both stack and hat |
|`clear_stack`    | None           | None        | Empties only stack |
|`clear_reservoir`| None           | None        | Empties only hat |

## Type safety
The C++23 internals are solidly typesafe, but I make no promises about
the Javascript bindings. **Be paranoid and ensure only ASCII-encoded
strings get passed to Smokestack.**

## Security considerations
`.add` and `.push` will happily let you put any ASCII string in
Smokestack. Be careful before trusting user-provided data without
adequate vetting.

# Build notes

Although it ships with a [CMake](https://www.cmake.org) build environment
that should work, it doesn’t work, due to Emscripten’s CMake support
requiring a tool Apple doesn‘t ship: `clang-scan-deps`. Other platforms
may have better luck.

Until then, build it the hard way: run `build.sh` in the root dir. Assuming
you have a sane UNIX environment with LLVM and Emscripten installed, you
should have a bouncing baby tarball, `smokestack-x.y.z.tar.xz`, waiting for
you to take it to a good home. Inside you’ll find:

* the Apache 2.0 license
* an `AUTHORS` file with copyright information and contact information,
* the C++ source code
* the WebAssembly
* the Javascript
* and an HTML page for testing.
