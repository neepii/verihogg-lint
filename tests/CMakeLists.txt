add_executable(class_checks_test class_checks_test.cpp ${LINTER_SRC})

target_link_libraries(class_checks_test PRIVATE GTest::gtest_main lint)

include(GoogleTest)
gtest_discover_tests(class_checks_test)
