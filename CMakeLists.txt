project(new_features)
cmake_minimum_required( VERSION 3.5 )
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-fsanitize=undefined,address,leak -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS "-fsanitize=undefined,address,leak -fno-omit-frame-pointer")
set(CMAKE_Ｌ_FLAGS "-fsanitize=undefined,address,leak -fno-omit-frame-pointer")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

#aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

include_directories(${CMAKE_SOURCE_DIR}/inc)
#aux_source_directory(${CMAKE_SOURCE_DIR}/src/thread_pool.cpp THREAD_SRC)
set(THREAD_TEST_SRC ${CMAKE_SOURCE_DIR}/src/thread_pool.cpp)
include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.9
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(doctest)

message(STATUS ${THREAD_SRC})

add_executable(thread_test ${CMAKE_SOURCE_DIR}/src/test_thread_pool.cpp ${THREAD_TEST_SRC})
target_link_libraries(thread_test doctest_with_main)


#add_executable(new_features main.cpp ${SRC})
#target_link_libraries(new_features PRIVATE ${EXTRA_LIBS})

#target_link_libraries(sort PRIVATE ${OpenCV_LIBS} ${EXTRA_LIBS})


