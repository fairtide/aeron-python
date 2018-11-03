cmake_minimum_required(VERSION 3.7)
project(aeron-python)

set(PYTHON_VERSION 3 CACHE STRING "Target version of python")
message(STATUS "Requested python version: ${PYTHON_VERSION}")

set(RELEASE_VERSION unknown CACHE STRING "Target release version")
message(STATUS "Release version: ${RELEASE_VERSION}")

# register modules
include(CTest)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-Wno-unknown-attributes -fvisibility=hidden -fPIC)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pthread")
    set(RT_LIBRARY "rt")
endif()

# find boost
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# find python
find_package(PythonInterp ${PYTHON_VERSION})
find_package(PythonLibs ${PYTHON_VERSION})

if (PYTHONINTERP_FOUND)
    message(STATUS "Found Python interpreter: ${PYTHON_VERSION_STRING}")
else ()
    message(FATAL_ERROR "Python not found!")
endif ()

if (PYTHONLIBS_FOUND)
    message(STATUS "Found Python libs: ${PYTHONLIBS_VERSION_STRING} in ${PYTHON_LIBRARIES}")
else ()
    message(FATAL_ERROR "Python dev libs not found!")
endif ()

# setup CMake to run tests
enable_testing()

# include subdirectories
add_subdirectory(external)
add_subdirectory(sources)
add_subdirectory(tests)

# define package build targets
add_custom_target(build-package
        COMMAND
            tar -cvf ${CMAKE_BINARY_DIR}/aeron_python.tar
                sources
                external
                tests
                CMakeLists.txt
                LICENSE
                README.md
                setup.py
                PKG-INFO
        COMMAND gzip ${CMAKE_BINARY_DIR}/aeron_python.tar
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})


add_custom_target(build-wheel
        COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel -d ${CMAKE_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
