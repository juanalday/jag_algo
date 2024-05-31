
add_library(jag_algorithm INTERFACE lcp_array.hpp suffix_array.hpp)
if(WIN32)
target_compile_options(jag_algorithm INTERFACE /Zc:preprocessor /Zc:__cplusplus)
endif()

add_executable(algorithms.t
  suffix_array.t.cpp
  lcp_array.t.cpp
)

target_link_libraries(algorithms.t jag_algorithm gmock gtest gtest_main)
gtest_discover_tests(algorithms.t)

