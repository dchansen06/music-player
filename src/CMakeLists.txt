add_executable(client client.cpp client_functions.cpp)
add_executable(server server.cpp server_functions.cpp)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
target_link_libraries(server SDL2)
target_link_libraries(server SDL2_mixer)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

INSTALL(TARGETS client DESTINATION ${MusicPlayer_SOURCE_DIR}/build)
INSTALL(TARGETS server DESTINATION ${MusicPlayer_SOURCE_DIR}/build)
