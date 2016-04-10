# This is an automatically generated file

add_executable(test_name_incr test_name_incr.c)
target_link_libraries(test_name_incr ${TEST_LIBS})

add_executable(test_version_queries test_version_queries.c)
target_link_libraries(test_version_queries ${TEST_LIBS})

add_executable(test_ok test_sys_tests/test_ok.c)
target_link_libraries(test_ok ${TEST_LIBS})

add_executable(test_rc test_sys_tests/test_rc.c)
target_link_libraries(test_rc ${TEST_LIBS})

add_executable(test_diff test_sys_tests/test_diff.c)
target_link_libraries(test_diff ${TEST_LIBS})

add_executable(test_mem_ok test_sys_tests/test_mem_ok.c)
target_link_libraries(test_mem_ok ${TEST_LIBS})

add_executable(test_mem_err test_sys_tests/test_mem_err.c)
target_link_libraries(test_mem_err ${TEST_LIBS})

