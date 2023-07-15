#cmake_minimum_required(VERSION 3.2)
cmake_minimum_required(VERSION 3.18)

project(ptens0neD)

include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest 
    GIT_TAG release-1.12.1
)
FetchContent_GetProperties(googletest)

if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()

#add_library(add STATIC add.cu)
add_library(permutation STATIC src/permutation/permutation.cpp)
add_library(dimension STATIC src/dimension/dimension.cpp)
add_library(tens0ne STATIC src/tens0ne/tens0ne.cpp)
add_library(cutens0ne STATIC src/cutens0ne/cutens0ne.cu)
add_library(d_cutens0ne STATIC src/cutens0ne/d_cutens0ne.cu)

#find_package(CUDA REQUIRED)
enable_language(CUDA)
#cuda_add_library(add STATIC add.cu)
#add_library(permutation STATIC ../permutation/permutation.cu)

#cuda_add_executable( )
add_executable(isgood isgood.cu device.cuh ../tensor/tensor.cpp ../permutation/permutation.cpp)
#cuda_add_executable(isgood isgood.cpp add.cu )
#target_link_libraries(isgood add  gtest_main gmock_main pthread) 
target_link_libraries(isgood   gtest_main gmock_main pthread) 

enable_testing()
add_test(Tester isgood)