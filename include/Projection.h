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
        void setPerspective ( GLfloat w, GLfloat h );

        // ====================  ACCESSORS     ================================
        glm::mat4& getProjection ();

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================

        struct Perspective {
            GLfloat FOV;
            GLfloat aspectRatio;
            GLfloat zNear;
            GLfloat zFar;
        } perspective;	// ----------  end of struct Perspective  ----------


        glm::mat4 projectionMatrix_;
}; // -----  end of class Projection  -----

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

#endif   // ----- #ifndef PROJECTION_H  -----
