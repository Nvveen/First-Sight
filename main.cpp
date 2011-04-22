// ============================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  k
// 
//        Version:  1.0
//        Created:  04/04/2011 03:14:38 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <iostream>
#include    <GL/glew.h>
#include    <GL/freeglut.h>
#include    <SFML/Graphics.hpp>
#include    "Context.h"

    int
main ( int argc, char *argv[] )
{
    int w = 1024, h = 768;
    std::string windowName = "Tech demo";
    Context windowContext(w, h, windowName);
    sf::ContextSettings settings;
    settings.MajorVersion = 3;
    settings.MinorVersion = 2;
    sf::VideoMode mode(w, h, 32);
    sf::RenderWindow window(mode, windowName, sf::Style::Default, settings);
    window.SetFramerateLimit(60);
    // Catch error
    GLenum res = glewInit();
    if ( res != GLEW_OK ) {
        std::cerr << "Error: " << glewGetErrorString(res) << "\n";
        exit(1);
    }

    windowContext.setup();
    bool windowOpened = true;
    while ( window.IsOpened() && windowOpened ) {
        sf::Event event;
        while ( window.PollEvent(event) ) {
            if ( event.Type == sf::Event::Closed ) {
                windowOpened = false;
            }
            if ( event.Type == sf::Event::KeyPressed &&
                 event.Key.Code == sf::Key::Escape ) {
                windowOpened = false;
            }
            if ( event.Type == sf::Event::Resized ) {
                windowContext.resize(event.Size.Width, event.Size.Height);
            }
        }
        window.Clear();
        windowContext.render();
        window.Display();
    }
    return 0;
}				// ----------  end of function main  ----------
