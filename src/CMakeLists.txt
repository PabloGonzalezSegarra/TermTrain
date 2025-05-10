
aux_source_directory("${CMAKE_CURRENT_LIST_DIR}/core/" CORE_SOURCES)

aux_source_directory("${CMAKE_CURRENT_LIST_DIR}/core/" UTIL_SOURCES)

add_executable(termtrain main.c ${CORE_SOURCES} ${UTIL_SOURCES})
target_include_directories(termtrain PRIVATE "${CMAKE_CURRENT_LIST_DIR}/core/")

set (CMAKE_C_FLAGS "-Wextra -Wall -Werror")