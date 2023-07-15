cmake_minimum_required(VERSION 3.18)

project(ptens0neD VERSION 2023.0
              LANGUAGES CXX
              HOMEPAGE_URL "https://github.com/fanasina/ptens0neD")

#####################################################################
# DEPENDENCIES
#####################################################################

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR}
    EXCLUDE_FROM_ALL)
endif()

#####################################################################
# LIBRARY
#####################################################################

add_library(${PROJECT_NAME}
  src/ptens0neD/implementation.cpp
  )

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME}
  PUBLIC include
  PRIVATE src)

target_link_libraries(${PROJECT_NAME}
  PUBLIC Threads::Threads
  PRIVATE Boost::boost)

target_compile_options(${PROJECT_NAME}
  PRIVATE -Wall -Wextra -pedantic -Werror)

target_compile_features(${PROJECT_NAME}
  PRIVATE cxx_std_17)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  EXPORT ptens0neDTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#####################################################################
# UNIT TESTS
#####################################################################

add_executable(${PROJECT_NAME}-tests
  test/test-myclass.cpp)

target_link_libraries(${PROJECT_NAME}-tests
  PRIVATE gtest_main Threads::Threads ${PROJECT_NAME}::${PROJECT_NAME})

target_compile_options(${PROJECT_NAME}-tests
  PRIVATE -Wall -Wextra -pedantic -Werror)

target_compile_features(${PROJECT_NAME}
  PRIVATE cxx_std_17)

