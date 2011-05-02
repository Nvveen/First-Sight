// ============================================================================
// 
//       Filename:  Context.h
// 
//    Description:  Class that handles the context creation of the window in
//                  freeglut.
// 
//        Version:  1.0
//        Created:  04/07/2011 07:56:12 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  CONTEXT_H
#define  CONTEXT_H

#include    <string>
#include    <vector>
<<<<<<< HEAD
#include    <SFML/Graphics.hpp>
=======
#include    <SDL.h>
>>>>>>> sdl
#include    "Projection.h"
#include    "Camera.h"
#include    "Object.h"
#include    "Shader.h"

// ============================================================================
//        Class:  Context
//  Description:  Context creation class that handles the calls to the objects.
// ============================================================================
class Context
{
    public:

        // ====================  LIFECYCLE     ================================
        Context ( GLfloat w, GLfloat h, std::string windowName,
                  Projection* proj=NULL, Camera* cam=NULL ); 
        ~Context ();
        void setup ();
        void render ();
        void resize ( int w, int h );

        // ====================  ACCESSORS     ================================
        bool isOpened ();
        Camera* getCam ();

        // ====================  MUTATORS      ================================
        void push ( const Object& cube );
<<<<<<< HEAD
=======
        void close ();
>>>>>>> sdl

        // ====================  OPERATORS     ================================

        friend class EventHandler;
    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        GLfloat w_;
        GLfloat h_;
        std::string windowName_;
<<<<<<< HEAD
        sf::RenderWindow* window_;
=======
>>>>>>> sdl

        Projection* proj_;
        Camera* cam_;

        std::vector<Object> objects;
        std::map<std::string, Shader> shaders;

<<<<<<< HEAD
=======
        SDL_Window *mainWindow_;
        SDL_GLContext mainContext_;
        bool windowOpened;

>>>>>>> sdl
}; // -----  end of class Context  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  isOpened
// Description:  Returns whether the window is opened or not.
//-----------------------------------------------------------------------------
    inline bool
Context::isOpened ()
{
<<<<<<< HEAD
    return window_->IsOpened();;
=======
    return windowOpened;
>>>>>>> sdl
}		// -----  end of method Context::isOpened  -----

//-----------------------------------------------------------------------------
//       Class:  Context
<<<<<<< HEAD
=======
//      Method:  close
// Description:  Sets the opened window bool to false, closing the window.
//-----------------------------------------------------------------------------
    inline void
Context::close ()
{
    windowOpened = false;
}		// -----  end of method Context::close  -----

//-----------------------------------------------------------------------------
//       Class:  Context
>>>>>>> sdl
//      Method:  push
// Description:  Pushes an object onto the object stack.
//-----------------------------------------------------------------------------
    inline void
Context::push ( const Object& cube )
{
    objects.push_back(cube);
}		// -----  end of method Context::push  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  getCamera
// Description:  Returns the camera object.
//-----------------------------------------------------------------------------
    inline Camera*
Context::getCam ()
{
    return cam_;
}		// -----  end of method Context::getCamera  -----

#endif   // ----- #ifndef CONTEXT_H  -----
