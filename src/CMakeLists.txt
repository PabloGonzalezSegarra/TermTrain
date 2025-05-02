
aux_source_directory("${CMAKE_CURRENT_LIST_DIR}/core/" SOURCES)

add_executable(termtrain main.c ${SOURCES})
target_include_directories(termtrain PRIVATE "${CMAKE_CURRENT_LIST_DIR}/core/")

set (CMAKE_C_FLAGS "-Wextra -Wall -Werror")