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
Projection::Projection ()
{
    perspectiveMatrix = glm::mat4(1.0f);
    cameraMatrix = glm::mat4(1.0f);
}  // -----  end of method Projection::Projection  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  setPerspective
// Description:  Sets the entire perspective from an orthogonal one.
//-----------------------------------------------------------------------------
    void
Projection::setPerspective ( GLfloat FOV, GLfloat aspectRatio, GLfloat zNear, 
                             GLfloat zFar )
{
    perspective.FOV = FOV;
    perspective.aspectRatio = aspectRatio;
    perspective.zNear = zNear;
    perspective.zFar = zFar;
    perspectiveMatrix = glm::perspective(perspective.FOV, 
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
    cameraMatrix = glm::lookAt(camera.pos, camera.target, camera.up);
    cameraMatrix = glm::rotate(cameraMatrix, 45.0f, 
                               glm::vec3(1.0f, 0.0f, 0.0f));
}		// -----  end of method Projection::setCamera  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  bind
// Description:  Binds the projection matrices to the shader
//-----------------------------------------------------------------------------
    void
Projection::bind ( const GLuint& shader )
{
    // Since the starting point needs to be moved back, multiply the camera
    // (could have use the projectino matrix too), with a translation matrix
    // that applies to all bound objects.

    GLint locProjection = glGetUniformLocation(shader, "vProjection");
    glUniformMatrix4fv(locProjection, 1, GL_FALSE, 
                       glm::value_ptr(perspectiveMatrix));
    GLint locCamera = glGetUniformLocation(shader, "vCamera");
    glUniformMatrix4fv(locCamera, 1, GL_FALSE, 
                       glm::value_ptr(cameraMatrix));
}		// -----  end of method Projection::bind  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  moveCamera
// Description:  Moves the camera according to the vector specified.
//-----------------------------------------------------------------------------
    void
Projection::moveCamera ( int key )
{
    switch (key) {
        case GLUT_KEY_UP:
        {
            cameraMatrix = glm::translate(cameraMatrix, 
                                          glm::vec3(0.0f, 0.0f, -0.1f));
            break;
        }
        case GLUT_KEY_DOWN:
        {
            cameraMatrix = glm::translate(cameraMatrix, 
                                          glm::vec3(0.0f, 0.0f, 0.1f));
            break;
        }
        case GLUT_KEY_LEFT:
        {
            cameraMatrix = glm::translate(cameraMatrix,
                                          glm::vec3(0.1f, 0.0f, 0.0f));
            break;
        }
        case GLUT_KEY_RIGHT:
        {
            cameraMatrix = glm::translate(cameraMatrix,
                                          glm::vec3(-0.1f, 0.0f, 0.0f));
            break;
        }
        case GLUT_KEY_PAGE_UP:
        {
            cameraMatrix = glm::translate(cameraMatrix,
                                          glm::vec3(0.0f, -0.1f, 0.0f));
            break;
        }
        case GLUT_KEY_PAGE_DOWN:
        {
            break;
            cameraMatrix = glm::translate(cameraMatrix,
                                          glm::vec3(0.0f, -0.1f, 0.0f));
        }
    }
}		// -----  end of method Projection::moveCamera  -----

