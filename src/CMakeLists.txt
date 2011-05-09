set(firstsight_SRCS
    Camera.cpp
    Context.cpp
    EventHandler.cpp
    Main.cpp
    Model.cpp
    Object.cpp
    Projection.cpp
    Shader.cpp
    Text.cpp
    Texture.cpp)

add_executable(firstsight ${firstsight_SRCS})
target_link_libraries(firstsight ${SDL_LIBRARY} ${SDLTTF_LIBRARY}
    ${GLEW_LIBRARY} ${PNG_LIBRARY}
    ${PROJECT_SOURCE_DIR}/lib/snappy/.libs/libsnappy.so)
