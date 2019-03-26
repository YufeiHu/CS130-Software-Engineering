cmake_minimum_required(VERSION 3.10.0)
project(nuke-server)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed.
  Please make a new directory (called a build directory) and run CMake from there.
  You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
  message(STATUS "Setting debug build type by default in devel shell")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system regex log)
message(STATUS "Boost version: ${Boost_VERSION}")
# ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
# set(CMAKE_CXX_FLAGS "-g -Wall -DBOOST_LOG_DYN_LINK")

include_directories(include)


# TODO(!): Update name and srcs
add_library(server src/server.cc)
add_library(session src/session.cc)
add_library(request_handler_util src/request_handler_util.cc)
add_library(request_handler_manager src/request_handler_manager.cc)
add_library(echo_handler src/echo_handler.cc)
add_library(file_handler src/file_handler.cc)
add_library(not_found_handler src/not_found_handler.cc)
add_library(bad_request_handler src/bad_request_handler.cc)
add_library(config_parser src/config_parser.cc)
add_library(markdown src/markdown.cc)
add_library(markdown_token src/markdown_tokens.cc)
add_library(server_status src/server_status.cc)
add_library(status_handler src/status_handler.cc src/server_status.cc)
add_library(proxy_handler src/proxy_handler.cc)
add_library(meme_home_handler src/meme_home_handler.cc)
add_library(meme_new_handler src/meme_new_handler.cc)
add_library(meme_create_handler src/meme_create_handler.cc)
add_library(meme_view_handler src/meme_view_handler.cc)
add_library(meme_list_handler src/meme_list_handler.cc)
add_library(meme_delete_handler src/meme_delete_handler.cc)
add_library(db_creation_factory src/db_creation_factory.cc)
add_library(meme_handler_manager src/meme_handler_manager.cc)
add_library(sqlite_util src/sqlite_util.cc)
add_library(sqlite_connection src/sqlite_connection.cc src/sqlite_util.cc src/db_creation_factory.cc)
add_library(health_check_request_handler src/health_check_request_handler.cc)
add_library(jaro_winkler_time src/jaro_winkler_time.cc)
add_library(levenshtein_time src/levenshtein_time.cc)
add_library(search_algo src/search_algo.cc)
add_library(search_algo_factory src/search_algo_factory.cc)


# TODO(!): Update executable name, srcs, and deps
add_executable(nuke_server src/main.cc)
target_link_libraries(nuke_server server session request_handler_util request_handler_manager echo_handler file_handler not_found_handler status_handler server_status bad_request_handler markdown markdown_token config_parser meme_home_handler meme_new_handler meme_create_handler meme_view_handler meme_list_handler meme_delete_handler Boost::system Boost::regex Boost::log_setup Boost::log proxy_handler meme_handler_manager db_creation_factory sqlite_connection sqlite_util sqlite3 health_check_request_handler search_algo_factory jaro_winkler_time levenshtein_time search_algo)


# TODO(!): Update test executable name, srcs, and deps
add_executable(config_parser_test tests/config_parser_test.cc)
target_link_libraries(config_parser_test config_parser status_handler server_status gtest_main Boost::log_setup Boost::log)

add_executable(session_test tests/session_test.cc)
target_link_libraries(session_test server session request_handler_util request_handler_manager echo_handler file_handler not_found_handler status_handler server_status bad_request_handler markdown markdown_token config_parser meme_home_handler meme_new_handler meme_create_handler meme_view_handler meme_list_handler meme_delete_handler Boost::system Boost::regex Boost::log_setup Boost::log proxy_handler meme_handler_manager db_creation_factory sqlite_connection sqlite_util sqlite3 health_check_request_handler search_algo_factory jaro_winkler_time levenshtein_time search_algo gtest_main)

add_executable(request_handler_util_test tests/request_handler_util_test.cc)
target_link_libraries(request_handler_util_test request_handler_util gtest_main)

add_executable(echo_handler_test tests/echo_handler_test.cc)
target_link_libraries(echo_handler_test echo_handler request_handler_util gtest_main Boost::system Boost::log_setup Boost::log)

add_executable(health_check_request_handler_test tests/health_check_request_handler_test.cc)
target_link_libraries(health_check_request_handler_test health_check_request_handler request_handler_util gtest_main Boost::system Boost::log_setup Boost::log)

add_executable(file_handler_test tests/file_handler_test.cc)
target_link_libraries(file_handler_test file_handler markdown markdown_token request_handler_util gtest_main Boost::system Boost::regex Boost::log_setup Boost::log)

add_executable(not_found_handler_test tests/not_found_handler_test.cc)
target_link_libraries(not_found_handler_test not_found_handler request_handler_util gtest_main Boost::system Boost::regex Boost::log_setup Boost::log)

