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
    cameraVectors_.pos = glm::vec3(0.0f, 0.0f, 7.0f);
    cameraVectors_.target = glm::vec3(0.0f, 0.0f, 1.0f);
    cameraVectors_.up = glm::vec3(0.0f, 1.0f, 0.0f);

    cameraMatrix_ = glm::lookAt(cameraVectors_.pos, cameraVectors_.target, 
                                cameraVectors_.up);
    cameraMatrix_ = glm::rotate(cameraMatrix_, 45.0f, 
                               glm::vec3(1.0f, 0.0f, 0.0f));
    rotate.cam_ = this;
    move.cam_ = this;
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
    cameraMatrix_ = glm::rotate(cameraMatrix_, 45.0f, 
                               glm::vec3(1.0f, 0.0f, 0.0f));
    rotate.cam_ = this;
    move.cam_ = this;
}		// -----  end of method Projection::setCamera  -----

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  rotateF
// Description:  Rotate the camera
//-----------------------------------------------------------------------------
    void
Camera::rotateF ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
    cameraMatrix_ = glm::rotate(cameraMatrix_, angle, glm::vec3(x, y, z));
}		// -----  end of method Camera::rotateF  -----

//-----------------------------------------------------------------------------
//       Class:  Camera
//      Method:  moveF
// Description:  Move the camera.
//-----------------------------------------------------------------------------
    void
Camera::moveF ( GLfloat x, GLfloat y, GLfloat z )
{
    cameraMatrix_ = glm::translate(cameraMatrix_, glm::vec3(x, y, z));
}		// -----  end of method Camera::moveF  -----

