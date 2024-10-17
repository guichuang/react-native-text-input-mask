cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE on)


file(GLOB text_input_mask_SRC CONFIGURE_DEPENDS *.cpp ${CMAKE_CURRENT_SOURCE_DIR}/**/*.h)

add_library(text_input_mask SHARED ${text_input_mask_SRC})
target_include_directories(text_input_mask PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(text_input_mask PUBLIC rnoh)
