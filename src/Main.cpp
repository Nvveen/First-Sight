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
#include    <GL/glew.h>
#include    "Context.h"
#include    "EventHandler.h"
#include    "Model.h"
#include    "Terrain.h"

    int
main ( int argc, char *argv[] )
{
    int w = 1024, h = 768;
    std::string windowName = "Tech demo";

    try {
        Context windowContext(w, h, windowName);
        windowContext.setup();

        Model test("data/dwarf.dat", windowContext, 0, 0, 0);
        unsigned int index = test.addAnimation(true);
        glm::vec3 limbPos;
        float angle;
        auto function = [&]( glm::mat4 mat )->glm::mat4 {
            glm::mat4 m = glm::translate(glm::mat4(1.0f), limbPos);
            m *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1, 0, 0));
            m *= glm::translate(glm::mat4(1.0f), -limbPos);
            mat *= m;
            return mat;
        };
        limbPos = glm::vec3(3, 11, 4);
        angle = 7.5f;
        test.animateLimb(1, index, 500, function);
        angle = -7.5f;
        test.animateLimb(1, index, 500, function);

        EventHandler event(windowContext);
        Camera& camera = windowContext.getCamera();
        event.bind([&]{ camera.move(10.0f, 0.0f, 0.0f);},
                        EventHandler::Key::Left);
        event.bind([&]{ camera.move(-10.0f, 0.0f, 0.0f);},
                        EventHandler::Key::Right);
        event.bind([&]{ camera.move(0.0f, 0.0f, 10.0f);}, 
                        EventHandler::Key::Up);
        event.bind([&]{ camera.move(0.0f, 0.0f, -10.0f);},
                        EventHandler::Key::Down);
        event.bind([&]{ camera.rotate(30.0f);},
                        EventHandler::Keyset{ EventHandler::Key::Lctrl,
                                              EventHandler::Key::Left});
        event.bind([&]{ camera.rotate(-30.0f);},
                        EventHandler::Keyset{ EventHandler::Key::Lctrl,
                                              EventHandler::Key::Right});
        event.bind([&]{ camera.zoom(10.0f);}, 
                        EventHandler::Keyset{ EventHandler::Key::Lctrl,
                                              EventHandler::Key::Up });
        event.bind([&]{ camera.zoom(-10.0f);}, 
                        EventHandler::Keyset{ EventHandler::Key::Lctrl,
                                              EventHandler::Key::Down });
        event.bind([&]{ windowContext.close();}, EventHandler::Key::Escape);

        unsigned int i = 0;
        while ( windowContext.isOpened() ) {
            if ( i == 100 ) test.startAnimation(index);
            event.pollEvents();
            windowContext.clear();
            test.draw();
            windowContext.render();
            i++;
        }
    }
    catch (...) {
        throw;
    }
    return 0;
}				// ----------  end of function main  ----------
