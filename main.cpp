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
#include    <SFML/Graphics.hpp>
#include    "Context.h"
#include    "EventHandler.h"
#include    "Object.h"

    int
main ( int argc, char *argv[] )
{
    int w = 1024, h = 768;
    std::string windowName = "Tech demo";

    Context windowContext(w, h, windowName);
    windowContext.setup();

    Object cube("data/dirt.dat");
    windowContext.push(cube);
    EventHandler event(windowContext);
    Camera *cam = windowContext.getCam();
    event.bind(cam, &Camera::move, 0.3f, 0.0f, 0.0f, Code::Left);
    event.bind(cam, &Camera::move, -0.3f, 0.0f, 0.0f, Code::Right);
    event.bind(cam, &Camera::move, 0.0f, 0.0f, 0.3f, Code::Up);
    event.bind(cam, &Camera::move, 0.0f, 0.0f, -0.3f, Code::Down);
    event.bind(cam, &Camera::move, 0.0f, 0.3f, 0.0f, Code::PageUp);
    event.bind(cam, &Camera::move, 0.0f, -0.3f, 0.0f, Code::PageDown);
    event.bind(cam, &Camera::rotate, 45, 0.0f, 1.0f, 0.0f, Code::Comma);
    event.bind(cam, &Camera::rotate, 315, 0.0f, 1.0f, 0.0f, Code::Period);

    while ( windowContext.isOpened() ) {
        event.pollEvents();
        windowContext.render();
    }
    return 0;
}				// ----------  end of function main  ----------
