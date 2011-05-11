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
#include    "Projection.h"
#include    "Camera.h"
#include    "Shader.h"
#include    "Model.h"
#include    "Texture.h"

// ============================================================================
//        Class:  Object
//  Description:  Base class for OpenGL objects.
// ============================================================================
class Object
{
    public:

        // ====================  LIFECYCLE     ================================
        Object ( Shader *shader=NULL );
        Object ( std::string datName, Shader *shader=NULL );
        ~Object ();
        virtual void draw ();

        void translate ( GLfloat x, GLfloat y, GLfloat z );
        void rotate ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
        void scale ( GLfloat x, GLfloat y, GLfloat z );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================
        void bind ( Projection *proj, Camera *cam );
        void push ( GLfloat vert[3], GLfloat tex[2], GLfloat normal[3] );
        void setColor ( GLfloat r, GLfloat g, GLfloat b, GLfloat a );

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================
        std::vector<GLfloat> modelData_;
        Texture *texture_;

        Shader *shader_;

        glm::mat4 projection_;
        glm::mat4 camera_;
        glm::mat4 translation_;
        glm::mat4 rotation_;
        glm::mat4 scaling_;
        glm::vec4 color_;

        virtual void init ();

    private:
        // ====================  DATA MEMBERS  ================================

        size_t triangleCount_;

        GLuint vbo_;

        Model *model_;
        
        void readDat( std::string datName );
        static GLfloat toRadians ( GLfloat angle );
}; // -----  end of class Object  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  toRadians
// Description:  Simple helper function that turns degrees into radians.
//-----------------------------------------------------------------------------
    inline GLfloat
Object::toRadians ( GLfloat angle )
{
    return ( angle * M_PI / 180.0f);
}		// -----  end of method Object::toRadians  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  bind
// Description:  Binds a projection to an object.
//-----------------------------------------------------------------------------
    inline void
Object::bind ( Projection *proj, Camera *cam )
{
    projection_ = proj->getProjection();
    if ( cam != NULL ) camera_ = cam->getCamera();
}		// -----  end of method Object::bind  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  push
// Description:  Pushes a vertex onto the modeldata matrix.
//-----------------------------------------------------------------------------
    inline void
Object::push ( GLfloat vert[3], GLfloat tex[2], GLfloat normal[3] )
{
    modelData_.push_back(vert[0]);
    modelData_.push_back(vert[1]);
    modelData_.push_back(vert[2]);
    modelData_.push_back(tex[0]);
    modelData_.push_back(tex[1]);
    modelData_.push_back(normal[0]);
    modelData_.push_back(normal[1]);
    modelData_.push_back(normal[2]);
}		// -----  end of method Object::push  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  setColor
// Description:  Sets the color (overlay) for the object.
//-----------------------------------------------------------------------------
    inline void
Object::setColor ( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
    color_ = glm::vec4(r, g, b, a);
}		// -----  end of method Object::setColor  -----

#endif   // ----- #ifndef OBJECT_H  -----
