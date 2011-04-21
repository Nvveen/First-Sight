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
#include    <GL/freeglut.h>
#include    "Context.h"

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  Context
// Description:  constructor
//-----------------------------------------------------------------------------
Context::Context ( GLfloat w, GLfloat h, std::string windowName ) :
    w_(w), h_(h), windowName_(windowName)
{
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
    // Set the different options for the objects.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Specify what shader to use
    Shader* shader = new Shader("default.vs", "default.fs");
    // Add shaders to shaderlist
    shaders["default"] = *shader;
    // Clean up shaders
    delete shader;

    // Set the perspective projection and camera
    persProj.setPerspective(45.0f, w_/h_, 0.1f, 100.0f);
    persProj.setCamera();

    // Create an object and initialize it
    Object cube = Object("data/dirt.dat", shaders["default"]);
    // Add objects to objectlist
    objects.push_back(cube);
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

    GLuint shader = shaders["default"].getShaderProgram();

    // Bind the perspective matrices to the shader
    persProj.bind(shader);

    // Draw objects
    for ( unsigned int i = 0; i < objects.size(); i += 1 ) {
        objects[i].draw();
    }

    glutSwapBuffers();
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
    persProj.setPerspective(45.0f, w_/h_, 1.0f, 100.0f);
}		// -----  end of method Context::resize  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  onSpecialKey
// Description:  Handles what to do with actions that are received from the
//               mouse and keyboard as special keys.
//-----------------------------------------------------------------------------
    void
Context::onSpecialKey ( int key, int x, int y )
{
    persProj.moveCamera(key);
}		// -----  end of method Context::onSpecialKey  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  onKeyboardKey
// Description:  Handles what to do with normal keyboard actions.
//-----------------------------------------------------------------------------
    void
Context::onKeyboardKey ( unsigned char key, int x, int y )
{
    if ( key == 'q' ) {
        glutLeaveMainLoop();
    }
}		// -----  end of method Context::onKeyboardKey  -----

