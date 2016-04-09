cmake_minimum_required(VERSION 3.0.0)
project(stupid)




if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")

	set(OPTS "-Wall -Wextra -Werror -Wno-variadic-macros")
	set(OPTS "${OPTS} -std=c++98 -pedantic -pedantic-errors -fno-strict-aliasing")
	
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${OPTS} -g -O0")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${OPTS} -O3")
	
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")

	set(OPTS "-Wall -Wextra -Weverything -Werror ")
	set(OPTS "${OPTS} -std=c++98 -pedantic -pedantic-errors")
	 
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${OPTS} -g -O0")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${OPTS} -O3")
	
endif()


set(STUPID_MATRIX_SRCS
	stupid_matrix.cpp
	run_command.cpp
	stupid_common.cpp string_utils.cpp
	stupid_tree.cpp
)

add_executable(stupid-matrix ${STUPID_MATRIX_SRCS})
target_link_libraries(stupid-matrix boost_system boost_filesystem)


set(STUPID_RUN_SRCS
	stupid_run.cpp
	run_command.cpp
	stupid_common.cpp string_utils.cpp
	stupid_tree.cpp
)

add_executable(stupid-run ${STUPID_RUN_SRCS})
target_link_libraries(stupid-run boost_system boost_filesystem)



set(STUPID_GEN_CMAKE_SRCS
	stupid_gen_cmake.cpp
	stupid_common.cpp string_utils.cpp
	stupid_tree.cpp
)

add_executable(stupid-gen-cmake ${STUPID_GEN_CMAKE_SRCS})


