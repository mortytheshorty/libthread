cmake_minimum_required(VERSION 3.0.0)

project(thread C)

#-----------------------------------------------------------------------------
# Include CMake modules
#-----------------------------------------------------------------------------
include(GNUInstallDirs)
include(sources.cmake)


# variables
#set(compile_opts "-pedantic" "-Wall" "-Wextra" "-Wconversion")
#set(compile_opts_debug "-g"  "-DDEBUG")
#set(linked_deps "pthread")

#-----------------------------------------------------------------------------
# Compose CFLAGS
#-----------------------------------------------------------------------------
set(LT_C_FLAGS -Wall -Wsign-compare -Wextra -Wshadow
               -Wbad-function-cast -Wcast-align
               -Wstrict-prototypes -Wpointer-arith -Wsystem-headers -DDEBUG)
set(CMAKE_C_FLAGS_DEBUG "-g3")
set(CMAKE_C_FLAGS_RELEASE "-O3 -funroll-loops -fomit-frame-pointer")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g3 -O2")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os")


# build commands for shared library
add_library(objects_shared OBJECT 
    ${SOURCES} 
    ${PUBLIC_HEADERS}
    ${PRIVATE_HEADERS}
)

add_library(objects_static OBJECT 
    ${SOURCES} 
    ${PUBLIC_HEADERS}
    ${PRIVATE_HEADERS}
)

target_include_directories(objects_shared PRIVATE "headers" "src/headers")
target_compile_options(objects_shared PRIVATE ${LT_C_FLAGS} "-fPIC")
target_link_libraries(objects_shared ${linked_deps})
add_library(thread_shared SHARED $<TARGET_OBJECTS:objects_shared>)

# build commands for static library
target_include_directories(objects_static PRIVATE "headers" "src/headers")
target_compile_options(objects_static PRIVATE ${compile_opts} ${compile_opts_debug})
target_link_libraries(objects_static ${linked_deps})
add_library(thread_static STATIC $<TARGET_OBJECTS:objects_static>)


# build commands for test executables
add_executable(simple_thread_test "src/test/simple_test.c")
target_include_directories(simple_thread_test PRIVATE "headers")
target_link_libraries(simple_thread_test thread_shared "pthread")

add_executable(tp_test "src/test/tp_test.c")
target_include_directories(tp_test PRIVATE "headers")
target_link_libraries(tp_test thread_shared "pthread")

