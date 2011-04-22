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
#include    "Context.h"

// First, we have to setup a simple clean wrapper for the callback functions
// to allow them to be used inside a class.
Context* openGLContext = NULL;

    void
displayFuncWrapper ()
{
    openGLContext->render();
}		// -----  end of function displayFuncWrapper  -----

    void
reshapeFuncWrapper ( int w, int h )
{
    openGLContext->resize( w, h );
}		// -----  end of function reshapeFuncWrapper  -----

    int
main ( int argc, char *argv[] )
{
    int w = 1024, h = 768;
    std::string windowName = "Tech demo";
    openGLContext = new Context(w, h, windowName);
    // Initialize glut, the window and its modes.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(windowName.c_str());
    
    glutDisplayFunc(displayFuncWrapper);
    glutReshapeFunc(reshapeFuncWrapper);
    glutIdleFunc(displayFuncWrapper);

    // Catch error
    GLenum res = glewInit();
    if ( res != GLEW_OK ) {
        std::cerr << "Error: " << glewGetErrorString(res) << "\n";
        exit(1);
    }

    // Setup the rendering screen.
    openGLContext->setup();

    glutMainLoop();
    return 0;
}				// ----------  end of function main  ----------
