// This file is part of First Sight.
// 
// First Sight is free software: you can redistribute it and/or modify it under 
// the terms of the GNU General Public License as published by the 
// Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.
// 
// First Sight is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
// more details.
// 
// You should have received a copy of the GNU General Public License along with 
// First Sight. If not, see <http://www.gnu.org/licenses/>.
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
#include    <sstream>
#include    <GL/glew.h>
#include    "Context.h"
#include    "EventHandler.h"
#include    "Object.h"
#include    "Font.h"
#include    "Text.h"

    int
main ( int argc, char *argv[] )
{
    int w = 1024, h = 768;
    std::string windowName = "Tech demo";

    Context windowContext(w, h, windowName);
    windowContext.setup();
//    windowContext.setFramerateLimit(60);

    int xObjects = 50;
    int yObjects = 50;
    Object objects[xObjects*yObjects];
    for ( int i = 0; i < xObjects; i += 1 ) {
        for ( int j = 0; j < yObjects; j += 1 ) {
            Object cube("data/dirt.dat", i, 0, j);
            cube.bind(windowContext);
            objects[i*xObjects+j] = cube;
        }
    }

    EventHandler event(windowContext);
    Camera *cam = windowContext.getCamera();
    event.bind(cam, &Camera::move, 0.3f, 0.0f, 0.0f, Key::Left);
    event.bind(cam, &Camera::move, -0.3f, 0.0f, 0.0f, Key::Right);
    event.bind(cam, &Camera::move, 0.0f, 0.0f, 0.3f, Key::Up);
    event.bind(cam, &Camera::move, 0.0f, 0.0f, -0.3f, Key::Down);
    event.bind(cam, &Camera::rotate, 45.0f, 0.0f, 1.0f, 0.0f, 
               EventHandler::Keyset(Key::Lctrl, Key::Left));
    event.bind(cam, &Camera::rotate, -45.0f, 0.0f, 1.0f, 0.0f,
               EventHandler::Keyset(Key::Lctrl, Key::Right));
    event.bind(&windowContext, &Context::close, Key::Escape);

    int frames = 0;
    int prevTime = SDL_GetTicks();
    while ( windowContext.isOpened() ) {
        event.pollEvents();
        windowContext.clear();
        frames += 1;
        for ( unsigned int i = 0; i < xObjects*yObjects; i += 1 ) {
            objects[i].draw();
        }
        windowContext.render();
        int currTime = SDL_GetTicks();
        if ( currTime - prevTime >= 1000 ) {
            prevTime = currTime;
            std::cout << frames << "\n";
            frames = 0;
        }
    }
    return 0;
}				// ----------  end of function main  ----------
