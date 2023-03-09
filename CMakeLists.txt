project(new_features)
cmake_minimum_required( VERSION 3.5 )
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS "-fsanitize=undefined,address,leak -fno-omit-frame-pointer"leetcode_solutions)
#set(CMAKE_C_FLAGS "-fsanitize=undefined,address,leak -fno-omit-frame-pointer")
#set(CMAKE_Ｌ_FLAGS "-fsanitize=undefined,address,leak -fno-omit-frame-pointer")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
message(${CMAKE_SOURCE_DIR})

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

include_directories(${CMAKE_SOURCE_DIR}/inc)
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.9
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(doctest)

find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} )

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(THREAD_PATH "/usr/lib/x86_64-linux-gnu/libpthread.so.0")


add_executable(new_features main.cpp ${SRC})
target_link_libraries(new_features PRIVATE ${EXTRA_LIBS} ${THREAD_PATH} ${OpenCV_LIBS} ${GTEST_BOTH_LIBRARIES})
message(${OpenCV_LIBS})

#target_link_libraries(sort PRIVATE ${OpenCV_LIBS} ${EXTRA_LIBS})


