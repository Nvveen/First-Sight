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
#include    "Context.h"

std::map<std::string, Shader> Context::shaders;

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  Context
// Description:  constructor
//-----------------------------------------------------------------------------
Context::Context ( GLfloat w, GLfloat h, std::string windowName, Camera cam ) :
    w_(w), h_(h), windowName_(windowName), cam_(cam)
{
    pers_ = Perspective(w_, h_);
    ortho_ = Ortho(w_, h_);
    mainWindow_ = NULL;
    windowOpened_ = true;
}  // -----  end of method Context::Context  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  ~Context
// Description:  destructor
//-----------------------------------------------------------------------------
Context::~Context ()
{
    SDL_FreeSurface(mainWindow_);
    SDL_Quit();
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
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
    mainWindow_ = SDL_SetVideoMode(w_, h_, 32, SDL_OPENGL | SDL_RESIZABLE);
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_3D);

    // Add shaders to shaderlist
    shaders["default"] = Shader();
    shaders["default"].add("shaders/default.vs", GL_VERTEX_SHADER);
    shaders["default"].add("shaders/default.fs", GL_FRAGMENT_SHADER);
    shaders["default"].link();
    shaders["default"].setUniformLocation("vMVP");
    shaders["default"].setUniformLocation("texTransform");
}		// -----  end of method Context::setup  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  clear
// Description:  Clears the screen with an OpenGL call.
//-----------------------------------------------------------------------------
    void
Context::clear ()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}		// -----  end of method Context::clear  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  render
// Description:  For each frame, render the scene by setting up the objects.
//-----------------------------------------------------------------------------
    void
Context::render ()
{
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
    pers_.setAspectRatio(w_, h_);
    ortho_.setAspectRatio(w_, h_);
}		// -----  end of method Context::resize  -----

