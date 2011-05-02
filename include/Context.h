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
#include    <vector>
#include    <SDL.h>
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
        void close ();

        // ====================  OPERATORS     ================================

        friend class EventHandler;
    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        GLfloat w_;
        GLfloat h_;
        std::string windowName_;

        Projection* proj_;
        Camera* cam_;

        std::vector<Object> objects;
        std::map<std::string, Shader> shaders;

        SDL_Window *mainWindow_;
        SDL_GLContext mainContext_;
        bool windowOpened;

}; // -----  end of class Context  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  isOpened
// Description:  Returns whether the window is opened or not.
//-----------------------------------------------------------------------------
    inline bool
Context::isOpened ()
{
    return windowOpened;
}		// -----  end of method Context::isOpened  -----

//-----------------------------------------------------------------------------
//       Class:  Context
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
