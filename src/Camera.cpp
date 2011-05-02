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
//       Filename:  Camera.cpp
// 
//    Description:  A class that handles the ingame camera.
// 
//        Version:  1.0
//        Created:  04/22/2011 12:16:53 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <glm/gtc/matrix_transform.hpp>
#include    <glm/gtc/type_ptr.hpp>
#include    "Camera.h"

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  Camera
// Description:  constructor
//-----------------------------------------------------------------------------
Camera::Camera ()
{
    // Define a standard set of positions, but adjust for a topdown view.
    cameraVectors_.pos = glm::vec3(0.0f, 5.0f, -5.0f);
    cameraVectors_.target = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraVectors_.up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Make a matrix
    cameraMatrix_ = glm::lookAt(cameraVectors_.pos, cameraVectors_.target, 
                                cameraVectors_.up);
}  // -----  end of method Camera::Camera  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  Camera
// Description:  constructor
//-----------------------------------------------------------------------------
Camera::Camera ( GLfloat pos[3], GLfloat target[3], GLfloat up[3] )
{
    // If the provided vectors aren't specified, use a default one.
    cameraVectors_.pos = glm::vec3(pos[0], pos[1], pos[2]);
    cameraVectors_.target = glm::vec3(target[0], target[1], target[2]);
    cameraVectors_.up = glm::vec3(up[0], up[1], up[2]);
    
    cameraMatrix_ = glm::lookAt(cameraVectors_.pos, cameraVectors_.target, 
                                cameraVectors_.up);
}		// -----  end of method Projection::setCamera  -----

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  rotate
// Description:  Rotates the camera.
//-----------------------------------------------------------------------------
    void
Camera::rotate ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
    // With rotation, all 3 vectors get adjusted.
    glm::vec4 pos(cameraVectors_.pos, 1.0f),
              target(cameraVectors_.target, 1.0f),
              up(cameraVectors_.up, 1.0f);
    pos    = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(x, y, z)) * pos;
    target = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(x, y, z)) * target;
    up     = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(x, y, z)) * up;

    // Set the new values in the matrix vectors.
    cameraVectors_.pos    = glm::vec3(pos.x, pos.y, pos.z);
    cameraVectors_.target = glm::vec3(target.x, target.y, target.z);
    cameraVectors_.up     = glm::vec3(up.x, up.y, up.z);

    // Make the matrix
    cameraMatrix_ = glm::lookAt(cameraVectors_.pos, cameraVectors_.target,
                                 cameraVectors_.up);
}		// -----  end of method Camera::rotate  -----

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  move
// Description:  Moves the camera.
//-----------------------------------------------------------------------------
    void
Camera::move ( GLfloat x, GLfloat y, GLfloat z )
{
    // Get a forward facing vector
    glm::vec3 forward = cameraVectors_.target - cameraVectors_.pos;
    forward = glm::normalize(forward);
    // We don't need the y component.
    forward[1] = 0;
    // Generate a vector that gives the translation needed per component.
    glm::vec3 translation = z * forward + y * cameraVectors_.up;
    translation += x * glm::normalize(glm::cross(cameraVectors_.up, forward));

    // Change the required vectors in th camera.
    cameraVectors_.pos += translation;
    cameraVectors_.target += translation;
    // Make the matrix
    cameraMatrix_ = glm::lookAt(cameraVectors_.pos, cameraVectors_.target, 
                                cameraVectors_.up);
}		// -----  end of method Camera::move  -----

