set(firstsight_SRCS
    Camera.cpp
    Context.cpp
    EventHandler.cpp
    Main.cpp
    Model.cpp
    Object.cpp
    Projection.cpp
    Shader.cpp
    Texture.cpp)

add_executable(firstsight ${firstsight_SRCS})
target_link_libraries(firstsight ${SDL_LIBRARY} ${GLEW_LIBRARY}
    ${PROJECT_SOURCE_DIR}/lib/snappy/.libs/libsnappy.so)
