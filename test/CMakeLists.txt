include(CTest)
set(TEST_SOURCE)
file(GLOB TEST_SOURCE *.c)

add_executable(libnlp_test ${TEST_SOURCE})
target_link_libraries(libnlp_test ${PROJECT_NAME})
target_include_directories(libnlp_test ${INCLUDE_DIRECTORIES})
add_test(NAME libnlp_test COMMAND libnlp_test)
