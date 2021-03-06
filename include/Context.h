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
#include    <map>
#include    <GL/glew.h>
#include    <SDL/SDL.h>
#include    "Projection.h"
#include    "Camera.h"
#include    "Shader.h"

// ============================================================================
//        Class:  Context
//  Description:  Context creation class that handles the calls to the objects.
// ============================================================================
class Context
{
    public:
        Context ( GLfloat w, GLfloat h, std::string windowName,
                  Camera camera=Camera() ); 
        ~Context ();
        void setup ();
        void clear ();
        void render ();
        void resize ( int w, int h );

        bool isOpened ();
        Perspective& getPerspective ();
        Ortho& getOrtho ();
        Camera& getCamera ();

        void close ();

        friend class EventHandler;

        static std::map<std::string, Shader> shaders;

    private:
        GLfloat w_;
        GLfloat h_;
        std::string windowName_;

        Perspective pers_;
        Ortho ortho_;
        Camera cam_;

        SDL_Surface *mainWindow_;
        bool windowOpened_;
}; // -----  end of class Context  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  isOpened
// Description:  Returns whether the window is opened or not.
//-----------------------------------------------------------------------------
    inline bool
Context::isOpened ()
{
    return windowOpened_;
}		// -----  end of method Context::isOpened  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  close
// Description:  Sets the opened window bool to false, closing the window.
//-----------------------------------------------------------------------------
    inline void
Context::close ()
{
    windowOpened_ = false;
}		// -----  end of method Context::close  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  getCamera
// Description:  Returns the camera object.
//-----------------------------------------------------------------------------
    inline Camera&
Context::getCamera ()
{
    return cam_;
}		// -----  end of method Context::getCamera  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  getPerspective
// Description:  Returns the perspective object.
//-----------------------------------------------------------------------------
    inline Perspective&
Context::getPerspective ()
{
    return pers_;
}		// -----  end of method Context::getPerspective  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  getOrtho
// Description:  Returns the orthographic object.
//-----------------------------------------------------------------------------
    inline Ortho&
Context::getOrtho ()
{
    return ortho_;
}		// -----  end of method Context::getOrtho  -----

#endif   // ----- #ifndef CONTEXT_H  -----
