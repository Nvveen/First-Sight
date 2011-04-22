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

#ifndef  OBJECT_INC
#define  OBJECT_INC

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
        Object ();
        Object ( std::string datName, 
                 Shader shader=Shader("default.vs", "default.fs") );
        ~Object ();
        void draw ();

        void translate ( GLfloat x, GLfloat y, GLfloat z );
        void rotate ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
        void scale ( GLfloat x, GLfloat y, GLfloat z );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================
        void bind ( Projection* proj, Camera* cam );

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        Shader shader_;
        GLuint vbo_;
        GLuint vao_;

        Model* model_;
        Texture* texture_;
        Projection* proj_;
        Camera* cam_;
        size_t triangleCount_;
        
        glm::mat4 translation;
        glm::mat4 rotation;
        glm::mat4 scaling;

        void readDat( std::string datName );
        void init ();
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
Object::bind ( Projection* proj, Camera* cam )
{
    proj_ = proj;
    cam_ = cam;
}		// -----  end of method Object::bind  -----

#endif   // ----- #ifndef OBJECT_INC  -----
