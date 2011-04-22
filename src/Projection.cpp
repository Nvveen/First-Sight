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
    perspective.aspectRatio = aspectRatio;
    perspective.FOV = FOV;
    perspective.zNear = zNear;
    perspective.zFar = zFar;
    perspectiveMatrix_ = glm::perspective(perspective.FOV, 
                                         perspective.aspectRatio, 
                                         perspective.zNear, perspective.zFar);
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

