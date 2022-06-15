# CMake generated Testfile for 
# Source directory: /home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/tlm/lt_mixed_endian
# Build directory: /home/jr/development/arquitetura/TFSim/systemc-2.3.3/build/examples/tlm/lt_mixed_endian
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(examples/tlm/lt_mixed_endian/lt_mixed_endian "/usr/bin/cmake" "-DTEST_EXE=/home/jr/development/arquitetura/TFSim/systemc-2.3.3/build/examples/tlm/lt_mixed_endian/lt_mixed_endian" "-DTEST_DIR=/home/jr/development/arquitetura/TFSim/systemc-2.3.3/build/examples/tlm/lt_mixed_endian" "-DTEST_INPUT=/home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/tlm/lt_mixed_endian/results/input.txt" "-DTEST_GOLDEN=/home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/tlm/lt_mixed_endian/results/expected.log" "-DTEST_FILTER=::" "-DDIFF_COMMAND=/usr/bin/diff" "-DDIFF_OPTIONS=-u" "-P" "/home/jr/development/arquitetura/TFSim/systemc-2.3.3/cmake/run_test.cmake")
set_tests_properties(examples/tlm/lt_mixed_endian/lt_mixed_endian PROPERTIES  FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR" _BACKTRACE_TRIPLES "/home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/CMakeLists.txt;137;add_test;/home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/tlm/lt_mixed_endian/CMakeLists.txt;61;configure_and_add_test;/home/jr/development/arquitetura/TFSim/systemc-2.3.3/examples/tlm/lt_mixed_endian/CMakeLists.txt;0;")
