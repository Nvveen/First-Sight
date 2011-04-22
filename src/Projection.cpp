// ============================================================================
// 
//       Filename:  Projection.cpp
// 
//    Description:  A class that handles the projection of the objects in 3
//                  dimensional space, along with the camera.
// 
//        Version:  1.0
//        Created:  04/08/2011 06:04:53 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <iostream>
#include    <glm/gtc/matrix_transform.hpp>
#include    <glm/gtc/type_ptr.hpp>
#include    "Projection.h"

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  Projection
// Description:  constructor
//-----------------------------------------------------------------------------
Projection::Projection ( GLfloat aspectRatio, GLfloat FOV, GLfloat zNear,
                         GLfloat zFar )
{
    cameraMatrix_ = glm::mat4(1.0f);

    perspective.aspectRatio = aspectRatio;
    perspective.FOV = FOV;
    perspective.zNear = zNear;
    perspective.zFar = zFar;
    perspectiveMatrix_ = glm::perspective(perspective.FOV, 
                                         perspective.aspectRatio, 
                                         perspective.zNear, perspective.zFar);
    setCamera();
}  // -----  end of method Projection::Projection  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  setPerspective
// Description:  Sets the perspective matrix. 
//-----------------------------------------------------------------------------
    void
Projection::setPerspective ( GLfloat w, GLfloat h )
{
    perspective.aspectRatio = w/h;
    perspectiveMatrix_ = glm::perspective(perspective.FOV, 
                                         perspective.aspectRatio, 
                                         perspective.zNear, perspective.zFar);
}		// -----  end of method Projection::setPerspective  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  setCamera
// Description:  Sets the camera to a default view, or one specified by the
//               user.
//-----------------------------------------------------------------------------
    void
Projection::setCamera ( GLfloat pos[3], GLfloat target[3], GLfloat up[3] )
{
    // If the provided vectors aren't specified, use a default one.
    if ( pos == NULL && target == NULL && up == NULL ) {
        camera.pos = glm::vec3(0.0f, 0.0f, 7.0f);
        camera.target = glm::vec3(0.0f, 0.0f, 1.0f);
        camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else {
        camera.pos = glm::vec3(pos[0], pos[1], pos[2]);
        camera.target = glm::vec3(target[0], target[1], target[2]);
        camera.up = glm::vec3(up[0], up[1], up[2]);
    }
    cameraMatrix_ = glm::lookAt(camera.pos, camera.target, camera.up);
    cameraMatrix_ = glm::rotate(cameraMatrix_, 45.0f, 
                               glm::vec3(1.0f, 0.0f, 0.0f));
}		// -----  end of method Projection::setCamera  -----

