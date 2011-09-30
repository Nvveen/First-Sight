// ============================================================================
// 
//       Filename:  Drawable.h
// 
//    Description:  Drawables are objects composed of an interface,
//                  that handles interfacing with OpenGL states, and objects
//                  that compose the vertex data.
// 
//        Version:  1.0
//        Created:  08/25/2011 01:50:26 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <glm/glm.hpp>
#include    <GL/glew.h>
#include    <array>
#include    "Context.h"
#include    "Shader.h"

// ============================================================================
//        Class:  DrawableInterface
//  Description:  Interface to OpenGL state changing.
// ============================================================================
class DrawableInterface
{
    public:
        DrawableInterface ();
        DrawableInterface ( Context &context,
                            Shader &shader=Context::shaders["default"] );
    protected:
        Shader *shader_;
        Perspective *pers_;

        void setDrawState ();
        void setMVP ( bool force=false );
        
        // Don't allow these methods to be overwritten, so no
        // virtual methods.
        void move ( const GLfloat& x, const GLfloat&, const GLfloat& z );
        void rotate ( const GLfloat& angle, const std::array<float, 3>& axis );
    private:
        Camera *cam_;
        
        glm::mat4 mvp_;
        glm::mat4 translation_, rotation_, scaling_;

}; // -----  end of class DrawableInterface  -----

// ============================================================================
//        Class:  DrawableObject
//  Description:  Object class that handles the creation, deletion and rendering
//                of an object.
// ============================================================================
class DrawableObject
{
    public:
        DrawableObject ();
        virtual void draw ();
    protected:
        typedef unsigned int Uint;

        GLenum texType_;
        unsigned int itemCount_;

        virtual void createVertexBuffer ( GLfloat *buffer );
        virtual void createTexture ( GLfloat *texBuffer, 
                                     Uint w, Uint h, Uint d=0 );
    private:
        GLuint vao_, vbo_;
        GLuint texID_;
}; // -----  end of class DrawableObject  -----

// ============================================================================
//        Class:  Drawable
//  Description:  Standard class that combines the interface and object.
// ============================================================================
class Drawable : public DrawableInterface, DrawableObject
{
    public:
        Drawable ();
        virtual void draw ();
    protected:
    private:
}; // -----  end of class Drawable  -----

