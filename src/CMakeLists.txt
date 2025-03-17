set(SMOKESTACK_SRC smokestack.cpp)

add_library(smokestack)
target_sources(smokestack PUBLIC ${SMOKESTACK_SRC}
        PUBLIC FILE_SET HEADERS
        BASE_DIRS . FILES
        smokestack.h)
set_target_properties(smokestack PROPERTIES
        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
        LINK_FLAGS --bind)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    include(GenerateExportHeader)
    generate_export_header(smokestack)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_compile_definitions(smokestack PUBLIC DLL_EXPORTS)
endif()
FetchContent_MakeAvailable(googletest)

Include(GoogleTest)
add_executable(smoketest smoketest.cpp)
target_include_directories(smoketest PUBLIC .)
target_link_libraries(smoketest smokestack GTest::gtest_main)
gtest_add_tests(TARGET smoketest)
