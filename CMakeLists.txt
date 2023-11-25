cmake_minimum_required(VERSION 3.11)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(avoid-meteors)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Add dependencies

# find_package(SDL2 QUIET)
# find_package(SDL2_image QUIET)
# include(FetchContent)
# set(FETCHCONTENT_QUIET FALSE)
# 
# # SDL2
# if (NOT SDL2_FOUND)
#   FetchContent_Declare(
#     SDL2
#     GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
#     GIT_TAG         release-2.28.5
#     GIT_SHALLOW     TRUE
#     GIT_PROGRESS    TRUE
#   )
# endif()
# 
# # SDL2_image
# if (NOT SDL2_image_FOUND)
#   FetchContent_Declare(
#     SDL2_image
#     GIT_REPOSITORY  https://github.com/libsdl-org/SDL_image.git
#     GIT_TAG         release-2.6.3
#     GIT_SHALLOW     TRUE
#     GIT_PROGRESS    TRUE
#   )
# endif()
# 
# # START ADDITION
# set(SDL2IMAGE_INSTALL OFF)
# set(BUILD_SHARED_LIBS FALSE)
# # END ADDITION
# 
# FetchContent_MakeAvailable(SDL2 SDL2_image)
# 
# set(SDL_LIBRARIES SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static)
add_executable(${PROJECT_NAME} src/main.cpp "include/constants.h" "include/components.h")
target_include_directories(${PROJECT_NAME} PRIVATE include)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL_LIBRARIES})

# if (WIN32)
#   add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
#     VERBATIM
#   )
# endif()