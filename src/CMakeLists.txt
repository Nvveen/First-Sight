set(firstsight_SRCS
    Camera.cpp
    Context.cpp
    EventHandler.cpp
    Font.cpp
    Main.cpp
    Model.cpp
    Object.cpp
    Shader.cpp
    Texture.cpp)

add_executable(firstsight ${firstsight_SRCS})
target_link_libraries(firstsight ${SDL_LIBRARY} ${SDLTTF_LIBRARY}
    ${GLEW_LIBRARY} ${PNG_LIBRARY})
