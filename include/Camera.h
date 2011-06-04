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

        Camera ();
        Camera ( GLfloat pos[3], GLfloat target[3], GLfloat up[3] ); 

        void rotate ( GLfloat angle );
        void move ( GLfloat x, GLfloat y, GLfloat z );
        void zoom ( GLfloat amount );

        glm::mat4& getCamera ();

    private:
        struct {
            glm::vec3 pos;
            glm::vec3 target;
            glm::vec3 up;
        } cameraVectors_;

        glm::mat4 cameraMatrix_;
}; // -----  end of class Camera  -----

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  getCamera
// Description:  Returns the camera matrix
//-----------------------------------------------------------------------------
    inline glm::mat4&
Camera::getCamera ()
{
    return cameraMatrix_;
}		// -----  end of method Camera::getCamera  -----

#endif   // ----- #ifndef CAMERA_H  -----
