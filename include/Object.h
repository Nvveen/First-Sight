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
//       Filename:  Object.h
// 
//    Description:  Base object class.
// 
//        Version:  1.0
//        Created:  04/07/2011 11:44:46 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  OBJECT_H
#define  OBJECT_H

#include    <string>
#include    "Model.h"
#include    "Projection.h"
#include    "Camera.h"
#include    "Shader.h"
#include    "Context.h"

// ============================================================================
//        Class:  Object
//  Description:  Base class for OpenGL objects.
// ============================================================================
class Object
{
    public:

        // ====================  LIFECYCLE     ================================
        Object ( std::string fileName, float x, float y, float z,
                 Context *context=NULL, Shader *shader=NULL );
        ~Object ();
        void draw ();

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================
        void bind ( Camera *camera );
        void bind ( Projection *projection );
        void setMVP( bool force=false );

        void translate ( GLfloat x, GLfloat y, GLfloat z );

        // ====================  OPERATORS     ================================

        // ====================  DATA MEMBERS  ================================
    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  LIFECYCLE     ================================
        void init ();
        // ====================  DATA MEMBERS  ================================
        std::string fileName_;
        Model *model_;
        float x_;
        float y_;
        float z_;

        Shader *shader_;

        Camera *camera_;
        Projection *projection_;
        glm::mat4 translation_;
        glm::mat4 rotation_;
        glm::mat4 scaling_;
        glm::mat4 mvp_;

}; // -----  end of class Object  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  bind
// Description:  (Re)binds the camera to the object.
//-----------------------------------------------------------------------------
    inline void
Object::bind ( Camera *camera )
{
    camera_ = camera;
}		// -----  end of method Object::bind  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  bind
// Description:  (Re)binds the projection to the object.
//-----------------------------------------------------------------------------
    inline void
Object::bind ( Projection *projection )
{
    projection_ = projection;
}		// -----  end of method Object::bind  -----

#endif   // ----- #ifndef OBJECT_H  -----
