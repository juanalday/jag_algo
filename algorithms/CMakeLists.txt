
add_library(jag_algorithm INTERFACE 
  lcp_array.hpp
  suffix_array.hpp
  suffix_automaton.hpp)
if(WIN32)
target_compile_options(jag_algorithm INTERFACE /Zc:preprocessor /Zc:__cplusplus)
endif()

add_executable(algorithms.t
  lcp_array.t.cpp
  suffix_array.t.cpp
  suffix_automaton.t.cpp)

target_link_libraries(algorithms.t jag_algorithm gmock gtest gtest_main)
gtest_discover_tests(algorithms.t)

