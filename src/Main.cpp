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
#include    <sys/time.h>
#include    "Context.h"
#include    "EventHandler.h"
#include    "Object.h"
#include    "Font.h"
#include    "Text.h"
#include    "Octree.h"

    int
main ( int argc, char *argv[] )
{
    {
        Octree<int> test(8);
        int q = 1;
        for ( int i = 0; i < 8; i += 1 ) {
            for ( int j = 0; j < 8; j += 1 ) {
                for ( int k = 0; k < 8; k += 1 ) {
                    test.insert(q, k, j, i);
                    q += 1;
                }
            }
        }
        test.remove(0, 0, 0);
        test.print();
        std::cout << "\n";
        std::cout << "\n";
        test.insert(5, 0, 0, 0);
        test.print();
        std::cout << "\n";
        std::cout << "\n";
    }
    exit(1);
    int w = 1024, h = 768;
    std::string windowName = "Tech demo";

    Context windowContext(w, h, windowName);
    windowContext.setup();
//    windowContext.setFramerateLimit(60);

    int xObjects = 10;
    int yObjects = 10;
    std::vector<Object> objects;
    for ( int i = 0; i < xObjects; i += 1 ) {
        for ( int j = 0; j < yObjects; j += 1 ) {
            objects.push_back(Object("data/dirt.dat", windowContext, i, 0, j));
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

    timeval t1, t2;
    while ( windowContext.isOpened() ) {
        gettimeofday(&t1, NULL);
        event.pollEvents();
        windowContext.clear();
        for ( int i = 0; i < xObjects*yObjects; i += 1 ) {
            objects[i].draw();
        }
        windowContext.render();
        gettimeofday(&t2, NULL);
        double diff = t2.tv_usec - t1.tv_usec;
        std::cout << diff / 1000 << "\n";
    }
    return 0;
}				// ----------  end of function main  ----------
