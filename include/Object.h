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
#include    "Context.h"
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
        Object ( Uint8 x=0, Uint8 y=0, Uint8 z=0,  Shader *shader=NULL );
        Object ( std::string datName, Context& context, 
                 Uint8 x=0, Uint8 y=0, Uint8 z=0, Shader *shader=NULL );
        ~Object ();
        virtual void draw ();
        virtual void setUniforms ();

        void translate ( GLfloat x, GLfloat y, GLfloat z );
        void translateGrid ( unsigned int x, unsigned int y, unsigned int z );
        void rotate ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
        void scale ( GLfloat x, GLfloat y, GLfloat z );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================
        void bind ( Context& context, bool isOrtho=false );
        void push ( GLfloat vert[3], GLfloat tex[2], GLfloat normal[3] );
        void setColor ( GLfloat r, GLfloat g, GLfloat b, GLfloat a );

        // ====================  OPERATORS     ================================
        Object& operator= ( Object const& r);

        // ====================  DATA MEMBERS  ================================
        static std::map<std::string, Object> objectCache;
    protected:
        // ====================  DATA MEMBERS  ================================
        std::vector<GLfloat> modelData_;

        GLfloat width_;
        GLfloat height_;
        GLfloat depth_;
        Uint8 x_;
        Uint8 y_;
        Uint8 z_;
        
        Texture *texture_;
        Shader *shader_;
        Projection *projection_;
        Camera *camera_;

        glm::mat4 translation_;
        glm::mat4 rotation_;
        glm::mat4 scaling_;
        glm::vec4 color_;

        virtual void initVertexBuffer ();
        virtual void initUniformBuffer ();

    private:
        // ====================  DATA MEMBERS  ================================

        size_t triangleCount_;

        GLuint vbo_;
        GLuint vao_;
        GLuint projectionUBO_;
        GLuint modelUBO_;
        GLuint textureUBO_;

        Model *model_;
        
        bool              readDat ( std::string datName );
        static  GLfloat toRadians ( GLfloat angle );
        GLfloat           getSize ( int axis );
        void                 bind ( Projection *proj, Camera *cam );

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
// Description:  Binds a camera and projection to an object.
//-----------------------------------------------------------------------------
    inline void
Object::bind ( Context& context, bool isOrtho )
{
    if ( !isOrtho )
        bind(context.getPerspective(), context.getCamera());
    else
        bind(context.getOrtho(), context.getCamera());
}		// -----  end of method Object::bind  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  bind
// Description:  Binds a projection to an object.
//-----------------------------------------------------------------------------
    inline void
Object::bind ( Projection *proj, Camera *cam )
{
    projection_ = proj;
    if ( cam != NULL ) camera_ = cam;
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

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  operator=
// Description:  Handles the assignment operation correctly.
//-----------------------------------------------------------------------------
    inline Object&
Object::operator= ( Object const& r )
{
    model_ = r.model_;
    texture_ = r.texture_;
    modelData_ = r.modelData_;
    projection_ = r.projection_;
    camera_ = r.camera_;
    width_ = r.width_;
    height_ = r.height_;
    depth_ = r.depth_;
    triangleCount_ = r.triangleCount_;
    vbo_ = r.vbo_;
    vao_ = r.vao_;
    projectionUBO_ = r.projectionUBO_;
    modelUBO_ = r.modelUBO_;
    textureUBO_ = r.textureUBO_;
    translation_ = r.translation_;
    rotation_ = r.rotation_;
    scaling_ = r.scaling_;
    color_ = r.color_;
    return *this;
}		// -----  end of method Object::operator=  -----

#endif   // ----- #ifndef OBJECT_H  -----
