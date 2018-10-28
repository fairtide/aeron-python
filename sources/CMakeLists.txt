include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${PYBIND11_INCLUDE_DIRS})
include_directories(${FMT_INCLUDE_DIRS})
include_directories(${AERON_INCLUDE_DIRS})
include_directories(${AERON_ARCHIVE_CLIENT_INCLUDE_DIRS})

set(CMAKE_SHARED_MODULE_PREFIX)

set(modules_SRC "")
file(GLOB_RECURSE modules_SRC "*.cpp")

foreach(module_file ${modules_SRC})
    get_filename_component (name_without_extension ${module_file} NAME_WE)
    add_library(${name_without_extension} MODULE
            ${modules_SRC})

    target_link_libraries(${name_without_extension}
            ${PYTHON_LIBRARIES}
            ${SYSCALL_INTERCEPT_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
            ${RT_LIBRARY}
            ${FMT_LIBRARIES}
            ${AERON_LIBRARIES}
            ${AERON_ARCHIVE_CLIENT_LIBRARIES})

    add_dependencies(${name_without_extension}
            fmt
            aeron
            aeron-archive-client)

    list(APPEND modules ${name_without_extension})
endforeach()