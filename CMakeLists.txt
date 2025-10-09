cmake_minimum_required(VERSION 3.28)
project(Battlecity LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main WIN32 src/main.cpp)
target_link_options(main PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/ENTRY:mainCRTStartup>
)
set_target_properties(main PROPERTIES OUTPUT_NAME "battlecity")
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
target_link_libraries(main PRIVATE SFML::Audio)
install(TARGETS main DESTINATION bin)
