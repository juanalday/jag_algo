CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)
PROJECT(jag_algo CXX)

option(ENABLE_TEST "Enable tests" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ENABLE_TEST)
  include(FetchContent)
  FetchContent_Declare(googletest
     GIT_REPOSITORY https://github.com/google/googletest.git
	 GIT_TAG main
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  #add_subdirectory(googletest.git)
  enable_testing()
  include(GoogleTest)
endif()

add_subdirectory(algorithms)
