# CMake generated Testfile for 
# Source directory: /home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/tlm/lt_extension_mandatory
# Build directory: /home/jr/development/arquitetura/TFSim/systemc-2.3.3/build/examples/tlm/lt_extension_mandatory
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(examples/tlm/lt_extension_mandatory/lt_extension_mandatory "/usr/bin/cmake" "-DTEST_EXE=/home/jr/development/arquitetura/TFSim/systemc-2.3.3/build/examples/tlm/lt_extension_mandatory/lt_extension_mandatory" "-DTEST_DIR=/home/jr/development/arquitetura/TFSim/systemc-2.3.3/build/examples/tlm/lt_extension_mandatory" "-DTEST_INPUT=" "-DTEST_GOLDEN=/home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/tlm/lt_extension_mandatory/results/expected.log" "-DTEST_FILTER=" "-DDIFF_COMMAND=/usr/bin/diff" "-DDIFF_OPTIONS=-u" "-P" "/home/jr/development/arquitetura/TFSim/systemc-2.3.3/cmake/run_test.cmake")
set_tests_properties(examples/tlm/lt_extension_mandatory/lt_extension_mandatory PROPERTIES  FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR" _BACKTRACE_TRIPLES "/home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/CMakeLists.txt;137;add_test;/home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/tlm/lt_extension_mandatory/CMakeLists.txt;55;configure_and_add_test;/home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/tlm/lt_extension_mandatory/CMakeLists.txt;0;")
