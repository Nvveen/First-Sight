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
#include    "Functionoid.h"

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

        glm::mat4& getCamera ();
        void rotateF ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
        void moveF ( GLfloat x, GLfloat y, GLfloat z );

        class CameraRotateFnd : public Functionoid
        {
            public:
                virtual void operator() ();
                void operator() ( GLfloat angle, GLfloat x, GLfloat y, 
                                  GLfloat z );
                friend class Camera;
            private:
                Camera* cam_;
                GLfloat angle_, x_, y_, z_;
        }rotate;

        class CameraMoveFnd : public Functionoid
        {
            public:
                virtual void operator() ();
                void operator() ( GLfloat x, GLfloat y, GLfloat z );
                friend class Camera;
            private:
                Camera* cam_;
                GLfloat x_, y_, z_;
        }move;

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

    inline void
Camera::CameraRotateFnd::operator() ()
{
    cam_->rotateF(angle_, x_, y_, z_);
}		// -----  end of method CameraRotateFnd::operator()  -----

    inline void
Camera::CameraRotateFnd::operator() ( GLfloat angle, GLfloat x, GLfloat y,
                                      GLfloat z )
{
    angle_ = angle;
    x_ = x; y_ = y; z_ = z;
}		// -----  end of method CameraRotateFnd::operator()  -----

    inline void
Camera::CameraMoveFnd::operator() ()
{
    cam_->moveF(x_, y_, z_);
}		// -----  end of method CameraMoveFnd::operator()  -----

    inline void
Camera::CameraMoveFnd::operator() ( GLfloat x, GLfloat y, GLfloat z )
{
    x_ = x; y_ = y; z_ = z;
}		// -----  end of method CameraMoveFnd::operator()  -----

#endif   // ----- #ifndef CAMERA_H  -----
