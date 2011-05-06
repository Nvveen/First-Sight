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
    projectionMatrix_ = glm::perspective(perspective.FOV, 
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
    projectionMatrix_ = glm::perspective(perspective.FOV, 
                                         perspective.aspectRatio, 
                                         perspective.zNear, perspective.zFar);
}		// -----  end of method Projection::setPerspective  -----

