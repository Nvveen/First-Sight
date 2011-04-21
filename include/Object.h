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
        Object ( std::string datName, Shader& shader );
        ~Object ();
        GLfloat toRadians ( GLfloat angle );
        void readDat( std::string datName );
        void init ();
        void draw ();
        void translate ( GLfloat x, GLfloat y, GLfloat z );
        void rotate ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
        void scale ( GLfloat x, GLfloat y, GLfloat z );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        Shader* shader_;
        GLuint vbo_;
        GLuint vao_;

        Model* model_;
        Texture* texture_;
        std::vector<GLfloat> modelData_;
        
        glm::mat4 translation;
        glm::mat4 rotation;
        glm::mat4 scaling;

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

#endif   // ----- #ifndef OBJECT_INC  -----
