cmake_minimum_required(VERSION 3.12)
project(tguy VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(libtguy)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE TGuy::TGuy)

include(CPackComponent)
cpack_add_component(tguy_cli DISPLAY_NAME "CLI" DESCRIPTION "TrashGuyC CLI" REQUIRED)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} COMPONENT tguy_cli)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/UNLICENSE")
set(CPACK_COMPONENTS_ALL tguy_cli)

# Only include after setting up the variables
include(CPack)
