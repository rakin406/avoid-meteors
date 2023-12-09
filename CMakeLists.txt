cmake_minimum_required(VERSION 3.26)

project(avoid-meteors)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 20)

## Add dependencies

find_package(SDL2 QUIET)
find_package(SDL2_image QUIET)
find_package(flecs QUIET)
# find_package(glm QUIET)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# SDL2
if (NOT SDL2_FOUND)
  FetchContent_Declare(
    SDL2
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
    GIT_TAG         release-2.28.5
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
  )
endif()

# SDL2_image
if (NOT SDL2_image_FOUND)
  FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL_image.git
    GIT_TAG         release-2.6.3
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
  )
endif()

# flecs
if (NOT flecs_FOUND)
  FetchContent_Declare(
    flecs
    GIT_REPOSITORY  https://github.com/SanderMertens/flecs.git
    GIT_TAG         v3.2.9
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
  )
endif()

# glm
#if (NOT glm_FOUND)
#  FetchContent_Declare(
#    glm
#    GIT_REPOSITORY  https://github.com/g-truc/glm.git
#    GIT_TAG         0.9.9.8
#    GIT_SHALLOW     TRUE
#    GIT_PROGRESS    TRUE
#  )
#endif()

# START ADDITION
set(SDL2IMAGE_INSTALL OFF)
set(BUILD_SHARED_LIBS FALSE)
# END ADDITION

FetchContent_MakeAvailable(SDL2 SDL2_image flecs)
# FetchContent_MakeAvailable(SDL2 SDL2_image flecs glm)
 
set(MY_INCLUDES
  include/modules/player.h
  include/modules/renderSystem.h
  include/collisionLayer.h
  include/colors.h
  include/components.h
  include/constants.h
  include/game.h
  include/renderWindow.h
  include/tags.h
  include/tools.h
)

set(MY_SOURCES
  src/modules/player.cpp
  src/modules/renderSystem.cpp
  src/game.cpp
  src/main.cpp
  src/renderWindow.cpp
  src/tools.cpp
)

set(SDL_LIBRARIES SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static)

add_executable(${PROJECT_NAME} ${MY_SOURCES} ${MY_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/")
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL_LIBRARIES} flecs::flecs_static)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL_LIBRARIES} flecs::flecs_static glm::glm)

if (WIN32)
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    VERBATIM
  )
endif()

# Set compiler flags
if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /OPT:REF /GF)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic
    -Wshadow -Wdouble-promotion -Wformat=2
    -Wformat-truncation -Wundef -fno-common
    -Wconversion
  )
endif()
