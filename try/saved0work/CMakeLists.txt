#cmake_minimum_required(VERSION 3.20)
#cmake_minimum_required(VERSION 3.18)
cmake_minimum_required(VERSION 3.0.0)

set(F0_PROJECT_NAME projectF0)

set(F0_LIBRARIES 
        permutation
        dimension
        tens0neD
)
set(F0_CUDA_LIBRARIES 
        tensCuda
        d_tensCuda
)

set(F0_SOURCE_DIR 
    src
)



project(${F0_PROJECT_NAME})

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

foreach(libvarcu  ${F0_CUDA_LIBRARIES})
    list(APPEND F0_SOURCES_CU ${F0_SOURCE_DIR}/tensor/tensCuda/${libvarcu}.cu)
endforeach()

foreach(libvar  ${F0_LIBRARIES})
    if( ${libvar} STREQUAL "tens0neD" )
        list(APPEND F0_SOURCES_CPP ${F0_SOURCE_DIR}/tensor/${libvar}/${libvar}.cpp)
    else()
        list(APPEND F0_SOURCES_CPP ${F0_SOURCE_DIR}/${libvar}/${libvar}.cpp)
        endif()
endforeach()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#find_package(CUDA REQUIRED) # no need
enable_language(CUDA)

#if version < 3.18
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
  set(CMAKE_CUDA_ARCHITECTURES 75)
endif()

add_library(${F0_PROJECT_NAME} STATIC ${F0_SOURCES_CPP} ${F0_SOURCES_CU}) # no need if add sources in executable



add_executable(isgood test/isgood.cu) # if library set, no need of ${F0_SOURCES_CPP} ${F0_SOURCES_CU}) but if not you nedd to add theses files src

target_link_libraries(isgood   gtest_main gmock_main pthread  ${F0_PROJECT_NAME}) # no need if no local library set

enable_testing()
add_test(Tester isgood)