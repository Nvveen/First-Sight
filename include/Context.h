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
#include    <map>
#include    <SFML/Graphics.hpp>
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

        // ====================  MUTATORS      ================================
        void push ( const Object& cube );

        // ====================  OPERATORS     ================================

        friend class EventHandler;
    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        GLfloat w_;
        GLfloat h_;
        std::string windowName_;
        sf::RenderWindow* window_;

        Projection proj_;
        Camera cam_;

        std::vector<Object> objects;
        std::map<std::string, Shader> shaders;

}; // -----  end of class Context  -----

//-----------------------------------------------------------------------------
//       Class:  Context
//      Method:  isOpened
// Description:  Returns whether the window is opened or not.
//-----------------------------------------------------------------------------
    inline bool
Context::isOpened ()
{
    return window_->IsOpened();;
}		// -----  end of method Context::isOpened  -----

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

#endif   // ----- #ifndef CONTEXT_H  -----
