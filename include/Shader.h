// ============================================================================
// 
//       Filename:  Shader.h
// 
//    Description:  A class that handles shader creation.
// 
//        Version:  1.0
//        Created:  04/07/2011 02:21:02 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  SHADER_H
#define  SHADER_H

#include    <string>
#include    <GL/gl.h>

// ============================================================================
//        Class:  Shader
//  Description:  Shader class, for compilation, linking and using.
// ============================================================================
class Shader
{
    public:

        // ====================  LIFECYCLE     ================================
        Shader ();
        Shader ( std::string vertexShaderFile, std::string fragmentShaderFile );
        std::string addCode ( std::string fileName );
        GLuint compileShader ( std::string code, GLenum type );
        void createProgram ();

        // ====================  ACCESSORS     ================================
        GLuint getShaderProgram();

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================
        GLuint shaderProgram;

    private:
        // ====================  DATA MEMBERS  ================================
        std::string vertexShaderCode_;
        std::string fragmentShaderCode_;
        GLuint vertexShaderObject_;
        GLuint fragmentShaderObject_;

}; // -----  end of class Shader  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  getShaderProgram
// Description:  Returns the shader program object.
//-----------------------------------------------------------------------------
    inline GLuint
Shader::getShaderProgram ()
{
    return shaderProgram;
}		// -----  end of method Shader::getShaderProgram  -----

#endif   // ----- #ifndef SHADER_H  -----
