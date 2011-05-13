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
#include    <SDL/SDL.h>
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
                  Projection* pers=NULL, Camera* cam=NULL ); 
        ~Context ();
        void setup ();
        void clear ();
        void render ();
        void resize ( int w, int h );

        // ====================  ACCESSORS     ================================
        bool isOpened ();
        Camera* getCam ();

        // ====================  MUTATORS      ================================
        void push ( Object& obj );
        void pushOrtho ( Object& obj );
        void close ();
        void setFramerateLimit ( int limit );

        // ====================  OPERATORS     ================================

        friend class EventHandler;
        // ====================  DATA MEMBERS  ================================
        static std::map<std::string, Shader> shaders;

        static Uint8 gridWidth;
        static Uint8 gridHeight;
        static Uint8 gridDepth;

    private:
        // ====================  DATA MEMBERS  ================================
        GLfloat w_;
        GLfloat h_;
        std::string windowName_;

        Projection *pers_;
        Projection *ortho_;
        Camera *cam_;

        std::vector<Object*> objects_;
        std::vector<Object*> orthoObjects_;

        SDL_Surface *mainWindow_;
        bool windowOpened_;

        struct {
            int prevTime;
            int timeElapsed;
            int limit;
            bool limitSet;
        } fpsLimit_;
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
//      Method:  setFramerateLimit
// Description:  Sets the framerate to a certain limit, disallowing SDL to
//               render more frames in a second.
//-----------------------------------------------------------------------------
    inline void
Context::setFramerateLimit ( int limit )
{
    fpsLimit_.limitSet = true;
    // The user inputs a maximum fps, we need to convert that to the limit each
    // frame gets in time allowed to render.
    fpsLimit_.limit = 1000/limit;
}		// -----  end of method Context::setFramerateLimit  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  push
// Description:  Pushes an object onto the object stack.
//-----------------------------------------------------------------------------
    inline void
Context::push ( Object& obj )
{
    objects_.push_back(&obj);
}		// -----  end of method Context::push  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  pushOrtho
// Description:  Pushes an orthogonal object onto the object stack.
//-----------------------------------------------------------------------------
    inline void
Context::pushOrtho ( Object& obj )
{
    orthoObjects_.push_back(&obj);
}		// -----  end of method Context::pushOrtho  -----

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
