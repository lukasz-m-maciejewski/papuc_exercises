add_library(lm-ch290-lib STATIC
  InstructionBuilderCombinator.cpp
  InstructionBuilder.cpp
  LoadInstructionBuilder.cpp
  OutputInstructionBuilder.cpp
  RotateLeftCyclicInstructionBuilder.cpp
  RotateRightCyclicInstructionBuilder.cpp
  DecrementAndJumpInstructionBuilder.cpp
  Processor.cpp)

add_executable(lm-ch290-test
  InstructionBuilderCombinatorTest.cpp
  LoadInstructionBuilderTest.cpp
  RotateLeftCyclicInstructionBuilderTest.cpp
  RotateRightCyclicInstructionBuilderTest.cpp
  DecrementAndJumpInstructionBuilderTest.cpp
  ProcessorTest.cpp)

target_link_libraries(lm-ch290-test lm-ch290-lib gtest_main gmock)

add_executable(lm-ch290
  main.cpp)

target_link_libraries(lm-ch290 lm-ch290-lib)
add_test(lm-ch290-test ${EXECUTABLE_OUTPUT_PATH}/lm-ch290-test)
