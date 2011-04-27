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
    cameraVectors_.pos = glm::vec3(0.0f, 0.0f, 7.0f);
    cameraVectors_.target = glm::vec3(0.0f, 0.0f, 1.0f);
    cameraVectors_.up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create 3 new 4-element vectors.
    glm::vec4 pos(cameraVectors_.pos, 1.0f), 
              target(cameraVectors_.target, 1.0f), 
              up(cameraVectors_.up, 1.0f);
    // Rotate every vector
    pos    = glm::rotate(glm::mat4(1.0f), -45.0f, glm::vec3(1.0f, 0, 0)) * 
             pos;
    target = glm::rotate(glm::mat4(1.0f), -45.0f, glm::vec3(1.0f, 0, 0)) *
             target;
    up     = glm::rotate(glm::mat4(1.0f), -45.0f, glm::vec3(1.0f, 0, 0)) *
             up;

    // Set the new vectors values in memory.
    cameraVectors_.pos = glm::vec3(pos.x, pos.y, pos.z);
    cameraVectors_.target = glm::vec3(target.x, target.y, target.z);
    cameraVectors_.up = glm::vec3(up.x, up.y, up.z);

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
    // To move, we don't need to adjust the up vector.
    glm::vec4 pos(cameraVectors_.pos, 1.0f),
              target(cameraVectors_.target, 1.0f);
    pos    = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * pos;
    target = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * target;

    // Set the new values in the matrix vectors.
    cameraVectors_.pos    = glm::vec3(pos.x, pos.y, pos.z);
    cameraVectors_.target = glm::vec3(target.x, target.y, target.z);

    // Make the matrix
    cameraMatrix_ = glm::lookAt(cameraVectors_.pos, cameraVectors_.target, 
                                cameraVectors_.up);
}		// -----  end of method Camera::move  -----

