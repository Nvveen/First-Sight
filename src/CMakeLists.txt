set(firstsight_SRCS
    Camera.cpp
    Context.cpp
    Drawable.cpp
    EventHandler.cpp
    Font.cpp
    Main.cpp
    Model.cpp
    Shader.cpp
    Terrain.cpp
    Texture.cpp)

add_executable(firstsight ${firstsight_SRCS})
target_link_libraries(firstsight ${SDL_LIBRARY} ${SDLTTF_LIBRARY}
    ${GLEW_LIBRARY} ${PNG_LIBRARY})
