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
#include    <glm/glm.hpp>

// ============================================================================
//        Class:  Projection
//  Description:  A projection handling class, that also handles the camera.
// ============================================================================
class Projection
{
    public:

        // ====================  LIFECYCLE     ================================
        Projection () {};
        Projection ( GLfloat aspectRatio, GLfloat FOV=45.0f,
                     GLfloat zNear=0.1f, GLfloat zFar=100.0f );
        void setPerspective ( GLfloat w, GLfloat h );

        // ====================  ACCESSORS     ================================
        glm::mat4& getPerspective ();

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================

        struct Perspective {
            GLfloat FOV;
            GLfloat aspectRatio;
            GLfloat zNear;
            GLfloat zFar;
        } perspective;	// ----------  end of struct Perspective  ----------


        glm::mat4 perspectiveMatrix_;
}; // -----  end of class Projection  -----

//-----------------------------------------------------------------------------
//       Class:  Projection
//      Method:  getPerspective
// Description:  Returns the perspective matrix.
//-----------------------------------------------------------------------------
    inline glm::mat4&
Projection::getPerspective ()
{
    return perspectiveMatrix_;
}		// -----  end of method Projection::getPerspective  -----

#endif   // ----- #ifndef PROJECTION_H  -----
