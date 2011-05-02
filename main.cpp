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
#include    <set>
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
    Camera *cam = windowContext.getCam();
    EventHandler event(windowContext);
    event.bind(cam, &Camera::move, 0.3f, 0.0f, 0.0f, Key::Left);
    event.bind(cam, &Camera::move, -0.3f, 0.0f, 0.0f, Key::Right);
    event.bind(cam, &Camera::move, 0.0f, 0.0f, 0.3f, Key::Up);
    event.bind(cam, &Camera::move, 0.0f, 0.0f, -0.3f, Key::Down);
    event.bind(&windowContext, &Context::close, Key::Escape);

    while ( windowContext.isOpened() ) {
        event.pollEvents();
        windowContext.render();
    }
    return 0;
}				// ----------  end of function main  ----------
