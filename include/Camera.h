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
//       Filename:  Camera.h
// 
//    Description:  A class that handles the ingame camera.
// 
//        Version:  1.0
//        Created:  04/22/2011 12:17:16 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  CAMERA_H
#define  CAMERA_H

#include    <iostream>
#include    <array>
#include    <glm/glm.hpp>
#include    <GL/gl.h>

// ============================================================================
//        Class:  Camera
//  Description:  Camera is a simple class like projection, that handles camera
//                views, rotating and moving around the camera.
// ============================================================================
class Camera
{
    public:
        typedef std::array<GLfloat, 3> CamVector;
        Camera ( CamVector pos=CamVector{0.0f, 200.0f, -200.0f},
                 CamVector target=CamVector{0.0f, 0.0f, 0.0f},
                 CamVector up=CamVector{0.0f, 1.0f, 0.0f} ); 

        void rotate ( GLfloat angle );
        void move ( GLfloat x, GLfloat y, GLfloat z );
        void zoom ( GLfloat amount );
        bool changed ();
        void clearUpdate ();

        glm::mat4& getMatrix ();
        glm::vec3 getPosition ();
        glm::vec3 getDirection ();

    private:
        struct {
            glm::vec3 pos;
            glm::vec3 target;
            glm::vec3 up;
        } cameraVectors_;

        glm::mat4 cameraMatrix_;
        bool update_;
}; // -----  end of class Camera  -----

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  changed
// Description:  Returns whether the camera vectors have changed.
//-----------------------------------------------------------------------------
    inline bool
Camera::changed ()
{
    return update_;
}		// -----  end of method Camera::changed  -----

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  clearUpdate
// Description:  Clears the update-state.
//-----------------------------------------------------------------------------
    inline void
Camera::clearUpdate ()
{
    update_ = false;
}		// -----  end of method Camera::clearUpdate  -----

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  getMatrix
// Description:  Returns the camera matrix
//-----------------------------------------------------------------------------
    inline glm::mat4&
Camera::getMatrix ()
{
    return cameraMatrix_;
}		// -----  end of method Camera::getMatrix  -----

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  getPosition
// Description:  Returns the position of the camera.
//-----------------------------------------------------------------------------
    inline glm::vec3
Camera::getPosition ()
{
    return cameraVectors_.pos;
}		// -----  end of method Camera::getPosition  -----

    inline glm::vec3
Camera::getDirection ()
{
    return glm::vec3(cameraVectors_.pos[0]*-1,
                     cameraVectors_.pos[1]*-1,
                     cameraVectors_.pos[2]*-1);
}		// -----  end of method Camera::getDirection  -----

#endif   // ----- #ifndef CAMERA_H  -----
