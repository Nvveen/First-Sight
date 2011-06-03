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
//       Filename:  Projection.h
// 
//    Description:  A class that handles the projection of the objects in 3 
//                  dimensional space, along with the camera.
// 
//        Version:  1.0
//        Created:  04/08/2011 06:05:34 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  PROJECTION_H
#define  PROJECTION_H

#include    <GL/gl.h>
#include    <glm/glm.hpp>
#include    <glm/gtc/matrix_transform.hpp>

// ============================================================================
//        Class:  Projection
//  Description:  The base projection class.
// ============================================================================
class Projection
{
    public:

        // ====================  LIFECYCLE     ================================
        Projection ( GLfloat w, GLfloat h ) : w_(w), h_(h) {}      // constructor
        virtual void init() = 0;

        // ====================  ACCESSORS     ================================
        virtual glm::mat4& getMatrix ();

        // ====================  MUTATORS      ================================
        virtual void setAspectRatio ( GLfloat w, GLfloat h );
    protected:
        // ====================  DATA MEMBERS  ================================
        glm::mat4 matrix_;
        GLfloat w_;
        GLfloat h_;
}; // -----  end of class Projection  -----

// ============================================================================
//        Class:  Perspective
//  Description:  Normal perspective class, derived from Projection.
// ============================================================================
class Perspective : public Projection
{
    public:

        // ====================  LIFECYCLE     ================================
        Perspective ( GLfloat w, GLfloat h, GLfloat FOV=45.0f, 
                      GLfloat zNear=0.1f, GLfloat zFar=1000.0f ) :
                    Projection(w,h), FOV_(FOV), zNear_(zNear), zFar_(zFar)
        {
            init ();
        }
        virtual void init ();
    private:
        // ====================  DATA MEMBERS  ================================
        GLfloat FOV_;
        GLfloat zNear_;
        GLfloat zFar_;
}; // -----  end of class Perspective  -----

// ============================================================================
//        Class:  Ortho
//  Description:  Ortographic class, derived from Projection.
// ============================================================================
class Ortho : public Projection
{
    public:

        // ====================  LIFECYCLE     ================================
        Ortho ( GLfloat w, GLfloat h ) : Projection(w, h)
        {
            init ();
        }
        virtual void init ();
}; // -----  end of class Ortho  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  getMatrix
// Description:  Accessor method for accessing the contained matrix.
//-----------------------------------------------------------------------------
    inline glm::mat4&
Projection::getMatrix ()
{
    return matrix_;
}		// -----  end of method Projection::getMatrix  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  setAspectRatio
// Description:  Mutator method for resetting the aspect ratio of the
//               projection.
//-----------------------------------------------------------------------------
    inline void
Projection::setAspectRatio ( GLfloat w, GLfloat h )
{
    w_ = w; h_ = h;
    init();
}		// -----  end of method Projection::setAspectRatio  -----

//-----------------------------------------------------------------------------
//       Class:  Perspective
//      Method:  init
// Description:  Initialize the perspective matrix.
//-----------------------------------------------------------------------------
    inline void
Perspective::init ()
{
    matrix_ = glm::perspective(FOV_, w_/h_, zNear_, zFar_);
}		// -----  end of method Perspective::init  -----

//-----------------------------------------------------------------------------
//       Class:  Ortho
//      Method:  init
// Description:  Initialize the orthographic matrix.
//-----------------------------------------------------------------------------
    inline void
Ortho::init ()
{
    matrix_ = glm::ortho(0.0f, w_, h_, 0.0f);
}		// -----  end of method Ortho::init  -----

#endif   // ----- #ifndef PROJECTION_H  -----
