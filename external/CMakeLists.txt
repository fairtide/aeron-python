# pybind11
set(PYBIND11_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/pybind11/include PARENT_SCOPE)

# aeron
ExternalProject_Add(aeron
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/aeron
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        BUILD_COMMAND ${CMAKE_COMMAND} --build .
        INSTALL_COMMAND ""
        BUILD_BYPRODUCTS
        "${CMAKE_CURRENT_BINARY_DIR}/aeron-prefix/src/aeron-build/lib/libaeron_client.a")

ExternalProject_Get_Property(aeron SOURCE_DIR)
set(AERON_INCLUDE_DIRS ${SOURCE_DIR}/aeron-client/src/main/cpp PARENT_SCOPE)

ExternalProject_Get_Property(aeron BINARY_DIR)
set(AERON_LIBRARIES "${BINARY_DIR}/lib/libaeron_client.a" PARENT_SCOPE)

