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
    proj_ = *proj;
    if ( cam == NULL ) cam = new Camera;
    cam_ = *cam;
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
    sf::ContextSettings settings;
    settings.MajorVersion = 3;
    settings.MinorVersion = 2;
    sf::VideoMode mode(w_, h_, 32);
    window_ = new sf::RenderWindow(mode, windowName_, sf::Style::Default, 
                                  settings);
    window_->SetFramerateLimit(60);
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
    window_->Clear();

    // Draw objects
    for ( unsigned int i = 0; i < objects.size(); i += 1 ) {
        objects[i].bind(&proj_, &cam_);
        objects[i].draw();
    }

    window_->Display();
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
    proj_.setPerspective(w_, h_);
}		// -----  end of method Context::resize  -----

