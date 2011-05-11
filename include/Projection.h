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

#include    <GL/glew.h>
#include    <glm/glm.hpp>
#include    <glm/gtc/matrix_transform.hpp>

// ============================================================================
//        Class:  Projection
//  Description:  A projection handling class, that also handles the camera.
// ============================================================================
class Projection
{
    public:

        // ====================  LIFECYCLE     ================================
        Projection () {};
        Projection ( GLfloat aspectRatio, GLfloat FOV=45.0f,
                     GLfloat zNear=0.1f, GLfloat zFar=100.0f );
        static Projection ortho ( GLfloat width, GLfloat h );

        // ====================  ACCESSORS     ================================
        glm::mat4& getProjection ();

        // ====================  MUTATORS      ================================
        void setAspectRatio ( GLfloat width, GLfloat height );
        void setOrtho ( GLfloat w, GLfloat h );

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        void init ();
        void initOrtho ();

        struct {
            GLfloat FOV;
            GLfloat aspectRatio;
            GLfloat zNear;
            GLfloat zFar;
        } pers_;
        
        struct {
            GLfloat w;
            GLfloat h;
        } ortho_;

        glm::mat4 projectionMatrix_;
}; // -----  end of class Projection  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  Projection
// Description:  constructor
//-----------------------------------------------------------------------------
    inline
Projection::Projection ( GLfloat aspectRatio, GLfloat FOV, GLfloat zNear,
                         GLfloat zFar )
{
    pers_.aspectRatio = aspectRatio;
    pers_.FOV = FOV; 
    pers_.zNear = zNear;
    pers_.zFar = zFar;
    init();
}  // -----  end of method Projection::Projection  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  ortho
// Description:  Returns an orthogonal projection instead of a normal one.
//-----------------------------------------------------------------------------
    inline Projection
Projection::ortho ( GLfloat width, GLfloat height )
{
    Projection ortho;
    ortho.ortho_.w = width;
    ortho.ortho_.h = height;
    ortho.initOrtho();
    return ortho;
}		// -----  end of method Projection::ortho  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  init
// Description:  Initialises a basic perspective projection.
//-----------------------------------------------------------------------------
    inline void
Projection::init ()
{
    projectionMatrix_ = glm::perspective(pers_.FOV, pers_.aspectRatio, 
                                         pers_.zNear, pers_.zFar);
}		// -----  end of method Projection::init  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  initOrtho
// Description:  Initialies the projection as an orthogonal one.
//-----------------------------------------------------------------------------
    inline void
Projection::initOrtho ()
{
    projectionMatrix_ = glm::ortho(0.0f, ortho_.w, ortho_.h, 0.0f);
}		// -----  end of method Projection::initOrtho  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  getProjection
// Description:  Returns the perspective matrix.
//-----------------------------------------------------------------------------
    inline glm::mat4&
Projection::getProjection ()
{
    return projectionMatrix_;
}		// -----  end of method Projection::getProjection  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  setAspectRatio
// Description:  Sets the aspect ratio and reinitializes the projection.
//-----------------------------------------------------------------------------
    inline void
Projection::setAspectRatio ( GLfloat width, GLfloat height )
{
    pers_.aspectRatio = width/height;
    init();
}		// -----  end of method Projection::setAspectRatio  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  setOrtho
// Description:  Sets the width and height for an orthogonal projection.
//-----------------------------------------------------------------------------
    inline void
Projection::setOrtho ( GLfloat w, GLfloat h )
{
    ortho_.w = w;
    ortho_.h = h;
    initOrtho();
}		// -----  end of method Projection::setOrtho  -----

#endif   // ----- #ifndef PROJECTION_H  -----