add_executable(bad_request_handler_test tests/bad_request_handler_test.cc)
target_link_libraries(bad_request_handler_test bad_request_handler request_handler_util gtest_main Boost::system Boost::regex Boost::log_setup Boost::log)

add_executable(server_status_test tests/server_status_test.cc)
target_link_libraries(server_status_test server_status status_handler request_handler_util gtest_main Boost::system Boost::regex Boost::log_setup Boost::log)

add_executable(status_handler_test tests/status_handler_test.cc)
target_link_libraries(status_handler_test server_status status_handler request_handler_util gtest_main Boost::system Boost::regex Boost::log_setup Boost::log)

add_executable(proxy_handler_test tests/proxy_handler_test.cc)
target_link_libraries(proxy_handler_test server_status proxy_handler Boost::system Boost::regex Boost::log_setup Boost::log markdown markdown_token request_handler_util gtest_main)

add_executable(meme_create_handler_test tests/meme_create_handler_test.cc)
target_link_libraries(meme_create_handler_test server_status meme_create_handler Boost::system Boost::regex Boost::log_setup Boost::log markdown markdown_token request_handler_util gtest_main  meme_handler_manager db_creation_factory sqlite_connection sqlite_util sqlite3)

# add_executable(meme_new_handler_test tests/meme_new_handler_test.cc)
# target_link_libraries(meme_new_handler_test server_status meme_new_handler Boost::system Boost::regex Boost::log_setup Boost::log markdown markdown_token request_handler_util gtest_main meme_handler_manager db_creation_factory sqlite_connection sqlite_util sqlite3 search_algo_factory jaro_winkler_time levenshtein_time search_algo)

# add_executable(meme_list_handler_test tests/meme_list_handler_test.cc)
# target_link_libraries(meme_list_handler_test server_status meme_list_handler Boost::system Boost::regex Boost::log_setup Boost::log markdown markdown_token request_handler_util gtest_main meme_handler_manager db_creation_factory sqlite_connection sqlite_util sqlite3 search_algo_factory jaro_winkler_time levenshtein_time search_algo)

# add_executable(meme_view_handler_test tests/meme_view_handler_test.cc)
# target_link_libraries(meme_view_handler_test server_status meme_view_handler Boost::system Boost::regex Boost::log_setup Boost::log markdown markdown_token request_handler_util gtest_main meme_handler_manager db_creation_factory sqlite_connection sqlite_util sqlite3 search_algo_factory jaro_winkler_time levenshtein_time search_algo)


# TODO(!): Update with test binary
gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(session_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_util_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(echo_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(health_check_request_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(file_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(not_found_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(bad_request_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(status_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(server_status_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(proxy_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(meme_create_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# gtest_discover_tests(meme_new_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# gtest_discover_tests(meme_list_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# gtest_discover_tests(meme_view_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)


# TODO(!): Add integration tests
add_test(NAME IntegrationTest COMMAND bash ./integration_test.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_test(NAME MultiThreadTest COMMAND bash multithread_test.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_test(NAME MultiCreateTest COMMAND bash multi_create_test.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)


# TODO(!): Update with target/test targets
include(cmake/CodeCoverageReportConfig.cmake)
# generate_coverage_report(TARGETS config_parser session request_handler_util request_handler_manager server_status echo_handler file_handler status_handler not_found_handler bad_request_handler proxy_handler meme_home_handler meme_new_handler meme_create_handler meme_view_handler meme_list_handler meme_handler_manager TESTS echo_handler_test file_handler_test request_handler_util_test config_parser_test not_found_handler_test bad_request_handler_test server_status_test status_handler_test proxy_handler_test )

generate_coverage_report(TARGETS config_parser session request_handler_util request_handler_manager server_status echo_handler file_handler status_handler not_found_handler bad_request_handler proxy_handler meme_home_handler meme_new_handler meme_create_handler meme_view_handler meme_list_handler meme_handler_manager TESTS meme_create_handler_test config_parser_test session_test request_handler_util_test echo_handler_test file_handler_test not_found_handler_test bad_request_handler_test status_handler_test server_status_test proxy_handler_test health_check_request_handler_test)

# generate_coverage_report(TARGETS config_parser session request_handler_util request_handler_manager server_status echo_handler file_handler status_handler not_found_handler bad_request_handler proxy_handler meme_home_handler meme_new_handler meme_create_handler meme_view_handler meme_list_handler meme_handler_manager TESTS echo_handler_test file_handler_test request_handler_util_test session_test config_parser_test not_found_handler_test bad_request_handler_test server_status_test server_status_test status_handler_test proxy_handler_test meme_create_handler_test meme_view_handler_test health_check_request_handler)
