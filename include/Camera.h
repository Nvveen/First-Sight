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

        // ====================  LIFECYCLE     ================================
        Camera ();
        Camera ( GLfloat pos[3], GLfloat target[3], GLfloat up[3] ); 

        // ====================  ACCESSORS     ================================
        glm::mat4& getCamera ();

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
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
