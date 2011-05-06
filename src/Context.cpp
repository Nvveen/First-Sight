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
//       Filename:  Context.cpp
// 
//    Description:  Class that handles the context creation of the window in
//                  freeglut.
// 
//        Version:  1.0
//        Created:  04/07/2011 07:55:42 PM
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
#include    "Text.h"

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  Context
// Description:  constructor
//-----------------------------------------------------------------------------
Context::Context ( GLfloat w, GLfloat h, std::string windowName,
                   Projection* proj, Camera* cam ) :
    w_(w), h_(h), windowName_(windowName)
{
    if ( proj == NULL ) proj = new Projection(w_/h_);
    proj_ = proj;
    if ( cam == NULL ) cam = new Camera;
    cam_ = cam;
    mainWindow_ = NULL;
    windowOpened = true;
}  // -----  end of method Context::Context  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  ~Context
// Description:  destructor
//-----------------------------------------------------------------------------
Context::~Context ()
{
}  // -----  end of method Context::~Context  (destructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  setup
// Description:  Before the rendering is started, we need to setup the different
//               parameters.
//-----------------------------------------------------------------------------
    void
Context::setup ()
{
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        std::cerr << "Couldn't initialize SDL.\n";
        exit(1);
    }
    mainWindow_ = SDL_SetVideoMode(w_, h_, 32, SDL_OPENGL);
    if ( mainWindow_ == NULL ) {
        std::cerr << "Error setting videomode.\n";
        exit(1);
    }

    SDL_WM_SetCaption(windowName_.c_str(), NULL);

    // Catch error
    GLenum res = glewInit();
    if ( res != GLEW_OK ) {
        std::cerr << "Error: " << glewGetErrorString(res) << "\n";
        exit(1);
    }
    // Set the different options for the objects.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Add shaders to shaderlist
    shaders["default"] = Shader("default.vs", "default.fs");
}		// -----  end of method Context::setup  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  render
// Description:  For each frame, render the scene by setting up the objects.
//-----------------------------------------------------------------------------
    void
Context::render ()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | 
            GL_STENCIL_BUFFER_BIT );

    // Draw objects
    for ( unsigned int i = 0; i < objects.size(); i += 1 ) {
        objects[i].bind(proj_, cam_);
        objects[i].draw();
    }

    SDL_GL_SwapBuffers();
}		// -----  end of method Context::render  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  resize
// Description:  Handles what action to take when the window is resized.  
//-----------------------------------------------------------------------------
    void
Context::resize ( int w, int h )
{
    glViewport(0, 0, w, h);
    w_ = (GLfloat)w;
    h_ = (GLfloat)h;
    // Set this function to resize with values it already has.
    proj_->setPerspective(w_, h_);
}		// -----  end of method Context::resize  -----

