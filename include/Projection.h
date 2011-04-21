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
#include    <GL/freeglut.h>
#include    <glm/glm.hpp>

// ============================================================================
//        Class:  Projection
//  Description:  A projection handling class, that also handles the camera.
// ============================================================================
class Projection
{
    public:

        // ====================  LIFECYCLE     ================================
        Projection ();                             // constructor
        void setPerspective ( GLfloat FOV, GLfloat aspectRatio, GLfloat zNear, 
                              GLfloat zFar );
        void setCamera ( GLfloat pos[3]=NULL, GLfloat target[3]=NULL, 
                         GLfloat up[3]=NULL );
        void bind ( const GLuint& shader );
        void moveCamera ( int key );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

        glm::mat4 perspectiveMatrix;
        glm::mat4 cameraMatrix;

    private:
        // ====================  DATA MEMBERS  ================================

        struct Perspective {
            GLfloat FOV;
            GLfloat aspectRatio;
            GLfloat zNear;
            GLfloat zFar;
        } perspective;	// ----------  end of struct Perspective  ----------

        struct Camera {
            glm::vec3 pos;
            glm::vec3 target;
            glm::vec3 up;
        } camera;		// ----------  end of struct Camera  ----------
}; // -----  end of class Projection  -----

#endif   // ----- #ifndef PROJECTION_H  -----
