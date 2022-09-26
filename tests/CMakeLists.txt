set(target_name libcsc_test)

add_executable(
  ${target_name}
  libcsc/PackedVectorTest.cpp
  libcsc/PackedIteratorTest.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_link_libraries(
  ${target_name}
  PRIVATE
    packed_vector
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(${target_name})
