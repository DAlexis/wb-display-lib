cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project("wbdl-full")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

# This is to run all tests using 'make run-unit-tests' command.
# Another way is simply run 'ctest -VV', but output is not colorized by default :(
# This project may be a part of other cmake project so this trick may be implemented yet
if (NOT COMMAND add_unit_test)
	add_custom_target(run-unit-tests)
	macro(add_unit_test test_target)
		add_custom_target(${test_target}_runtest
	    	COMMAND ${test_target} 
	    	DEPENDS ${test_target}
	    	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
		add_dependencies(run-unit-tests ${test_target}_runtest)
	endmacro()
endif()

if (NOT COMMAND add_functional_test)
	add_custom_target(run-functional-tests)
	macro(add_functional_test test_script script_argument)
	    set(command_to_run ./${test_script})
	    set(target_name ${test_script}_${script_argument}_run_func_test)
	    string(REPLACE "/" "_" target_name ${target_name})
	  	add_custom_target(${target_name}
	    	COMMAND ${command_to_run} ${script_argument}
	      	WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
	  	add_dependencies(run-functional-tests ${target_name})
	endmacro()
endif()

## This macro creates variable ${PROJECT_NAME}_INCLUDE_DIRS with list of 
# include directories exported by project
macro(export_include_dirs)
    set(${PROJECT_NAME}_INCLUDE_DIRS
        ${ARGV}
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
endmacro()

# This macro creates variable ${PROJECT_NAME}_LIBRARY with list of libraries exported 
# by project
macro(export_libs)
    set(${PROJECT_NAME}_LIBRARY
        ${ARGV}
        CACHE INTERNAL "${PROJECT_NAME}: used and exported libs" FORCE)
endmacro()

add_subdirectory(wbdl)

# To enable ctest usage
enable_testing()

add_subdirectory(tests)
#add_subdirectory(functional-tests)
